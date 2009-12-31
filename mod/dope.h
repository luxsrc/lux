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
 * Generalized <lux/tensor.h> that determine rank at runtime
 */
#include <lux/assert.h>
#include <stddef.h> /* for ptrdiff_t */
#include <stdlib.h> /* for malloc() and free() */

struct dope {
	size_t    d; /* bitwise-OR of rank and dimension */
	ptrdiff_t s; /* stride is in unit of bytes */
};

#define LUX_TENSORDIM_MAX (((size_t)1 << LUX_TENSORDIM_BIT) - 1)
#define LUX_TENSORRNK_MAX ((size_t)1 << (LUX_SIZE_T_BIT - LUX_TENSORDIM_BIT))

#define _DOPEOF(T, R) struct { struct dope dd; struct dope d[R]; T e[8]; }
#define _HEADEROF(P, R) headerof(_DOPEOF(typeof(*P), R), P, e)

#define dalloc(T, R, Ds) ({                                         \
	struct dope *_ptr_;                                         \
	                                                            \
	size_t _hsz_ = offsetof(_DOPEOF(T, R), e);                  \
	size_t _cnt_, _i_;                                          \
	lux_assert(R <= LUX_TENSORRNK_MAX);                         \
	for(_i_ = 0, _cnt_ = 1; _i_ < R; ++_i_) {                   \
		lux_assert(Ds[_i_] <= LUX_TENSORDIM_MAX);           \
		_cnt_ *= Ds[_i_];                                   \
	}                                                           \
	                                                            \
	_ptr_ = malloc(_hsz_ + sizeof(T) * _cnt_);                  \
	if(_ptr_) {                                                 \
		_ptr_[0].d = R;                                     \
		_ptr_[0].s = sizeof(struct dope);                   \
		for(_i_ = 0; _i_ < R; ++_i_) {                      \
			_ptr_[_i_+1].d = _i_ << LUX_TENSORDIM_BIT | \
			   (Ds[_i_] & LUX_TENSORDIM_MAX);           \
			_ptr_[_i_+1].s = sizeof(T) * _cnt_;         \
			_cnt_ /= Ds[_i_];                           \
		}                                                   \
	}                                                           \
	(T *)((char *)_ptr_ + (_ptr_ ? _hsz_ : 0));                 \
})

#define dfree(P) free(_HEADEROF(P, 1 + (_HEADEROF(P, 1)->d[0].d >> \
                                        LUX_TENSORDIM_BIT)))
#define getdim(P, J) (_HEADEROF(P, 1 + (_HEADEROF(P, 1)->d[0].d >>    \
                                        LUX_TENSORDIM_BIT))->d[J].d & \
                      LUX_TENSORDIM_MAX)

#endif /* _LUX_DOPE_H_ */
