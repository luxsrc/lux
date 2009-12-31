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

#if HAVE_STDDEF_H
#include <stddef.h> /* for size_t and ptrdiff_t */
#else
#include <stdlib.h> /* for size_t */
typedef long long ptrdiff_t;
#endif

#define DOPE_DIM_MAX (((size_t)1<<LUX_DIMENSION_BIT) - 1)
#define DOPE_RNK_MAX ((size_t)1 << (LUX_SIZE_T_BIT-LUX_DIMENSION_BIT))

struct dope {
	ptrdiff_t s;  /* stride is in unit of bytes */
	size_t    rd; /* bitwise-OR of rank and dimension */
};

static inline struct dope
pkdope(ptrdiff_t stride_in_bytes, size_t rnk, size_t dim)
{
	struct dope d = {stride_in_bytes, (rnk << LUX_DIMENSION_BIT) | dim};
	return d;
}

static inline ptrdiff_t
dope_getsib(struct dope *d) {
	return d->s;
}

static inline size_t
dope_getrnk(struct dope *d) {
	return d->rd >> LUX_DIMENSION_BIT;
}

static inline size_t
dope_getdim(struct dope *d) {
	return d->rd & DOPE_DIM_MAX;
}

#endif /* _LUX_DOPE_H_ */
