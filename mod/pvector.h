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
#ifndef _LUX_PVECTOR_H_
#define _LUX_PVECTOR_H_

#include <lux/parray.h>

#define pvector(T, ...) ({                  \
	T      _a_[] = {__VA_ARGS__};       \
	size_t _n_[] = {countof(_a_)};      \
	T     *_     = pallocdn(T, 1, _n_); \
	size_t  _i_;                        \
	for(_i_ = 0; _i_ < _n_[0]; ++_i_)   \
		_[_i_] = _a_[_i_];          \
	_;                                  \
})

#endif /* _LUX_PVECTOR_H_ */
