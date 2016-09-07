/*
 * Copyright (C) 2010 Chi-kwan Chan
 * Copyright (C) 2010 Harvard-Smithsonian Center for Astrophysics
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
#include <lux/assert.h>
#include <lux/opencl.h>
#include <unistd.h>

#define A(E) lux_always_assert(E)

int
main(int argc, char *argv[])
{
	Lux_opencl *ocl1, *ocl2;

	cl_context  ctx;
	cl_uint     ndev, count;

	struct LuxOopencl opts  = OPENCL_NULL;
	const  char      *src[] = {"dummy", NULL};
	opts.base = main; /* this function to pin location */
	opts.src  = src;

	lux_setup(&argc, &argv);

	ocl1 = lux_load("../mod/opencl/.libs/opencl", &opts);
	A(ocl1);

	ctx = ocl1->ctx;
	A(ctx);

	clGetContextInfo(ctx, CL_CONTEXT_REFERENCE_COUNT,
	                 sizeof(count), &count, NULL);
	lux_print("refcnt(%p) == %u\n", ctx, (unsigned)count);
	ndev = ocl1->nque;
	A(count == 2 + ndev);

	opts.nque = ocl1->nque;
	opts.que  = ocl1->que;

	ocl2 = lux_load("../mod/opencl/.libs/opencl", &opts);
	A(ocl2);
	A(ocl1->ctx == ocl2->ctx);

	clGetContextInfo(ctx, CL_CONTEXT_REFERENCE_COUNT,
	                 sizeof(count), &count, NULL);
	lux_print("refcnt(%p) == %u\n", ctx, (unsigned)count);
	A(count == 4 + ndev);

	lux_unload(ocl2);
	clGetContextInfo(ctx, CL_CONTEXT_REFERENCE_COUNT,
	                 sizeof(count), &count, NULL);
	lux_print("refcnt(%p) == %u\n", ctx, (unsigned)count);
	A(count == 2 + ndev);

	lux_unload(ocl1);

	return 0;
}
