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
#ifndef _LUX_OPENCL_H_
#define _LUX_OPENCL_H_

#if SUPPORT_FRAMEWORK
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

#include <lux/numeric.h>

typedef struct LuxSopencl        Lux_opencl;
typedef struct LuxSopencl_kernel Lux_opencl_kernel;

struct LuxSopencl {
	cl_platform_id (*lsplf)(Lux_opencl *, unsigned);
	int            (*lsdev)(Lux_opencl *, unsigned, unsigned, cl_device_type);

	Lux_opencl_kernel *(*mkkern)(Lux_opencl *, const char *);
	void               (*rmkern)(Lux_opencl *, Lux_opencl_kernel *);

	cl_mem (*mk    )(Lux_opencl *, size_t, unsigned);
	void   (*rm    )(Lux_opencl *, cl_mem);
	void  *(*mmap  )(Lux_opencl *, cl_mem, size_t);
	void   (*munmap)(Lux_opencl *, cl_mem, void *);

	void   (*set   )(Lux_opencl *, Lux_opencl_kernel *, size_t, size_t, void *);
	void   (*setM  )(Lux_opencl *, Lux_opencl_kernel *, size_t, cl_mem);
	void   (*setW  )(Lux_opencl *, Lux_opencl_kernel *, size_t, whole);
	void   (*setZ  )(Lux_opencl *, Lux_opencl_kernel *, size_t, integer);
	void   (*setR  )(Lux_opencl *, Lux_opencl_kernel *, size_t, real);
	double (*exec  )(Lux_opencl *, Lux_opencl_kernel *, size_t, const size_t *);

	cl_device_id     dev; /* default device */
	cl_command_queue que; /* default queue on default device */
};

struct LuxOopencl {
	void *base;

	unsigned iplf;
	unsigned idev;
	cl_device_type devtype;
	size_t         realsz;

	const char  *flags;
	const char **src;
};

#define OPENCL_NULL {NULL, 0, 0, CL_DEVICE_TYPE_ALL, sizeof(float), NULL, NULL}

static inline cl_device_type
strtotype(const char *str)
{
	switch(str[0]) {
	case 'c': return CL_DEVICE_TYPE_CPU;
	case 'g': return CL_DEVICE_TYPE_GPU;
	case 'a': return CL_DEVICE_TYPE_ACCELERATOR;
#ifdef CL_DEVICE_TYPE_CUSTOM /* OpenCL version >= 1.2 */
	case 'u': return CL_DEVICE_TYPE_CUSTOM;
#endif
	case 'd': return CL_DEVICE_TYPE_DEFAULT;
	default : return CL_DEVICE_TYPE_ALL;
	}
}

static inline size_t
strtoprec(const char *str)
{
	switch(str[0]) {
	case 's': return sizeof(float);
	case 'f': return sizeof(float);
	case 'd': return sizeof(double);
	case 'l': return sizeof(long double);
	default : return 0U;
	}
}

static inline const char *
prectostr(size_t prec)
{
	switch(prec) {
	case 2 : return "half"  ;
	case 4 : return "float" ;
	case 8 : return "double";
	default: return "float" ;
	}
}

#endif /* _LUX_OPENCL_H_ */
