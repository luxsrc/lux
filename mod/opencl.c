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

static const char *
getsrc(const char *name)
{
	size_t sz;
	char  *src;

	char   buf[1024];
	FILE  *f = NULL;

	if(!f) {
		sprintf(buf, "%s", name);
		f = fopen(buf, "r");
	}
	if(!f) {
		sprintf(buf, "%s.cl", name);
		f = fopen(buf, "r");
	}
	if(!f) {
		sprintf(buf, LUX_MOD_PATH "/%s", name);
		f = fopen(buf, "r");
	}
	if(!f) {
		sprintf(buf, LUX_MOD_PATH "/%s.cl", name);
		f = fopen(buf, "r");
	}
	if(!f)
		return NULL;
	else
		lux_print("Loaded kernel \"%s\"\n", buf);

	fseek(f, 0, SEEK_END);
	sz = ftell(f);
	fseek(f, 0, SEEK_SET);

	src = malloc(sz+1);
	fread(src, sz, 1, f);
	src[sz] = '\0';
	fclose(f);
	return src;
}

static cl_kernel *
mkkern(cl_context context, cl_device_id device,
       const char **src,
       const char **name,
       const char  *flags)
{
	cl_int     err;
	cl_uint    n, i;
	cl_program program;
	cl_kernel *kernels;

	for(n = 0; src[n]; ++n) {
		if(strlen(src[n]) < 64) {/* UGLY HACK */
			const char *s = getsrc(src[n]);
			if(!s) {
				lux_error("Failed to load source\n");
				exit(1);
			}
			src[n] = s;
		}
	}

	program = clCreateProgramWithSource(context, n, src, NULL, &err);
	if(!program) {
		lux_error("Failed to create program\n");
		exit(1);
	}
	err = clBuildProgram(program, 1, &device, flags, NULL, NULL);
	if(err != CL_SUCCESS) {
		size_t len;
		char   buf[10240];
		lux_error("Failed to build program\n");
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
		                      sizeof(buf), buf, &len);
		lux_error("%s\n", buf);
		exit(1);
	}

	n = 0;
	while(name[n])
		++n;

	kernels = (cl_kernel *)malloc(sizeof(cl_kernel) * (n+1));
	for(i = 0; i < n; ++i) {
		kernels[i] = clCreateKernel(program, name[i], &err);
		if (!kernels[i] || err != CL_SUCCESS) {
			lux_error("Failed to create compute kernel \"%s\"\n",
			          name[i]);
			exit(1);
		}
	}
	kernels[i] = NULL;

	lux_print("%u kernel%s created:\n", n, n > 1 ? "s" : "");
	for(i = 0; i < n; ++i)
		lux_print("\t%u. \"%s\"\n", i, name[i]);

	return kernels;
}

void *
LUX_MKMOD(const struct LuxOopencl *opts)
{
	Lux_opencl *ego;

	struct LuxOopencl def = OPENCL_NULL;
	if(!opts)
		opts = &def;

	lux_print("\nGetting OpenCL platforms... ");
	lsplf();
	lux_print("\nGetting OpenCL devices from platform %u... ", opts->iplf);
	lsdev(opts->iplf);
	lux_print("\n");

	ego = (Lux_opencl *)malloc(sizeof(Lux_opencl));
	if(ego) {
		cl_int err;
		ego->super  = clCreateContextFromType
			(NULL, opts->devtype, NULL, NULL, &err);
		ego->mkkern = mkkern;
		if(err)
			goto cleanup;
		else {
			cl_device_id d[COUNT_MAX];
			size_t i, sz;
			err = clGetContextInfo
				(ego->super, CL_CONTEXT_DEVICES,
				 sizeof(d), d, &sz);
			if(err)
				goto cleanup;
			sz /= sizeof(cl_device_id);
			lux_print("OpenCL context %p contains device%s %u",
			          ego->super,
			          sz > 1 ? "s" : "",
			          (unsigned)d[0]);
			for(i = 1; i < sz; ++i)
				lux_print(", %u", (unsigned)d[i]);
			lux_print("\n");
		}
	}
	return ego;

 cleanup:
	free(ego);
	return NULL;
}

void
LUX_RMMOD(void *ego)
{
	cl_int err = clReleaseContext(((Lux_opencl *)ego)->super);
	if(!err)
		free(ego);
}
