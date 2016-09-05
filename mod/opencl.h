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

/* OpenCL context, device, program, kernel, queue, etc
 *
 * The OpenCL framework provides a flexible abstraction to manage
 * resource and computing tasks on heterogeneous platforms.  While
 * this lux OpenCL module should simplify the usage of OpenCL, it
 * should be designed in a way to allow all the flexibility.
 *
 * An OpenCL context encapsulates computing resource (CPUs, GPUs) and
 * memory.  It allows the computing resource to share memory.
 * Multiple command queues and programs can be associated with a
 * context.  While each program can be compiled on multiple devices,
 * each queue can only be associated with one device.  Conceptually,
 *
 * +----------------------------------------------------------------+
 * |                           Context 0                            |
 * | +---------------------------+ +------------------------------+ |
 * | | program 1                 | | program 2                    | |
 * | +---------------------------+ +------------------------------+ |
 * |           +--------------------------------------------------+ |
 * |           | program 3                                        | |
 * |           +--------------------------------------------------+ |
 * |                               :                                |
 * |                               :                                |
 * |                               :                                |
 * |                     +-----------------+                        |
 * |                     | program p       |                        |
 * |                     +-----------------+                        |
 * | +-------+ +-------+ +-------+ +-------+              +-------+ |
 * | |       | |       | |       | |       |              |       | |
 * | | que 0 | | que 1 | | que 2 | | que 3 |     ....     | que n | |
 * | |       | |       | |       | |       |              |       | |
 * | +-------+ +-------+ +-------+ +-------+              +-------+ |
 * | |       | |       | |       | |       |              |       | |
 * | | dev 0 | | dev 1 | | dev 2 | | dev 3 |     ....     | dev n | |
 * | |       | |       | |       | |       |              |       | |
 * | +-------+ +-------+ +-------+ +-------+              +-------+ |
 * | +------------------------------------------------------------+ |
 * | | shared memory 0                                            | |
 * | +------------------------------------------------------------+ |
 * | +------------------------------------------------------------+ |
 * | | shared memory 1                                            | |
 * | +------------------------------------------------------------+ |
 * | +------------------------------------------------------------+ |
 * | | shared memory 2                                            | |
 * | +------------------------------------------------------------+ |
 * |                               :                                |
 * |                               :                                |
 * |                               :                                |
 * | +------------------------------------------------------------+ |
 * | | shared memory m                                            | |
 * | +------------------------------------------------------------+ |
 * +----------------------------------------------------------------+
 *
 * It is natural to map an OpenCL program to a lux module.  This means
 * that multiple modules may share the same context with multiple
 * queues and devices.  Because a device can be determined from a
 * queue, we should provide options to share context and queues during
 * lux OpenCL module creation.
 */

/* Forward declaration */
typedef struct LuxSopencl        Lux_opencl;
typedef struct LuxSopencl_kernel Lux_opencl_kernel;

/* The options should setup a program */
struct LuxOopencl {
	/* Specify the OpenCL source codes */
	const void  *base;
	const char **src;

	/* How the program should be built */
	size_t      realsz;
	const char *flags;

	/* How to setup the context and queues:
	   0 == no reuse, specified by i-values,
	   1 == reuse by queues,
	   2 == specified by device */
	int reuse;
	union {
		struct {
			unsigned       iplf;
			unsigned       idev;
			cl_device_type devtype;
		} spec;
		struct {
			cl_context    ctx;
			size_t        ndev;
			cl_device_id *dev;
		} dev;
		struct {
			cl_context        ctx;
			size_t            nque;
			cl_command_queue *que;
		} que;
	} settings;
};

#define OPENCL_NULL {NULL, NULL, sizeof(float), NULL, 0, {{0, 0, CL_DEVICE_TYPE_ALL}}}

struct LuxSopencl {
	/* Make OpenCL context and queues public so they can be shared */
	cl_context        ctx;
	size_t            nque;
	cl_command_queue *que;

	/* Memory related */
	cl_mem (*mk    )(Lux_opencl *, size_t, unsigned);
	void   (*rm    )(Lux_opencl *, cl_mem);
	cl_mem (*h2d   )(Lux_opencl *, cl_mem, void *, size_t);
	void  *(*d2h   )(Lux_opencl *, void *, cl_mem, size_t);
	void  *(*mmap  )(Lux_opencl *, cl_mem, size_t);
	void   (*munmap)(Lux_opencl *, cl_mem, void *);

	/* Compute related */
	Lux_opencl_kernel *(*mkkern)(Lux_opencl *, const char *);
	double             (*exec  )(Lux_opencl *, Lux_opencl_kernel *, size_t, const size_t *);
	void               (*rmkern)(Lux_opencl *, Lux_opencl_kernel *);
};

struct LuxSopencl_kernel {
	cl_kernel krn;

	void (*set )(Lux_opencl_kernel *, size_t, size_t, void *);
	void (*setM)(Lux_opencl_kernel *, size_t, cl_mem);
	void (*setS)(Lux_opencl_kernel *, size_t, size_t);
	void (*setW)(Lux_opencl_kernel *, size_t, whole);
	void (*setZ)(Lux_opencl_kernel *, size_t, integer);
	void (*setR)(Lux_opencl_kernel *, size_t, real);

	Lux_opencl_kernel *(*with)(Lux_opencl_kernel *, ...);
};

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
