/*
 * Copyright (C) 2016 Chi-kwan Chan
 * Copyright (C) 2016 Steward Observatory
 *
 * This file is part of lux.
 *
 * lux is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lux is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lux.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <lux.h>
#include <lux/dlfcn.h>
#include <lux/mangle.h>
#include <stdlib.h> /* for malloc(), free(), and NULL */
#include <stdio.h>  /* for sprintf() etc */
#include <string.h> /* for strlen() */
#include "mod.h"

#define EGO ((struct opencl *)ego)

#define PLF_COUNT 8  /* 3 bits */
#define DEV_COUNT 32 /* 5 bits */

const char preamble_fmt[] = "\
typedef unsigned whole;\n\
typedef int      integer;\n\
\n\
typedef %s   fast;\n\
typedef %s2  fast2;\n\
typedef %s4  fast4;\n\
typedef %s8  fast8;\n\
typedef %s16 fast16;\n\
\n\
typedef %s   real;\n\
typedef %s2  real2;\n\
typedef %s4  real4;\n\
typedef %s8  real8;\n\
typedef %s16 real16;\n\
\n\
typedef %s   extended;\n\
typedef %s2  extended2;\n\
typedef %s4  extended4;\n\
typedef %s8  extended8;\n\
typedef %s16 extended16;\n\
\n\
#define K(x) x %s\n\
\n\
";

static cl_platform_id
lsplf(Lux_opencl *ego, unsigned iplf)
{
	cl_platform_id p[PLF_COUNT];
	cl_uint        n, i;

	(void)clGetPlatformIDs(PLF_COUNT, p, &n);

	lux_print("%d platform%s found:\n", n, n > 1 ? "s are" : " is");
	for(i = 0; i < n; ++i) {
		char buf[1024];

		(void)clGetPlatformInfo(p[i], CL_PLATFORM_NAME,
		                        sizeof(buf), buf, NULL);
		lux_print("%s\t%d. %s ", i == iplf ? "*" : "", i, buf);

		(void)clGetPlatformInfo(p[i], CL_PLATFORM_VENDOR,
		                        sizeof(buf), buf, NULL);
		lux_print("by %s: ", buf);

		(void)clGetPlatformInfo(p[i], CL_PLATFORM_VERSION,
		                        sizeof(buf), buf, NULL);
		lux_print("%s\n", buf);
	}

	return p[iplf < n ? iplf : n];

	(void)ego; /* silence unused variable warning */
}

static int
lsdev(Lux_opencl *ego, unsigned iplf, unsigned idev, cl_device_type devtype)
{
	cl_platform_id p[PLF_COUNT];
	cl_device_id   d[DEV_COUNT];
	cl_uint        n, i;

	(void)clGetPlatformIDs(PLF_COUNT, p, NULL);
	(void)clGetDeviceIDs(p[iplf], devtype, DEV_COUNT, d, &n);

	lux_print("%d device%s found:\n", n, n > 1 ? "s are" : " is");
	for(i = 0; i < n; ++i) {
		char buf[1024];

		(void)clGetDeviceInfo(d[i], CL_DEVICE_NAME,
		                      sizeof(buf), buf, NULL);
		lux_print("%s\t%d. %s ", i == idev ? "*" : "", i, buf);

		(void)clGetDeviceInfo(d[i], CL_DEVICE_VENDOR,
		                      sizeof(buf), buf, NULL);
		lux_print("by %s: ", buf);

		(void)clGetDeviceInfo(d[i], CL_DRIVER_VERSION,
		                      sizeof(buf), buf, NULL);
		lux_print("%s\n", buf);
	}

	return EXIT_SUCCESS;

	(void)ego; /* silence unused variable warning */
}

static FILE *
ftryopen(const char *name)
{
	const char *fmt[] = {"%s", "%s.cl"};

	size_t i;
	for(i = 0; i < countof(fmt); ++i) {
		FILE *f;
		char  buf[1024];
		sprintf(buf, fmt[i], name);

		f = fopen(buf, "r");
		if(f) {
			lux_print("Loaded kernel \"%s\"\n", buf);
			return f;
		}
	}
	return NULL;
}

static const char *
freadall(FILE *f)
{
	size_t l;
	char  *s;

	fseek(f, 0, SEEK_END);
	l = ftell(f);
	fseek(f, 0, SEEK_SET);

	s = malloc(l+1);
	fread(s, l, 1, f);
	s[l] = '\0';

	return s;
}

static const char *
getsrc(const char *path, const char *name)
{
	char buf[1024];

	/* Always try to open without path */
	FILE *f = ftryopen(name);
	if(name[0] == '/' || /* done trying if aboslute or */
	   name[0] == '.')   /* explicit relative path     */
		goto nopath;

	if(!f) {
		/* Try to open with an associated directory named as
		   the base module */
		const char *sep = strrchr(path, '.');
		if(sep) {
			size_t l = sep - path;
			memcpy(buf, path, l);
			buf[l] = '/';
			strcpy(buf+l+1, name);
			f = ftryopen(buf);
		}
	}

	if(!f) {
		/* Try to open with at the same directory that
		   contains the base module */
		const char *sep = strrchr(path, '/');
		if(sep) {
			size_t l = sep - path;
			memcpy(buf, path, l);
			buf[l] = '/';
			strcpy(buf+l+1, name);
			f = ftryopen(buf);
		}
	}

 nopath:
	if(!f)
		return NULL;
	else {
		const char *s = freadall(f);
		fclose(f);
		return s;
	}
}

static cl_mem
mk(Lux_opencl *ego, size_t sz, unsigned flags)
{
	return clCreateBuffer(EGO->super.ctx, flags, sz, NULL, NULL);
}

static void
rm(Lux_opencl *ego, cl_mem buf)
{
	(void)clReleaseMemObject(buf);
	(void)ego; /* silence unused variable warning */
}

static cl_mem
h2d(Lux_opencl *ego, cl_mem dst, void *src, size_t sz)
{
	(void)clEnqueueWriteBuffer(ego->que,
	                           dst, CL_TRUE, 0, sz,
	                           src, 0, NULL, NULL);
	return dst;
}

static void *
d2h(Lux_opencl *ego, void *dst, cl_mem src, size_t sz)
{
	(void)clEnqueueReadBuffer(ego->que,
	                          src, CL_TRUE, 0, sz,
	                          dst, 0, NULL, NULL);
	return dst;
}

static void *
mmap(Lux_opencl *ego, cl_mem buf, size_t sz)
{
	return clEnqueueMapBuffer(ego->que, buf,
	                          CL_TRUE, CL_MAP_READ, 0, sz,
	                          0, NULL, NULL, NULL);
}

static void
munmap(Lux_opencl *ego, cl_mem buf, void *host)
{
	(void)clEnqueueUnmapMemObject(ego->que, buf, host,
	                              0, NULL, NULL);
}

void *
LUX_MKMOD(const struct LuxOopencl *opts)
{
	Lux_opencl  *ego = NULL;

	cl_context ctx;
	cl_program pro;

	const char *path;
	cl_context_properties plf[] = {CL_CONTEXT_PLATFORM,
	                               (cl_context_properties)NULL,
	                               (cl_context_properties)NULL};
	char buf[1024];
	const char *src[16] = {buf, NULL};

	cl_device_id dev[DEV_COUNT];
	size_t       i, ndev;
	cl_int       err;

	struct LuxOopencl def = OPENCL_NULL;
	if(!opts)
		opts = &def;

	lux_print("\nGetting OpenCL platforms... ");
	plf[1] = (cl_context_properties)lsplf(NULL, opts->iplf);
	lux_print("\nGetting OpenCL devices from platform %u... ", opts->iplf);
	lsdev(NULL, opts->iplf, opts->idev, opts->devtype);
	lux_print("\n");

	ctx = clCreateContextFromType(plf, opts->devtype, NULL, NULL, &err);
	if(!ctx || err)
		return NULL;

	err = clGetContextInfo(ctx, CL_CONTEXT_DEVICES,
	                       sizeof(dev), dev, &ndev);
	if(err)
		goto cleanup1;

	ndev /= sizeof(cl_device_id);
	lux_print("OpenCL context %p contains device%s %s%p",
	          ctx, ndev > 1 ? "s" : "",
	          opts->idev == 0 ? "* " : "", dev[0]);
	for(i = 1; i < ndev; ++i)
		lux_print(", %s%p",
		          opts->idev == i ? "* " : "", dev[i]);
	lux_print("\n");

	ego = (Lux_opencl *)malloc(sizeof(struct opencl) +
	                           (ndev-1) * sizeof(cl_command_queue));
	if(!ego)
		goto cleanup1;

	EGO->integersz  = sizeof(cl_int);
	EGO->fastsz     = sizeof(float);
	EGO->realsz     = opts->realsz;
	EGO->extendedsz = sizeof(double);
	{
		const char *fn = prectostr(EGO->fastsz);
		const char *rn = prectostr(EGO->realsz);
		const char *xn = prectostr(EGO->extendedsz);
		snprintf(buf, sizeof(buf), preamble_fmt,
		         fn, fn, fn, fn, fn,
		         rn, rn, rn, rn, rn,
		         xn, xn, xn, xn, xn,
		         EGO->realsz>4 ? EGO->realsz>8 ? "## L" : "" : "## f");
	}

	path = dlfname(opts->base ? opts->base : (void *)LUX_MKMOD);
	for(i = 0; opts->src[i]; ++i) {
		if(strlen(opts->src[i]) < 64) { /* UGLY HACK */
			const char *s = getsrc(path, opts->src[i]);
			if(!s) {
				lux_error("Failed to load source\n");
				exit(1);
			}
			src[i+1] = s;
		} else
			src[i+1] = opts->src[i];
	}

	pro = clCreateProgramWithSource(ctx, i+1, src, NULL, &err);
	if(!pro || err) {
		lux_error("Failed to create program\n");
		exit(1);
	}

	err = clBuildProgram(pro, ndev, dev, opts->flags, NULL, NULL);
	if(err != CL_SUCCESS) {
		lux_error("Failed to build program\n");
		for(i = 0; i < ndev; ++i) {
			size_t len;
			char   buf[10240];
			clGetProgramBuildInfo(pro, dev[i], CL_PROGRAM_BUILD_LOG,
			                      sizeof(buf), buf, &len);
			lux_error("%s\n", buf);
		}
		exit(1);
	}

	ego->lsplf  = lsplf;
	ego->lsdev  = lsdev;
	ego->mkkern = mkkern;
	ego->exec   = exec;
	ego->rmkern = rmkern;
	ego->mk     = mk;
	ego->rm     = rm;
	ego->h2d    = h2d;
	ego->d2h    = d2h;
	ego->mmap   = mmap;
	ego->munmap = munmap;

	EGO->pro    = pro;
	EGO->nqueue = ndev;
	for(i = 0; i < ndev; ++i) {
		cl_command_queue q =
			clCreateCommandQueue(ctx, dev[i],
			                     CL_QUEUE_PROFILING_ENABLE,
			                     &err);
		if(!q || err)
			goto cleanup2;
		EGO->queue[i] = q;
		if(i == opts->idev) {
			ego->ctx = ctx;
			ego->dev = dev[i];
			ego->que = q;
		}
	}
	return ego;

 cleanup2:
	while(i--) {
		err = clReleaseCommandQueue(ego->que);
		/* TODO: check error */
	}
 cleanup1:
	err = clReleaseContext(ctx);
	/* TODO: check error */
	free(ego);
	return NULL;
}

void
LUX_RMMOD(void *ego)
{
	cl_int err;
	size_t i = EGO->nqueue;
	while(i--) {
		err = clReleaseCommandQueue(EGO->queue[i]);
		/* TODO: check error */
	}
	err = clReleaseContext(EGO->super.ctx);
	/* TODO: check error */
	free(ego);
}
