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
#include "opencl.h"

void *
LUX_MKMOD(const void *opts)
{
	Lux_opencl *ego;

	ego = (Lux_opencl *)malloc(sizeof(Lux_opencl));
	if(ego) {
		cl_int err;
		ego->super = clCreateContextFromType(NULL,
		                                     CL_DEVICE_TYPE_DEFAULT,
		                                     NULL, NULL, &err);
		if(err)
			goto cleanup;
	}
	return ego;

 cleanup:
	free(ego);
	return NULL;

	(void)opts; /* silence unused parameter warning */
}

void
LUX_RMMOD(void *ego)
{
	cl_int err = clReleaseContext(((Lux_opencl *)ego)->super);
	if(!err)
		free(ego);
}
