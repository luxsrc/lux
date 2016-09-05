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
	return safe(cl_mem,
	            CreateBuffer, ego->ctx, flags, sz, NULL);
}

void
rm(Lux_opencl *ego, cl_mem buf)
{
	check(ReleaseMemObject, buf);

	(void)ego; /* silence unused variable warning */
}

cl_mem
h2d(Lux_opencl *ego, cl_mem dst, void *src, size_t sz)
{
	check(EnqueueWriteBuffer, ego->que[0],
	      dst, CL_TRUE, 0, sz,
	      src, 0, NULL, NULL);
	return dst;
}

void *
d2h(Lux_opencl *ego, void *dst, cl_mem src, size_t sz)
{
	check(EnqueueReadBuffer, ego->que[0],
	      src, CL_TRUE, 0, sz,
	      dst, 0, NULL, NULL);
	return dst;
}

void *
mmap(Lux_opencl *ego, cl_mem buf, size_t sz)
{
	return safe(void *,
	            EnqueueMapBuffer, ego->que[0], buf,
	            CL_TRUE, CL_MAP_READ, 0, sz, 0, NULL, NULL);
}

void
munmap(Lux_opencl *ego, cl_mem buf, void *host)
{
	check(EnqueueUnmapMemObject,
	      ego->que[0], buf, host, 0, NULL, NULL);
}
