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

cl_context
mkctx(unsigned iplf, unsigned idev, cl_device_type devtype)
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
