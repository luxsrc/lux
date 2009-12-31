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
#ifndef _LUX_ARRAY_H_
#define _LUX_ARRAY_H_
/*
 * Generalized <lux/tensor.h> that determines rank at runtime
 */
#include <lux/assert.h>
#include <lux/dope.h>
#include <stddef.h> /* for ptrdiff_t */
#include <stdlib.h> /* for malloc() and free() */

#define _ARRAYOF(T, R) struct { struct dope dd; struct dope d[R]; T e[8]; }
#define _HEADEROF(P, R) headerof(_ARRAYOF(typeof(*P), R), P, e)

#define aalloc(T, R, Ds) ({                                      \
	struct dope *_ptr_;                                      \
	                                                         \
	size_t _hsz_ = offsetof(_ARRAYOF(T, R), e);              \
	size_t _cnt_, _i_;                                       \
	lux_assert(R <= LUX_ARRRNK_MAX);                         \
	for(_i_ = 0, _cnt_ = 1; _i_ < R; ++_i_) {                \
		lux_assert(Ds[_i_] <= LUX_ARRDIM_MAX);           \
		_cnt_ *= Ds[_i_];                                \
	}                                                        \
	                                                         \
	_ptr_ = malloc(_hsz_ + sizeof(T) * _cnt_);               \
	if(_ptr_) {                                              \
		_ptr_[0] = pkdope(sizeof(struct dope), 0, R);    \
		for(_i_ = 0; _i_ < R; ++_i_) {                   \
			_ptr_[_i_+1] = pkdope(sizeof(T) * _cnt_, \
			                      _i_, Ds[_i_]);     \
			_cnt_ /= Ds[_i_];                        \
		}                                                \
	}                                                        \
	(T *)((char *)_ptr_ + (_ptr_ ? _hsz_ : 0));              \
})

#define afree(P)          free(_HEADEROF(P, dope_getr(_HEADEROF(P, 1)->d)+1))
#define getdim(P, J) dope_getd(_HEADEROF(P, dope_getr(_HEADEROF(P, 1)->d)+1)->d + J)

#endif /* _LUX_ARRAY_H_ */
