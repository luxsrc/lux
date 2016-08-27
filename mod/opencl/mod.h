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

#include "../opencl.h"

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

extern Lux_opencl_kernel *mkkern(Lux_opencl *, const char *);
extern double             exec  (Lux_opencl *, Lux_opencl_kernel *, size_t, const size_t *);
extern void               rmkern(Lux_opencl *, Lux_opencl_kernel *);

#endif /* _MOD_H_ */
