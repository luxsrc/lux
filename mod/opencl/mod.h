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
#ifndef _MOD_H_
#define _MOD_H_

#include <lux/mangle.h>
#include <stdio.h> /* for sprintf() etc */
#include "../opencl.h"

#define PLF_COUNT 8  /* 3 bits */
#define DEV_COUNT 32 /* 5 bits */

#define getinfo(f, l, ...) do {	                          \
	size_t need;                                      \
l:                                                        \
	if(clGet##f##Info(__VA_ARGS__, sz, buf, &need)) { \
		sz  = need;                               \
		buf = lzrealloc(buf, sz);                 \
		goto l;                                   \
	}                                                 \
} while(0)

#define check(f, ...) do {                                      \
	cl_int err;                                             \
	err = cl##f(__VA_ARGS__);                               \
	if(err != CL_SUCCESS)                                   \
		lux_error(LUX_STRING(cl##f) " failed \"%s\"\n", \
		          strerr(err));                         \
} while(0)

#define safe(T, f, ...) ({                                      \
	cl_int err;                                             \
	T r = cl##f(__VA_ARGS__, &err);                         \
	if(err != CL_SUCCESS)                                   \
		lux_error(LUX_STRING(cl##f) " failed \"%s\"\n", \
		          strerr(err));                         \
	r;                                                      \
})

struct opencl {
	Lux_opencl super;

	size_t integersz;
	size_t fastsz;
	size_t realsz;
	size_t extendedsz;

	cl_program       pro;
	size_t           nqueue;
	cl_command_queue queue[1]; /* flexible array element */
};

extern void *LUX_MKMOD(const struct LuxOopencl *);

extern const char *strerr(cl_int);

extern cl_platform_id lsplf(Lux_opencl *, unsigned);
extern int            lsdev(Lux_opencl *, unsigned, unsigned, cl_device_type);

extern const char *getsrc(const char *, const char *);

extern cl_program mkpro(struct opencl *, const struct LuxOopencl *, size_t, cl_device_id *);

extern Lux_opencl_kernel *mkkern(Lux_opencl *, const char *);
extern double             exec  (Lux_opencl *, Lux_opencl_kernel *,
                                 size_t, const size_t *);
extern void               rmkern(Lux_opencl *, Lux_opencl_kernel *);

extern cl_mem   mk(Lux_opencl *, size_t, unsigned);
extern void     rm(Lux_opencl *, cl_mem);
extern cl_mem  h2d(Lux_opencl *, cl_mem, void *, size_t);
extern void   *d2h(Lux_opencl *, void *, cl_mem, size_t);
extern void  *mmap(Lux_opencl *, cl_mem, size_t);
extern void munmap(Lux_opencl *, cl_mem, void *);

#endif /* _MOD_H_ */
