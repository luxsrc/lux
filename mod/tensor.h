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
 * In principle, we can let tfree() and tgetn() determine the rank in
 * runtime.  Nevertheless, we intend to use <lux/tensor.h> even in
 * performance critical inner loops.  Hence, we keep <lux/tensor.h>
 * static to ensure it can be optimized at compile time.  The
 * programmer must keep track of the tensor rank at compile time.
 *
 * For dynamic rank tensors, the programmer can use <lux/dope.h> and
 * <lux/array.h>.  For static dimension tensors, the programmer should
 * simply use fixed dimension arryas, e.g., `arr[16][16][16]`.
 */
#include <lux/assert.h>
#include <lux/aver.h>
#include <lux/dope.h>
#include <stdlib.h> /* for malloc() and free() */

#if !HAVE_TYPEOF
# error typeof() is not available; <lux/tensor.h> cannot be used as is
#endif

#define TENSOROF(T, D) struct { size_t n[D]; T e[8]; }
#define HEADERSZOF(T, D) offsetof(TENSOROF(T, D), e)
#define HEADEROF(P, D) headerof(TENSOROF(typeof(*P), D), P, e)

#if LUX_ASSERTION
#define pkdn(d, n) PKDN(d, n) /* pack d for runtime assertion */
#else
#define pkdn(d, n) (n) /* for performance critical inner loops */
#endif

#define talloc(T, ...) ({                               \
	size_t *_p_;                                    \
	                                                \
	size_t _n_[] = {__VA_ARGS__};                   \
	size_t _d_   = countof(_n_);                    \
	size_t _hsz_ = HEADERSZOF(T, countof(_n_));     \
	size_t _c_, _i_;                                \
	lux_aver(_d_ <= DOPE_D_MAX);                    \
	for(_i_ = 0, _c_ = 1; _i_ < _d_; ++_i_) {       \
		lux_assert(_n_[_i_] <= DOPE_N_MAX);     \
		_c_ *= _n_[_i_];                        \
	}                                               \
	                                                \
	_p_ = malloc(_hsz_ + sizeof(T) * _c_);          \
	if(_p_)                                         \
		for(_i_ = 0; _i_ < _d_; ++_i_)          \
			_p_[_i_] = pkdn(_i_, _n_[_i_]); \
	(T *)((char *)_p_ + (_p_ ? _hsz_ : 0));         \
})

#define tfree(P, D) free(HEADEROF(P, D))

#define tgetn(P, D, J) ({                                      \
	lux_assert(GETD(HEADEROF(P, D)->n[(D)-1]) + 1 == (D)); \
	GETN(HEADEROF(P, D)->n[J]);                            \
})

#endif /* _LUX_TENSOR_H_ */
