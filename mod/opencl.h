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
#include <lux/switch.h>

typedef struct LuxSopencl Lux_opencl;

struct LuxSopencl {
	cl_context super;
	cl_program program;

	cl_platform_id (*lsplf)(Lux_opencl *, unsigned);
	int            (*lsdev)(Lux_opencl *, unsigned, unsigned, cl_device_type);

	cl_kernel (*mkkern)(Lux_opencl *, const char *);
	void      (*rmkern)(Lux_opencl *, cl_kernel);

	cl_mem (*mk)(Lux_opencl *, unsigned, size_t);
	void   (*rm)(Lux_opencl *, cl_mem);

	void  *(*mmap  )(Lux_opencl *, cl_mem, size_t);
	void   (*munmap)(Lux_opencl *, cl_mem, void *);

	void      (*set )(Lux_opencl *, cl_kernel, size_t, size_t, void *);
	void      (*setM)(Lux_opencl *, cl_kernel, size_t, cl_mem);
	void      (*setW)(Lux_opencl *, cl_kernel, size_t, whole);
	void      (*setZ)(Lux_opencl *, cl_kernel, size_t, integer);
	void      (*setR)(Lux_opencl *, cl_kernel, size_t, real);
	double    (*exec)(Lux_opencl *, cl_kernel, size_t, const size_t *, const size_t *);

	cl_command_queue que; /* default queue on default device */

	size_t integersz;
	size_t fastsz;
	size_t realsz;
	size_t extendedsz;

	size_t nqueue;
	cl_command_queue queue[1]; /* flexible array element */
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
	SWITCH {
	CASE(str[0] == 'c') return CL_DEVICE_TYPE_CPU;
	CASE(str[0] == 'g') return CL_DEVICE_TYPE_GPU;
	CASE(str[0] == 'a') return CL_DEVICE_TYPE_ACCELERATOR;
	CASE(str[0] == 'o') return CL_DEVICE_TYPE_CUSTOM;
	DEFAULT             return CL_DEVICE_TYPE_DEFAULT;
	}
}

static inline size_t
strtoprec(const char *str)
{
	SWITCH {
	CASE(str[0] == 's') return sizeof(float);
	CASE(str[0] == 'f') return sizeof(float);
	CASE(str[0] == 'd') return sizeof(double);
	CASE(str[0] == 'l') return sizeof(long double);
	DEFAULT             return 0U;
	}
}

#endif /* _LUX_OPENCL_H_ */
