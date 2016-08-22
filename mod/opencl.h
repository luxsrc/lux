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

#define PLF_COUNT 8  /* 3 bits */
#define DEV_COUNT 32 /* 5 bits */

typedef struct LuxSopencl Lux_opencl;

struct LuxSopencl {
	int       (*lsplf)(Lux_opencl *);
	int       (*lsdev)(Lux_opencl *, unsigned);

	cl_kernel (*mkkern)(Lux_opencl *, const char *);
	void      (*rmkern)(Lux_opencl *, cl_kernel);

	cl_mem    (*mk)(Lux_opencl *, unsigned, size_t);
	void      (*rm)(Lux_opencl *, cl_mem);
};

struct LuxOopencl {
	unsigned       iplf;
	cl_device_type devtype;
	const char    *flags;
	const char   **src;
};

#define OPENCL_NULL {0, CL_DEVICE_TYPE_ALL, NULL, NULL}

#endif /* _LUX_OPENCL_H_ */
