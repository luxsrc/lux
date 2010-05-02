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

static inline size_t
roundup(size_t n, size_t m)
{
	return ((n + m - 1) / m) * m;
}

#define DOPESZOF(D) ((1+(D))*sizeof(struct dope))
#define HEADERSZOF(T, D) roundup(DOPESZOF(D), alignof(T))
#define HEADEROF(P, D) ((struct dope *)((char *)(P)-HEADERSZOF(typeof(*P), D)))
#define DIMOF(P) (dope_getd(HEADEROF(P, 1)+1)+1)

#define aalloc(T, D, Ns) ({                                                 \
	struct dope *_p_;                                                   \
	                                                                    \
	size_t _hsz_ = HEADERSZOF(T, D);                                    \
	size_t _c_, _i_;                                                    \
	lux_assert(D <= DOPE_D_MAX);                                        \
	for(_i_ = 0, _c_ = 1; _i_ < D; ++_i_) {                             \
		lux_assert(Ns[_i_] <= DOPE_N_MAX);                          \
		_c_ *= Ns[_i_];                                             \
	}                                                                   \
	                                                                    \
	_p_ = malloc(_hsz_ + sizeof(T) * _c_);                              \
	if(_p_) {                                                           \
		_p_[0] = pkdope(sizeof(struct dope), 0, D);                 \
		for(_i_ = 0; _i_ < D; ++_i_) {                              \
			_p_[_i_+1] = pkdope(sizeof(T) * _c_, _i_, Ns[_i_]); \
			_c_ /= Ns[_i_];                                     \
		}                                                           \
	}                                                                   \
	(T *)((char *)_p_ + (_p_ ? _hsz_ : 0));                             \
})

#define afree(P)         free(HEADEROF(P, DIMOF(P)))
#define agetn(P, J) dope_getn(HEADEROF(P, DIMOF(P))+1+J)

#endif /* _LUX_ARRAY_H_ */
