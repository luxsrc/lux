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
#include <stdlib.h> /* for NULL */
#include "mod.h"

cl_mem
mk(Lux_opencl *ego, size_t sz, unsigned flags)
{
	cl_int err;
	cl_mem buf = clCreateBuffer(ego->ctx, flags, sz, NULL, &err);
	if(err != CL_SUCCESS)
		lux_error("Failed to create buffer \"%s\"\n",
		          strerr(err));
	return buf;
}

void
rm(Lux_opencl *ego, cl_mem buf)
{
	cl_int err;
	err = clReleaseMemObject(buf);
	if(err != CL_SUCCESS)
		lux_error("Failed to release memory object \"%s\"\n",
		          strerr(err));

	(void)ego; /* silence unused variable warning */
}

cl_mem
h2d(Lux_opencl *ego, cl_mem dst, void *src, size_t sz)
{
	cl_int err;
	err = clEnqueueWriteBuffer(ego->que,
	                           dst, CL_TRUE, 0, sz,
	                           src, 0, NULL, NULL);
	if(err != CL_SUCCESS)
		lux_error("Failed to enqueue write buffer \"%s\"\n",
		          strerr(err));
	return dst;
}

void *
d2h(Lux_opencl *ego, void *dst, cl_mem src, size_t sz)
{
	cl_int err;
	err = clEnqueueReadBuffer(ego->que,
	                          src, CL_TRUE, 0, sz,
	                          dst, 0, NULL, NULL);
	if(err != CL_SUCCESS)
		lux_error("Failed to enqueue read buffer \"%s\"\n",
		          strerr(err));
	return dst;
}

void *
mmap(Lux_opencl *ego, cl_mem buf, size_t sz)
{
	cl_int err;
	void *host = clEnqueueMapBuffer(ego->que, buf,
	                               CL_TRUE, CL_MAP_READ, 0, sz,
	                               0, NULL, NULL, &err);
	if(err != CL_SUCCESS)
		lux_error("Failed to enqueue map buffer \"%s\"\n",
		          strerr(err));
	return host;
}

void
munmap(Lux_opencl *ego, cl_mem buf, void *host)
{
	cl_int err;
	err = clEnqueueUnmapMemObject(ego->que, buf, host,
	                              0, NULL, NULL);
	if(err != CL_SUCCESS)
		lux_error("Failed to enqueue unmap buffer \"%s\"\n",
		          strerr(err));
}
