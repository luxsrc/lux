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

#define EGO ((struct opencl *)ego)
#define KRN ((struct kernel *)krn)

static void
set(Lux_opencl_kernel *krn, size_t i, size_t sz, void *arg)
{
	clSetKernelArg(krn->krn, i, sz, arg);
}

static void
setM(Lux_opencl_kernel *krn, size_t i, cl_mem m)
{
	clSetKernelArg(krn->krn, i, sizeof(cl_mem), &m);
}

static void
setS(Lux_opencl_kernel *krn, size_t i, size_t sz)
{
	clSetKernelArg(krn->krn, i, sz, NULL);
}

static void
setW(Lux_opencl_kernel *krn, size_t i, whole w)
{
	cl_uint clw = w;
	clSetKernelArg(krn->krn, i, KRN->integersz, &clw);
}

static void
setZ(Lux_opencl_kernel *krn, size_t i, integer z)
{
	cl_int clz = z;
	clSetKernelArg(krn->krn, i, KRN->integersz, &clz);
}

static void
setR(Lux_opencl_kernel *krn, size_t i, real r)
{
	cl_real clr;
	switch(KRN->realsz) {
	case 2: clr.h = r; break;
	case 4: clr.f = r; break;
	case 8: clr.d = r; break;
	}
	clSetKernelArg(krn->krn, i, KRN->realsz, &clr);
}

Lux_opencl_kernel *
mkkern(Lux_opencl *ego, const char *name)
{
	cl_int    err;
	cl_kernel kernel;
	size_t    bsz_max;
	size_t    bml_pref;

	Lux_opencl_kernel *krn;

	kernel = clCreateKernel(EGO->pro, name, &err);
	if(!kernel || err != CL_SUCCESS) {
		lux_error("Failed to obtain compute kernel \"%s\"\n", name);
		exit(1);
	}

	err = clGetKernelWorkGroupInfo(kernel, ego->dev,
	                               CL_KERNEL_WORK_GROUP_SIZE,
	                               sizeof(size_t), &bsz_max, NULL);
	if(err != CL_SUCCESS) {
		lux_error("Failed to obtain workgroup size for \"%s\"\n", name);
		exit(1);
	}

	err = clGetKernelWorkGroupInfo(kernel, ego->dev,
	                               CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
	                               sizeof(size_t), &bml_pref, NULL);
	if(err != CL_SUCCESS) {
		lux_error("Failed to obtain preferred multiple for \"%s\"\n", name);
		exit(1);
	}

	krn = malloc(sizeof(struct kernel));
	if(krn) {
		krn->krn = kernel;

		krn->set  = set;
		krn->setS = setS;
		krn->setM = setM;
		krn->setW = setW;
		krn->setZ = setZ;
		krn->setR = setR;

		KRN->integersz  = EGO->integersz;
		KRN->fastsz     = EGO->fastsz;
		KRN->realsz     = EGO->realsz;
		KRN->extendedsz = EGO->extendedsz;
		KRN->bsz        = bsz_max;
		KRN->bml        = bml_pref;
	}
	return krn;
}

double
exec(Lux_opencl *ego, Lux_opencl_kernel *krn, size_t dim, const size_t *shape)
{
	cl_event event;
	cl_ulong t0, t1;

	/* In OpenCL, the global size must be multipple of local size;
	   hence we round up the last dimension to a multiple of bml */
	size_t bml = KRN->bml;
	size_t shapeup[3];
	size_t i;
	for(i = 0; i < dim-1; ++i)
		shapeup[i] = shape[i];
	shapeup[i] = ((shape[i] + bml - 1) / bml) * bml;

	/* TODO: automatic load balancing across devices */
	clEnqueueNDRangeKernel(ego->que, krn->krn,
	                       dim, NULL, shapeup, NULL, 0, NULL, &event);
	clWaitForEvents(1, &event);

	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START,
	                        sizeof(t0), &t0, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END,
	                        sizeof(t1), &t1, NULL);
	clReleaseEvent(event);

	return (double)(t1 - t0);
}

void
rmkern(Lux_opencl *ego, Lux_opencl_kernel *krn)
{
	(void)clReleaseKernel(krn->krn);
	free(krn);
	(void)ego; /* silence unused variable warning */
}
