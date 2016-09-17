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
#ifndef _LUX_DARRAY_H_
#define _LUX_DARRAY_H_

#include <lux/dope.h>
#include <stdlib.h> /* for malloc() and free() */

struct darray {
	struct dope *dope;
	void        *data;
};

#define dalloc(T, a, ...) ({          \
	size_t _n_[] = {__VA_ARGS__}; \
	size_t _d_   = countof(_n_);  \
	dallocdn(T, a, _d_, _n_);     \
})

#define dallocdn(T, a, D, Ns) ({                       \
	struct darray darray;                          \
	darray.dope = mkdopedn(sizeof(T), a, D, Ns);   \
	darray.data = malloc(dope_getsz(darray.dope)); \
	darray;                                        \
})

#define dfree(da) do {   \
	rmdope(da.dope); \
	free(da.data);   \
} while(0)

#ifdef _LUX_OPENCL_H_

#define dmk(o, T, ...) ({             \
	size_t _n_[] = {__VA_ARGS__}; \
	size_t _d_   = countof(_n_);  \
	dmkdn(o, T, _d_, _n_);        \
})

#define dmkdn(o, T, D, Ns) ({                           \
	struct darray darray;                           \
	darray.dope = mkdopedn(sizeof(T), 16, D, Ns);   \
	darray.data = o->mk(o, dope_getsz(darray.dope), \
	                      CL_MEM_READ_WRITE);       \
	darray;                                         \
})

#define drm(da) do {       \
	rmdope(da.dope);   \
	o->rm(o, da.data); \
} while(0)

#endif

#define dgetd(da) pgetn(da.dope, 0)

#define dgetn(da, j) dope_getn(da.dope+j)

#endif /* _LUX_DARRAY_H_ */
