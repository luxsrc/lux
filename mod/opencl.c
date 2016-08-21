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
#include <lux/mangle.h>
#include <stdlib.h> /* for malloc(), free(), and NULL */
#include <stdio.h>  /* for sprintf() etc */
#include <string.h> /* for strlen() */
#include "opencl.h"

#define COUNT_MAX 16

static int
lsplf(void)
{
	cl_platform_id p[COUNT_MAX];
	cl_uint        n, i;

	(void)clGetPlatformIDs(COUNT_MAX, p, &n);

	lux_print("%d platform%s found:\n", n, n > 1 ? "s are" : " is");
	for(i = 0; i < n; ++i) {
		char buf[1024];

		(void)clGetPlatformInfo(p[i], CL_PLATFORM_NAME,
		                        sizeof(buf), buf, NULL);
		lux_print("\t%d. %s ", i, buf);

		(void)clGetPlatformInfo(p[i], CL_PLATFORM_VENDOR,
		                        sizeof(buf), buf, NULL);
		lux_print("by %s: ", buf);

		(void)clGetPlatformInfo(p[i], CL_PLATFORM_VERSION,
		                        sizeof(buf), buf, NULL);
		lux_print("%s\n", buf);
	}

	return EXIT_SUCCESS;
}

static int
lsdev(unsigned iplf)
{
	cl_platform_id p[COUNT_MAX];
	cl_device_id   d[COUNT_MAX];
	cl_uint        n, i;

	(void)clGetPlatformIDs(COUNT_MAX, p, NULL);
	(void)clGetDeviceIDs(p[iplf], CL_DEVICE_TYPE_ALL, COUNT_MAX, d, &n);

	lux_print("%d device%s found:\n", n, n > 1 ? "s are" : " is");
	for(i = 0; i < n; ++i) {
		char buf[1024];

		(void)clGetDeviceInfo(d[i], CL_DEVICE_NAME,
		                      sizeof(buf), buf, NULL);
		lux_print("\t%d. %s ", i, buf);

		(void)clGetDeviceInfo(d[i], CL_DEVICE_VENDOR,
		                      sizeof(buf), buf, NULL);
		lux_print("by %s: ", buf);

		(void)clGetDeviceInfo(d[i], CL_DRIVER_VERSION,
		                      sizeof(buf), buf, NULL);
		lux_print("%s\n", buf);
	}

	return EXIT_SUCCESS;
}

static FILE *
ftryopen(const char *name)
{
	const char *fmt[] = {
		"%s",
		"%s.cl",
		LUX_MOD_PATH "/%s",
		LUX_MOD_PATH "/%s.cl"
	};

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
getsrc(const char *name)
{
	FILE *f = ftryopen(name);
	if(f) {
		const char *s = freadall(f);
		fclose(f);
		return s;
	}
	return NULL;
}

static cl_kernel
mkkern(Lux_opencl *ego, const char *name)
{
	cl_int    err;
	cl_kernel k = clCreateKernel(ego->program, name, &err);
	if(!k || err != CL_SUCCESS) {
		lux_error("Failed to obtain compute kernel \"%s\"\n", name);
		exit(1);
	}
	return k;
}

static void
rmkern(cl_kernel k)
{
	(void)clReleaseKernel(k);
}

static cl_mem
mk(cl_context context, unsigned flags, size_t sz)
{
	return clCreateBuffer(context, flags, sz, NULL, NULL);
}

static void
rm(cl_mem buf)
{
	(void)clReleaseMemObject(buf);
}

static double
exec(Lux_opencl *ego, cl_kernel kern,
     size_t dim, const size_t *gsz, const size_t *bsz)
{
	cl_event event;
	cl_ulong t0, t1;

	/* TODO: automatic load balancing across devices */
	clEnqueueNDRangeKernel(ego->queue[0], kern,
	                       dim, NULL, gsz,  bsz, 0, NULL, &event);
	clWaitForEvents(1, &event);

	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START,
	                        sizeof(t0), &t0, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END,
	                        sizeof(t1), &t1, NULL);

	return (double)(t1 - t0);
}

void *
LUX_MKMOD(const struct LuxOopencl *opts)
{
	Lux_opencl  *ego = NULL;
	cl_context   ctx;
	cl_program   pro;
	cl_device_id dev[COUNT_MAX];
	size_t       i, ndev;
	cl_int       err;

	struct LuxOopencl def = OPENCL_NULL;
	if(!opts)
		opts = &def;

	lux_print("\nGetting OpenCL platforms... ");
	lsplf();
	lux_print("\nGetting OpenCL devices from platform %u... ", opts->iplf);
	lsdev(opts->iplf);
	lux_print("\n");

	ctx = clCreateContextFromType(NULL, opts->devtype, NULL, NULL, &err);
	if(!ctx || err)
		return NULL;

	err = clGetContextInfo(ctx, CL_CONTEXT_DEVICES, sizeof(dev), dev, &ndev);
	if(err)
		goto cleanup1;

	ndev /= sizeof(cl_device_id);
	lux_print("OpenCL context %p contains device%s %p",
	          ctx, ndev > 1 ? "s" : "", dev[0]);
	for(i = 1; i < ndev; ++i)
		lux_print(", %p", dev[i]);
	lux_print("\n");

	ego = (Lux_opencl *)malloc(sizeof(Lux_opencl) +
	                           (ndev-1) * sizeof(cl_command_queue));
	if(!ego)
		goto cleanup1;

	for(i = 0; opts->src[i]; ++i) {
		if(strlen(opts->src[i]) < 64) { /* UGLY HACK */
			const char *s = getsrc(opts->src[i]);
			if(!s) {
				lux_error("Failed to load source\n");
				exit(1);
			}
			opts->src[i] = s;
		}
	}
	pro = clCreateProgramWithSource(ctx, i, opts->src, NULL, &err);
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

	ego->super   = ctx;
	ego->program = pro;
	ego->mkkern  = mkkern;
	ego->rmkern  = rmkern;
	ego->mk      = mk;
	ego->rm      = rm;
	ego->exec    = exec;
	ego->nqueue  = ndev;
	for(i = 0; i < ndev; ++i) {
		cl_command_queue q = clCreateCommandQueue(ctx, dev[i], 0, &err);
		if(!q || err)
			goto cleanup2;
		ego->queue[i] = q;
		if(i == opts->idev)
			ego->que = q;
	}
	return ego;

 cleanup2:
	while(i--) {
		err = clReleaseCommandQueue(ego->queue[i]);
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
	size_t i = ((Lux_opencl *)ego)->nqueue;
	while(i--) {
		err = clReleaseCommandQueue(((Lux_opencl *)ego)->queue[i]);
		/* TODO: check error */
	}
	err = clReleaseContext(((Lux_opencl *)ego)->super);
	/* TODO: check error */
	free(ego);
}
