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
/*
 * Similar to <lux/vector.h>, there are different ways to keep track
 * of the shapes of multi-dimensional arrays that makes sense in
 * standard C.  For convenience, we call these arrays "tensors" so
 * that their dimensions are naturally called "rank", and their shapes
 * are naturally called "dimensions".
 *
 * We do not list all the possible ways here.  The basic principle is
 * the same as <lux/vector.h>, that we need to keep track both a
 * header that contains the rank and dimension information, and the
 * actual data of the tensor.
 *
 * In principle, we can let tfree() and getdim() determine the rank in
 * runtime.  Nevertheless, we intend to use <lux/tensor.h> even in
 * performance critical inner loops.  Hence, we keep <lux/tensor.h>
 * static to ensure it can be optimized at compile time.  The
 * programmer must keep track of the tensor rank at compile time.
 *
 * For dynamic rank tensors, the programmer can use <lux/dope.h>.  For
 * static dimension tensors, the programmer should simply use fixed
 * dimension arryas, e.g., `arr[16][16][16]`.
 */
#include <lux/assert.h>
#include <stdlib.h> /* for malloc() and free() */

#if !HAVE_TYPEOF
# error typeof() is not available; <lux/tensor.h> cannot be used as is
#endif

#define _TENSOROF(T, R) struct { size_t d[R]; T e[8]; }
#define _HEADERSZ(T, R) offsetof(_TENSOROF(T, R), e)
#define _HEADEROF(P, R) headerof(_TENSOROF(typeof(*P), R), P, e)

#if LUX_ASSERTION

#define LUX_TENSORDIM_MAX (((size_t)1 << LUX_TENSORDIM_BIT) - 1)
#define LUX_TENSORRNK_MAX ((size_t)1 << (LUX_SIZE_T_BIT - LUX_TENSORDIM_BIT))

#define talloc(T, ...) ({                                       \
	size_t *_ptr_;                                          \
	                                                        \
	size_t _dims_[] = {__VA_ARGS__};                        \
	size_t _rnk_    = countof(_dims_);                      \
	size_t _hsz_    = _HEADERSZ(T, countof(_dims_));        \
	size_t _cnt_, _i_;                                      \
	lux_assert(_rnk_ <= LUX_TENSORRNK_MAX);                 \
	for(_i_ = 0, _cnt_ = 1; _i_ < _rnk_; ++_i_) {           \
		lux_assert(_dims_[_i_] <= LUX_TENSORDIM_MAX);   \
		_cnt_ *= _dims_[_i_];                           \
	}                                                       \
	                                                        \
	_ptr_ = malloc(_hsz_ + sizeof(T) * _cnt_);              \
	if(_ptr_)                                               \
		for(_i_ = 0; _i_ < _rnk_; ++_i_)                \
			_ptr_[_i_] = _i_ << LUX_TENSORDIM_BIT | \
			     (_dims_[_i_] & LUX_TENSORDIM_MAX); \
	(T *)((char *)_ptr_ + (_ptr_ ? _hsz_ : 0));             \
})
#define tfree(P, R)     free(_HEADEROF(P, R))
#define getdim(P, R, J) ({	                                           \
	size_t _rnk_ = (_HEADEROF(P, R)->d[R-1] >> LUX_TENSORDIM_BIT) + 1; \
	lux_assert(_rnk_ == R);                                            \
	_HEADEROF(P, R)->d[J] & LUX_TENSORDIM_MAX;                         \
})

#else

#define talloc(T, ...) ({                                \
	size_t *_ptr_;                                   \
	                                                 \
	size_t _dims_[] = {__VA_ARGS__};                 \
	size_t _rnk_    = countof(_dims_);               \
	size_t _hsz_    = _HEADERSZ(T, countof(_dims_)); \
	size_t _cnt_, _i_;                               \
	for(_i_ = 0, _cnt_ = 1; _i_ < _rnk_; ++_i_)      \
		_cnt_ *= _dims_[_i_];                    \
	                                                 \
	_ptr_ = malloc(_hsz_ + sizeof(T) * _cnt_);       \
	if(_ptr_)                                        \
		for(_i_ = 0; _i_ < _rnk_; ++_i_)         \
			_ptr_[_i_] = _dims_[_i_];        \
	(T *)((char *)_ptr_ + (_ptr_ ? _hsz_ : 0));      \
})
#define tfree(P, R)     free(_HEADEROF(P, R))
#define getdim(P, R, J) (_HEADEROF(P, R)->d[J])

#endif

#endif /* _LUX_TENSOR_H_ */
