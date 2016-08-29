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
#include <string.h> /* for strlen() */
#include "mod.h"

#define EGO ((struct opencl *)ego)

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

	snprintf(buf, sizeof(buf), "-cl-kernel-arg-info %s", opts->flags);
	err = clBuildProgram(pro, ndev, dev, buf, NULL, NULL);
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
