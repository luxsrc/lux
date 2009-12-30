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

#define countof(A) (sizeof(A) / sizeof(A[0]))
#define lengthof(...) ({ size_t _arr_[] = {__VA_ARGS__}; countof(_arr_); })

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

#define _TENSOROF(T, R) struct { size_t d[R]; T e[8]; }
#define _HEADERSZ(T, R) offsetof(_TENSOROF(T, R), e)
#define _HEADEROF(P, R) headerof(_TENSOROF(typeof(*P), R), P, e)

#define talloc(T, ...) ({                                \
	size_t *_ptr_;                                   \
	                                                 \
	size_t _dims_[] = {__VA_ARGS__};                 \
	size_t _rank_   = countof(_dims_);               \
	size_t _hsz_    = _HEADERSZ(T, countof(_dims_)); \
	size_t _cnt_, _i_;                               \
	for(_i_ = 0, _cnt_ = 1; _i_ < _rank_; ++_i_)     \
		_cnt_ *= _dims_[_i_];                    \
	                                                 \
	_ptr_ = malloc(_hsz_ + sizeof(T) * _cnt_);       \
	for(_i_ = 0; _i_ < _rank_; ++_i_)                \
		_ptr_[_i_] = _dims_[_i_];                \
	(T *)((char *)_ptr_ + _hsz_);                    \
})

#define tfree(P, R)  free(_HEADEROF(P, R))
#define dimsof(P, R) (_HEADEROF(P, R)->d)

#endif /* _LUX_TENSOR_H_ */
