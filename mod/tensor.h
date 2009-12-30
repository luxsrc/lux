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

#include <lux/assert.h>
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

#define LUX_TENSORDIM_MASK (((size_t)1 << LUX_TENSORDIM_BIT) - 1)
#define LUX_TENSORRANK_MAX ((size_t)1 << (LUX_SIZE_T_BIT - LUX_TENSORDIM_BIT))

#define _TENSOROF(T, R) struct { size_t d[R]; T e[8]; }
#define _HEADERSZ(T, R) offsetof(_TENSOROF(T, R), e)
#define _HEADEROF(P, R) headerof(_TENSOROF(typeof(*P), R), P, e)

#define talloc(T, ...) ({                                        \
	size_t *_ptr_;                                           \
	                                                         \
	size_t _dims_[] = {__VA_ARGS__};                         \
	size_t _rank_   = countof(_dims_);                       \
	size_t _hsz_    = _HEADERSZ(T, countof(_dims_));         \
	size_t _cnt_, _i_;                                       \
	lux_assert(_rank_ <= LUX_TENSORRANK_MAX);                \
	for(_i_ = 0, _cnt_ = 1; _i_ < _rank_; ++_i_) {           \
		lux_assert(_dims_[_i_] <= LUX_TENSORDIM_MASK);   \
		_cnt_ *= _dims_[_i_];                            \
	}                                                        \
	                                                         \
	_ptr_ = malloc(_hsz_ + sizeof(T) * _cnt_);               \
	if(_ptr_)                                                \
		for(_i_ = 0; _i_ < _rank_; ++_i_)                \
			_ptr_[_i_] = _i_ << LUX_TENSORDIM_BIT |  \
			     (_dims_[_i_] & LUX_TENSORDIM_MASK); \
	(T *)((char *)_ptr_ + (_ptr_ ? _hsz_ : 0));              \
})

#define tfree(P, R)     free(_HEADEROF(P, R))
#define getdim(P, R, J) ({	                                            \
	size_t _rank_ = (_HEADEROF(P, R)->d[R-1] >> LUX_TENSORDIM_BIT) + 1; \
	lux_assert(_rank_ == R);                                            \
	_HEADEROF(P, R)->d[J] & LUX_TENSORDIM_MASK;                         \
})

#endif /* _LUX_TENSOR_H_ */
