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
#ifndef _LUX_TENSOR_H_
#define _LUX_TENSOR_H_

#include <lux/header.h>
#include <lux/offset.h>
#include <stdlib.h> /* for malloc() and free() */

#if !HAVE_TYPEOF
# error typeof() is not available; <lux/tensor.h> cannot be used as is
#endif

/* Similar to <lux/vector.h>, there are different ways to keep track
   of the shapes of multi-dimensional arrays that makes sense in
   standard C.  For convenience, we call these arrays "tensors" so
   that their dimensions are naturally called "rank", and their shapes
   are naturally called "dimensions".

   We do not list all the possible ways here.  The basic principle is
   the same as <lux/vector.h>, that we need to keep track both a
   header that contains the rank and dimension information, and the
   actual data of the tensor.  */

#define _COUNTOF(...) ({ size_t arr[] = {__VA_ARGS__}; sizeof(arr) / sizeof(size_t); })
#define _TENSOROF(R, T) struct { size_t d[R]; T e[8]; }
#define _PSIZEOFD(R, T) offsetof(_TENSOROF(R, T), e)
#define _HEADEROF(R, P) headerof(_TENSOROF(R, typeof(*P)), P, e)

#define _talloc(dsz, esz, ...) ({                 \
	size_t *p;                                \
	                                          \
	size_t ds[] = {__VA_ARGS__};              \
	size_t r   = sizeof(ds) / sizeof(size_t); \
	size_t c    = 1;                          \
	size_t i;                                 \
	for(i = 0; i < r; ++i)                    \
		c *= ds[i];                       \
	                                          \
	p = malloc((dsz) + (esz) * c);            \
	for(i = 0; i < r; ++i)                    \
		p[i] = ds[i];                     \
	(char *)p + (dsz);                        \
})

#define talloc(T, ...) ((T *)_talloc(_PSIZEOFD(_COUNTOF(__VA_ARGS__), T), sizeof(T), __VA_ARGS__))
#define tfree(R, P)  free(_HEADEROF(R, P))
#define dimsof(R, P) (_HEADEROF(R, P)->d)

#endif /* _LUX_TENSOR_H_ */
