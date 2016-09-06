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

void *
LUX_MKMOD(const struct LuxOopencl *opts)
{
	struct opencl *EGO;
	Lux_opencl    *ego;

	cl_context ctx;

	size_t       i, ndev;
	cl_device_id dev[DEV_COUNT];

	struct LuxOopencl def = OPENCL_NULL;
	if(!opts)
		opts = &def;

	if(opts->nque && opts->que)
		/* Copy context, ... */
		ctx = mkctx_que(0, opts->nque, opts->que);
	else
		/* Create new context, ... */
		ctx = mkctx_spec(opts->iplf, opts->idev, opts->devtype);

	if(!ctx)
		goto cleanup1;

	check(GetContextInfo,
	      ctx, CL_CONTEXT_DEVICES, sizeof(dev), dev, &i);
	ndev = i / sizeof(dev[0]);

	EGO = malloc(sizeof(struct opencl) - sizeof(cl_command_queue)
	                            + ndev * sizeof(cl_command_queue));
	if(!EGO)
		goto cleanup2;

	ego = &EGO->super;

	ego->ctx    = ctx;
	ego->mk     = mk;
	ego->rm     = rm;
	ego->h2d    = h2d;
	ego->d2h    = d2h;
	ego->mmap   = mmap;
	ego->munmap = munmap;
	ego->mkkern = mkkern;
	ego->exec   = exec;
	ego->rmkern = rmkern;

	ego->nque   = ndev;
	for(i = 0; i < ndev; ++i) {
		cl_command_queue q =
			safe(cl_command_queue, CreateCommandQueue,
			     ctx, dev[i], CL_QUEUE_PROFILING_ENABLE);
		if(!q)
			goto cleanup3;

		ego->que[i] = q;
	}

	EGO->integersz  = sizeof(cl_int);
	EGO->fastsz     = sizeof(float);
	EGO->realsz     = opts->realsz;
	EGO->extendedsz = sizeof(double);

	EGO->pro = opts->src ? mkpro(EGO, opts, ndev, dev) : 0;

	return ego;

cleanup3:
	while(i--)
		check(ReleaseCommandQueue, ego->que[i]);

cleanup2:
	free(EGO);

cleanup1:
	check(ReleaseContext, ctx);

	return NULL;
}

void
LUX_RMMOD(void *ego)
{
	size_t i = ((Lux_opencl *)ego)->nque;
	while(i--)
		check(ReleaseCommandQueue, ((Lux_opencl *)ego)->que[i]);

	check(ReleaseContext, ((Lux_opencl *)ego)->ctx);

	free(headerof(struct opencl, ego, super));
}
