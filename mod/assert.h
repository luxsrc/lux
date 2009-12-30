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
#ifndef _LUX_ASSERT_H_
#define _LUX_ASSERT_H_
/*
 * Run-time assertion similar to C's standard assert() macro
 *
 * lux_assert() is only enabled if LUX_ASSERTION is defined, which is
 * only defined if "--enable-assertion" is passed at configure time.
 * When assertion is disabled, the expreission inside lux_assert() is
 * not seen by compiler and hence has no runtime overhead.
 */
#ifndef LUX_ASSERT_FAILURE
#define LUX_ASSERT_FAILURE 0
#endif

#if LUX_ASSERTION
#define lux_assert(E) do {	                                              \
	if((E) == LUX_ASSERT_FAILURE)                                         \
		lux_fatal("lux_assert(" #E ") failed on line %d in \"%s\"\n", \
		          __LINE__, __FILE__);                                \
} while(0)
#else
#define lux_assert(E) /* do nothing */
#endif

#endif /* _LUX_ASSERT_H_ */
