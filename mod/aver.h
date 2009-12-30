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
#ifndef _LUX_AVER_H_
#define _LUX_AVER_H_
/*
 * Compile-time assertion; hence the more formal term "aver"
 *
 * The expression inside lux_aver() must be evaluated at compile time,
 * otherwise lux_aver() will simply fail.  Because there is no runtime
 * overhead, it does not make sense to disable lux_aver().  It is
 * always on if used.
 */
#ifndef LUX_AVER_FAILURE
#define LUX_AVER_FAILURE 0
#endif

#define lux_aver(E) do {                             \
	char aver[(E) == LUX_AVER_FAILURE ? -1 : 1]; \
	(void)aver;                                  \
} while(0)

#endif /* _LUX_AVER_H_ */
