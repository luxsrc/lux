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
#include <lux/lazybuf.h>
#include <stdlib.h> /* for NULL */
#include "mod.h"

cl_platform_id
mkplf_dev(cl_platform_id plf, size_t ndev, cl_device_id *dev)
{
	size_t i;
	for(i = 0; dev && i < ndev; ++i) {
		cl_platform_id p;
		check(GetDeviceInfo,
		      dev[i], CL_DEVICE_PLATFORM, sizeof(p), &p, NULL);

		if(!plf) {
			lux_print("Infer platform %p from device[%zu] %p\n",
			          p, i, dev[i]);
			plf = p;
		} else if(plf != p)
			lux_error("WARNING: "
			          "inferred platform %p from device[%zu] %p "
			          "do not match platform %p\n",
			          p, i, dev[i], plf);
	}
	return plf;
}

cl_context
mkctx_spec(unsigned iplf, unsigned idev, cl_device_type devtype)
{
	cl_context_properties plf[] = {CL_CONTEXT_PLATFORM,
	                               (cl_context_properties)NULL,
	                               (cl_context_properties)NULL};
	cl_device_id dev;

	lux_print("\nGetting OpenCL platforms... ");
	plf[1] = (cl_context_properties)lsplf(iplf);

	lux_print("\nGetting OpenCL devices from platform %u... ", iplf);
	dev = lsdev(iplf, idev, devtype);

	lux_print("\n");
	return safe(cl_context, CreateContext, plf, 1, &dev, NULL, NULL);
}

cl_context
mkctx_que(cl_context ctx, size_t nque, cl_command_queue *que)
{
	size_t i;
	for(i = 0; que && i < nque; ++i) {
		cl_context c;
		check(GetCommandQueueInfo,
		      que[i], CL_QUEUE_CONTEXT, sizeof(c), &c, NULL);

		if(!ctx) {
			lux_print("Infer context %p from queue[%zu] %p\n",
			          c, i, que[i]);
			ctx = c;
		} else if(ctx != c)
			lux_fatal("inferred context %p from queue[%zu] %p "
			          "do not match context %p\n",
			          c, i, que[i], ctx);
	}
	return ctx;
}
