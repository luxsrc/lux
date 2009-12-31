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
 * Generalized <lux/tensor.h> to determines rank at runtime
 */
#include <stddef.h> /* for ptrdiff_t */

struct dope {
	size_t    rd; /* bitwise-OR of rank and dimension */
	ptrdiff_t s;  /* stride is in unit of bytes */
};

#define LUX_ARRDIM_MAX (((size_t)1<<LUX_ARRDIM_BIT) - 1)
#define LUX_ARRRNK_MAX ((size_t)1 << (LUX_SIZE_T_BIT-LUX_ARRDIM_BIT))

static inline struct dope
pkdope(size_t rank, size_t dim, ptrdiff_t stride)
{
	struct dope d = {(rank << LUX_ARRDIM_BIT) |
	                 (dim  &  LUX_ARRDIM_MAX), stride};
	return d;
}

static inline size_t
dope_getr(struct dope *d) {
	return d->rd >> LUX_ARRDIM_BIT;
}

static inline size_t
dope_getd(struct dope *d) {
	return d->rd & LUX_ARRDIM_MAX;
}

static inline ptrdiff_t
dope_gets(struct dope *d) {
	return d->s;
}

#endif /* _LUX_DOPE_H_ */
