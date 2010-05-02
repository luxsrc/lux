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

#define DOPE_N_MAX (((size_t)1 << LUX_N_BIT) - 1)
#define DOPE_D_MAX ((size_t)1 << (LUX_SIZE_T_BIT - LUX_N_BIT))

struct dope {
	ptrdiff_t s;  /* stride is in unit of bytes */
	size_t    dn; /* bitwise-OR of dim and number of elements */
};

static inline size_t
pkdn(size_t d, size_t n)
{
	return (d << LUX_N_BIT) | n;
}

static inline struct dope
pkdope(ptrdiff_t stride_in_bytes, size_t d, size_t n)
{
	struct dope _ = {stride_in_bytes, pkdn(d, n)};
	return _;
}

static inline ptrdiff_t
dope_gets(struct dope *d)
{
	return d->s;
}

static inline size_t
dope_getd(struct dope *d)
{
	return d->dn >> LUX_N_BIT;
}

static inline size_t
dope_getn(struct dope *d)
{
	return d->dn & DOPE_N_MAX;
}

#endif /* _LUX_DOPE_H_ */
