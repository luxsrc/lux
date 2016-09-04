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

#define PKDN(d, n) (((d) << LUX_N_BIT) | (n))
#define GETD(dn)   ((dn) >> LUX_N_BIT)
#define GETN(dn)   ((dn) & DOPE_N_MAX)

#include <lux/parray.h> /* must be before mkdopedn() and rmdope() so
                           that the palloc() and pfree() macros are
                           defined.  */

struct dope {
	ptrdiff_t s;  /* stride is in unit of bytes */
	size_t    dn; /* bitwise-OR of dim and number of elements */
};

static inline struct dope
pkdope(ptrdiff_t stride_in_bytes, size_t d, size_t n)
{
	struct dope _ = {stride_in_bytes, PKDN(d, n)};
	return _;
}

static inline ptrdiff_t
dope_gets(struct dope *dp)
{
	return dp->s;
}

static inline size_t
dope_getd(struct dope *dp)
{
	return GETD(dp->dn);
}

static inline size_t
dope_getn(struct dope *dp)
{
	return GETN(dp->dn);
}

#define mkdope(T, a, ...) ({              \
	size_t _n_[] = {__VA_ARGS__};     \
	size_t _d_   = countof(_n_);      \
	mkdopedn(sizeof(T), a, _d_, _n_); \
})

static inline struct dope *
mkdopedn(size_t esz, size_t a, size_t d, size_t *n)
{
	struct dope *dp = palloc(struct dope, d);
 	size_t s, i;
	for(s = esz, i = d-1; i < d /* works because size_t */; s *= n[i--]) {
		if(i+1 == d-1)
			s = ((s + a - 1) / a) * a;
		dp[i] = pkdope(s, i, n[i]);
	}
	return dp;
}

static inline void
rmdope(struct dope *dp)
{
	pfree(dp);
}

#endif /* _LUX_DOPE_H_ */
