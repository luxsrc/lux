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
#ifndef _LUX_NUMERIC_H_
#define _LUX_NUMERIC_H_

#if HAVE_STDDEF_H
#include <stddef.h> /* for size_t and ptrdiff_t */

typedef size_t    Lux_whole;
typedef ptrdiff_t Lux_int;
#else
typedef unsigned  Lux_whole;
typedef int       Lux_int;
#endif

#ifdef LUX_SINGLE
typedef float  Lux_real;
#else
typedef double Lux_real;
#endif

typedef double Lux_xreal; /* real numbers with possibly extra precision */

typedef struct {Lux_int   n, m;} Lux_rational;
typedef struct {Lux_real  r, i;} Lux_complex;
typedef struct {Lux_xreal r, i;} Lux_xcomplex;

#endif /* _LUX_NUMERIC_H_ */
