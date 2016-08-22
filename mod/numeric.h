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
/*
 * In C, the different floating point types, i.e., float, double, and
 * long double, almost always have fixed size.  They are 32-bit,
 * 64-bit, and 80-bit, respecitvely.  The different integer types,
 * i.e., short, int, and long, have lengths that are platform
 * dependent.
 *
 * In OpenCL, both floating point and integer types have fixed size.
 * They are:
 *
 *  API Type	Kernel	Size
 *  cl_char	char	8-bit
 *  cl_short	short	16-bit
 *  cl_int	int	32-bit
 *  cl_long	long	64-bit
 *
 *  cl_half	half	16-bit
 *  cl_float	float	32-bit
 *  cl_double	double	64-bit (optional)
 *
 */

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
