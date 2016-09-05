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
#include <stdlib.h> /* for malloc(), free(), and NULL */
#include <string.h> /* for strlen() */
#include "mod.h"

#define EGO ((struct opencl *)ego)

void *
LUX_MKMOD(const struct LuxOopencl *opts)
{
	Lux_opencl *ego = NULL;

	cl_context ctx;
	cl_program pro;

	cl_device_id dev[DEV_COUNT];
	size_t       i, ndev;
	cl_int       err;

	struct LuxOopencl def = OPENCL_NULL;
	if(!opts)
		opts = &def;

	if(opts->ctx) {
		(void)clRetainContext(opts->ctx);
		ctx = opts->ctx;
	} else {
		ctx = mkctx(opts->iplf, opts->idev, opts->devtype);
		if(!ctx)
			return NULL;
	}

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

	if(opts->src) {
		pro = mkpro(EGO, opts, ndev, dev);
	} else
		pro = 0;

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

	(void)err; /* silence "set but not used" warning */
}
