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
lsdev(unsigned ip)
{
	cl_platform_id p[COUNT_MAX];
	cl_device_id   d[COUNT_MAX];
	cl_uint        n, i;

	(void)clGetPlatformIDs(COUNT_MAX, p, NULL);
	(void)clGetDeviceIDs(p[ip], CL_DEVICE_TYPE_ALL, COUNT_MAX, d, &n);

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

void *
LUX_MKMOD(const void *opts)
{
	Lux_opencl *ego;

	lux_print("\nGetting OpenCL platforms... ");
	lsplf();
	lux_print("\nGetting OpenCL devices... ");
	lsdev(0);
	lux_print("\n");

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
