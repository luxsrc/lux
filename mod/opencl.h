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

typedef struct LuxSopencl Lux_opencl;

struct LuxSopencl {
	cl_context super;
	cl_program program;

	cl_kernel (*mkkern)(Lux_opencl *, const char *);
	void      (*rmkern)(cl_kernel);

	cl_mem (*mk)(cl_context, unsigned, size_t);
	void   (*rm)(cl_mem);

	double (*exec)(Lux_opencl *, cl_kernel,
	               size_t, const size_t *, const size_t *);

	cl_command_queue que; /* default queue on default device */

	size_t nqueue;
	cl_command_queue queue[1]; /* flexible array element */
};

struct LuxOopencl {
	unsigned iplf;
	unsigned idev;
	cl_device_type devtype;

	const char  *flags;
	const char **src;
};

#define OPENCL_NULL {0, 0, CL_DEVICE_TYPE_DEFAULT, NULL, NULL}

#endif /* _LUX_OPENCL_H_ */
