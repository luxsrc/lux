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
 * In order to be portable and to support a wide range of hardware at
 * the same time, C primitive types can be classified into two
 * categories to accommodate two different meanings of "portability".
 *
 * In one category, the types are platform independent.  When a
 * developer use a type in this category, the behaviors of the type is
 * expected to be the same across platform.  If a platform does not
 * have a particular feature, the type is simply missing.  Floating
 * point types in C fall in this category.  `float` and `double` are
 * 32-bit and 64-bit on almost all platforms, respectively.  `half`
 * and `long double`, if available, are 16-bit and (often) 80-bit,
 * although they may simply be missing if the platform do not support
 * them.  "Portable" here means that, if a developer write a code once
 * and the code compiles, he or she knows how it would behave the same
 * across platform (assuming round off error is irrelevant).  However,
 * the developer is responsible to check if a type is available in the
 * first place, and to use additional checks (mostly with macros) to
 * make sure that the code would compile.
 *
 * In the other category, the types are known that they can be
 * different on different platforms.  Nevertheless, the same types
 * always exist across platform so the code can be written once and
 * compile everyone.  Integer types such as `int` and `long` fall into
 * this category.
 *
 * In OpenCL, both floating point and integer types have fixed size
 * across platform and `cl_double` is optional.  The details are
 * listed here:
 *
 *  C Type       OpenCL Type  C Size    OpenCL Size
 *
 *  char         cl_char      ~  8-bit  =  8-bit
 *  short        cl_short     >=16-bit  = 16-bit
 *  int          cl_int       >=16-bit  = 32-bit
 *  long         cl_long      >=32-bit  = 64-bit
 *  long long    ---          >=64-bit  ---
 *
 *  ---          cl_half      = 16-bit  = 16-bit
 *  float        cl_float     = 32-bit  = 32-bit
 *  double       cl_double    = 64-bit  = 64-bit
 *  long double  ---          ~ 80-bit  ---
 *
 * While this helps OpenCL kernels to behave consistently once they
 * are compiled, it limits the number of devices that the same kernel
 * can compile on.
 *
 * To overcome this, lux follow the second C category above to define
 * classes of `integer` and `real` types that garanteed to exist, but
 * may be different on different platform.  In fact, lux OpenCL (see
 * <lux/opencl.h> goes all the way to make these types redefinable at
 * *runtime*.  lux can then measure the performance of these different
 * redefinition and provide the most sensible configuration to the
 * user.
 *
 * As a summary, lux provides
 *
 *               "Host"    "OpenCL Device", adjustable
 *            /----^----\  /------------^------------\
 *  Type      CPU          GPU-HPC   GPU      Graphics
 *
 *  integer   ptrdiff_t    int       cl_int   cl_int
 *  whole     size_t       unsigned  cl_uint  cl_uint
 *
 *  fast      float        float     float    half
 *  real      double       double    float    float
 *  extended  long double  double    double   float
 */
#if ! HAVE_STDDEF_H
#error <stddef.h> is required to define `whole` and `integer` on the host.
#endif

#include <stddef.h> /* for size_t and ptrdiff_t */

typedef size_t    whole;
typedef ptrdiff_t integer;

typedef float fast; /* "fast" real number */
#ifdef LUX_SINGLE
typedef float real;
typedef double extended; /* real numbers with possibly extra precision */
#else
typedef double real;
typedef long double extended; /* real numbers with possibly extra precision */
#endif

typedef struct {integer  n, m;} rational;

typedef struct {fast     r, i;} fcomplex;
typedef struct {real     r, i;}  complex;
typedef struct {extended r, i;} xcomplex;

#endif /* _LUX_NUMERIC_H_ */
