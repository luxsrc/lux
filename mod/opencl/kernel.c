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
#include <lux/switch.h>
#include <stdarg.h>
#include <string.h>
#include "mod.h"

struct kernel {
	Lux_opencl_kernel super;

	size_t integersz;
	size_t fastsz;
	size_t realsz;
	size_t extendedsz;

	size_t bsz;
	size_t bml;
};

typedef union {
	cl_half   h;
	cl_float  f;
	cl_double d;
} cl_real;

#define OCL ((struct opencl *)ocl)
#define EGO ((struct kernel *)ego)

#define TYPE(s) CASE(!strcmp(type, s))

static void
set(Lux_opencl_kernel *ego, size_t i, size_t sz, void *arg)
{
	check(SetKernelArg, ego->krn, i, sz, arg);
}

static void
setM(Lux_opencl_kernel *ego, size_t i, cl_mem m)
{
	check(SetKernelArg, ego->krn, i, sizeof(cl_mem), &m);
}

static void
setS(Lux_opencl_kernel *ego, size_t i, size_t sz)
{
	check(SetKernelArg, ego->krn, i, sz, NULL);
}

static void
setW(Lux_opencl_kernel *ego, size_t i, whole w)
{
	cl_uint clw = w;
	check(SetKernelArg, ego->krn, i, EGO->integersz, &clw);
}

static void
setZ(Lux_opencl_kernel *ego, size_t i, integer z)
{
	cl_int clz = z;
	check(SetKernelArg, ego->krn, i, EGO->integersz, &clz);
}

static void
setR(Lux_opencl_kernel *ego, size_t i, real r)
{
	cl_real clr;
	switch(EGO->realsz) {
	case 2: clr.h = r; break;
	case 4: clr.f = r; break;
	case 8: clr.d = r; break;
	}
	check(SetKernelArg, ego->krn, i, EGO->realsz, &clr);
}

static Lux_opencl_kernel *
with(Lux_opencl_kernel *ego, ...)
{
	va_list ap;
	cl_uint i, argc;

	check(GetKernelInfo,
	      ego->krn, CL_KERNEL_NUM_ARGS,
	      sizeof(cl_uint), &argc, NULL);

	va_start(ap, ego);
	for(i = 0; i < argc; ++i) {
		char type[64];
		check(GetKernelArgInfo,
		      ego->krn, i, CL_KERNEL_ARG_TYPE_NAME,
		      sizeof(type), type, NULL);

		SWITCH {
		TYPE("uint")
			whole w;
			if(sizeof(whole) >= sizeof(unsigned))
				w = va_arg(ap, whole);
			else
				w = va_arg(ap, unsigned);
			setW(ego, i, w);
		TYPE("int")
			integer z;
			if(sizeof(integer) >= sizeof(int))
				z = va_arg(ap, integer);
			else
				z = va_arg(ap, int);
			setZ(ego, i, z);
		TYPE("real")
			real r;
			if(sizeof(real) >= sizeof(double))
				r = va_arg(ap, real);
			else
				r = va_arg(ap, double);
			setR(ego, i, r);
		CASE(type[strlen(type)-1] == '*')
			cl_kernel_arg_address_qualifier aq;
			check(GetKernelArgInfo,
			      ego->krn, i,
			      CL_KERNEL_ARG_ADDRESS_QUALIFIER,
			      sizeof(aq), &aq, NULL);
			if(aq == CL_KERNEL_ARG_ADDRESS_LOCAL) {
				size_t s = va_arg(ap, size_t);
				setS(ego, i, s);
			} else {
				cl_mem p = va_arg(ap, cl_mem);
				setM(ego, i, p);
			}
		DEFAULT
			lux_error("unknown type \"%s\"\n", type);
		}
	}
	va_end(ap);

	return ego;
}

Lux_opencl_kernel *
mkkern(Lux_opencl *ocl, const char *name)
{
	cl_int    err;
	cl_kernel kernel;
	size_t    bsz_max;
	size_t    bml_pref;

	Lux_opencl_kernel *ego;

	kernel = clCreateKernel(OCL->pro, name, &err);
	if(!kernel || err != CL_SUCCESS) {
		lux_error("Failed to obtain compute kernel \"%s\"\n", name);
		exit(1);
	}

	err = clGetKernelWorkGroupInfo(kernel, ocl->dev,
	                               CL_KERNEL_WORK_GROUP_SIZE,
	                               sizeof(size_t), &bsz_max, NULL);
	if(err != CL_SUCCESS) {
		lux_error("Failed to obtain workgroup size for \"%s\"\n", name);
		exit(1);
	}

	err = clGetKernelWorkGroupInfo(kernel, ocl->dev,
	                               CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
	                               sizeof(size_t), &bml_pref, NULL);
	if(err != CL_SUCCESS) {
		lux_error("Failed to obtain preferred multiple for \"%s\"\n", name);
		exit(1);
	}

	ego = malloc(sizeof(struct kernel));
	if(ego) {
		ego->krn = kernel;

		ego->set  = set;
		ego->setS = setS;
		ego->setM = setM;
		ego->setW = setW;
		ego->setZ = setZ;
		ego->setR = setR;
		ego->with = with;

		EGO->integersz  = OCL->integersz;
		EGO->fastsz     = OCL->fastsz;
		EGO->realsz     = OCL->realsz;
		EGO->extendedsz = OCL->extendedsz;
		EGO->bsz        = bsz_max;
		EGO->bml        = bml_pref;
	}
	return ego;
}

double
exec(Lux_opencl *ocl, Lux_opencl_kernel *ego, size_t dim, const size_t *shape)
{
	cl_event event;
	cl_ulong t0, t1;

	/* In OpenCL, the global size must be multipple of local size;
	   hence we round up the last dimension to a multiple of bml */
	size_t bml = EGO->bml;
	size_t shapeup[3];
	size_t i;
	for(i = 0; i < dim-1; ++i)
		shapeup[i] = shape[i];
	shapeup[i] = ((shape[i] + bml - 1) / bml) * bml;

	/* TODO: automatic load balancing across devices */
	check(EnqueueNDRangeKernel,
	      ocl->que, ego->krn,
	      dim, NULL, shapeup, NULL, 0, NULL, &event);
	check(WaitForEvents, 1, &event);

	check(GetEventProfilingInfo,
	      event, CL_PROFILING_COMMAND_START, sizeof(t0), &t0, NULL);
	check(GetEventProfilingInfo,
	      event, CL_PROFILING_COMMAND_END, sizeof(t1), &t1, NULL);
	check(ReleaseEvent, event);

	return (double)(t1 - t0);
}

void
rmkern(Lux_opencl *ocl, Lux_opencl_kernel *ego)
{
	check(ReleaseKernel, ego->krn);
	free(ego);

	(void)ocl; /* silence unused variable warning */
}
