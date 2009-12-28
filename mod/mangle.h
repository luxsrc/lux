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
#ifndef _LUX_MANGLE_H_
#define _LUX_MANGLE_H_

/* Macros for token transformation */
#define LUX_STR1NG(s) #s
#define LUX_STRING(s) LUX_STR1NG(s)
#define LUX_C0NCAT(s, t) s ## t
#define LUX_CONCAT(s, t) LUX_C0NCAT(s, t)

/* Name mangling by passing a cpp-flag "-DLUX_MOD_NAME=name" */
#define LUXCMOD LUX_CONCAT(luxC, LUX_MOD_NAME) /* constructor */
#define LUXDMOD LUX_CONCAT(luxD, LUX_MOD_NAME) /* destructor  */
#define LUXEMOD LUX_CONCAT(luxE, LUX_MOD_NAME) /* entry-point */

#endif /* _LUX_MANGLE_H_ */
