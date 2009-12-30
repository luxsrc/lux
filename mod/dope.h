/*
 * Copyright (C) 2009 Chi-kwan Chan
 * Copyright (C) 2009 Harvard-Smithsonian Center for Astrophysics
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
#ifndef _LUX_DOPE_H_
#define _LUX_DOPE_H_
/*
 * Generalized <lux/tensor.h> that determine rank at runtime
 *
 * FIXME: using pointer arithmetic to determine the offset of the data
 * array from the dope vector, although is self-consistent, may cause
 * incorrect data alignment.  For 64-bit machines, this only happens
 * if the data type of an array starts with `long double`.  In this
 * rare case, misalignment could significantly slow down the algorithm
 * or even produce incorrect results on some hardwares.
 *
 * For now, it is possible to follow <lux/tensor.h> and then use the
 * compile-time assertion macro lux_aver() to stop lux for compiling
 * for situations with incorrect alignment.
 *
 * #define _TENSOROF(T, R) struct { Lux_dope d[R]; T e[8]; }
 * #define _HEADERSZ(T, R) offsetof(_TENSOROF(T, R), e)
 * ...
 * {
 *	...
 * 	lux_aver(_HEADERSZ(long double, 3) == sizeof(Lux_dope[3]));
 * }
 */
#include <lux/numeric.h>
#include <lux/zalloc.h>

typedef struct {
	Lux_int r; /* rank/dimension */
	Lux_int c; /* count/size */
	Lux_int s; /* stride */
} Lux_dope;

static inline Lux_dope *
mkdope(size_t sz, int rank)
{
	Lux_dope *head = (Lux_dope *)zalloc(rank * sizeof(Lux_dope) + sz);
	if(head) {
		Lux_int r;
		for(r = 0; r < rank; ++r)
			head[r].r = r;
		if(sz >= sizeof(Lux_int))
			head[r].r = r; /* r == rank here */
		return head + rank;
	} else
		return NULL;
}

static inline void
rmdope(Lux_dope *dope)
{
	free(dope - dope->r);
}

#endif /* _LUX_DOPE_H_ */
