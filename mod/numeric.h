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
typedef size_t    whole;
typedef ptrdiff_t integer;
#else
typedef unsigned  whole;
typedef int       integer;
#endif

#ifdef LUX_SINGLE
typedef float  fast; /* "fast" real number */
typedef float  real;
#else
typedef double fast; /* "fast" real number */
typedef double real;
#endif
typedef double extended; /* real numbers with possibly extra precision */

typedef struct {integer  n, m;} rational;

typedef struct {fast     r, i;} fcomplex;
typedef struct {real     r, i;}  complex;
typedef struct {extended r, i;} xcomplex;

#endif /* _LUX_NUMERIC_H_ */
