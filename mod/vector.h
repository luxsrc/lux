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
#ifndef _LUX_VECTOR_H_
#define _LUX_VECTOR_H_
/*
 * There are 6 ways to keep track of the shapes/lengths/counts of
 * arrays that make sense in standard C.  For convenience, we use 1D
 * arrays as example here:
 *
 * 1. Use two variables manually: GOOD if everything is in the same
 *    scope; cannot be returned; BAD to pass as argument
 *
 *	size_t n;
 *	char   v[n];
 *
 * 2. Use pointer for the vector: FINE if everything is in the same
 *    scope; cannot be returned; OK to pass as argument
 *
 *	size_t n;
 *	char  *v; // -> char _v[n];
 *
 * 3. struct with flexible array element: same as 1---large struct to
 *    passed (for pointer to this struct, see method 5)
 *
 *	struct {
 *		size_t n;
 *		char   v[n];
 *	} v;
 *
 * 4. struct with pointer: FINE to return result; GOOD to pass
 *    argument
 *
 *	struct {
 *		size_t n;
 *		char  *v; // -> char _v[n];
 *	} v;
 *
 * 5. Pointer to struct with flexible array element: GOOD to return
 *    result, FINE to pass argument
 *
 *	struct {
 *		size_t n;
 *		char   v[n];
 *	} *v;
 *
 * 6. Pointer to struct with pointer: BAD because of double
 *    de-referencing---not much point to use it
 *
 *	struct string {
 *		size_t n;
 *		char  *v; // -> char _v[n];
 *	} *v;
 *
 * Traditionally, C APIs use methods 2, which can only be "returned"
 * by using arguments:
 *
 *	void mkvect(size_t *n_out, char **v_out);
 *
 * and is cumbersome.  In lux, we have used both methods 4 and 5,
 * depending on if passing or returning is more important.
 *
 * For this static module, we introduce yet another method using a
 * "hack".  The vector is allocated in memory using method 3.  But
 * instead of referencing is using method 5, we return the reference
 * to the flexible length array.  I.e.,
 *
 * 7. "headerof" method: GOOD to return result and pass result; act
 *     like normal pointers; slightly slower in order to look up
 *     length
 *
 *	struct {
 *		size_t n;
 *		char   v[n];
 *	} _v; // in memory
 *
 *	char *v = _v.v; // pointer to v[] instead of _v.
 *
 * In fact, because of the "headerof" method return ordinary a
 * pointer, it can be combined with method 2 and 4, which makes it
 * very flexible.
 *
 * This static module and <lux/tensor.h> provide macros that let
 * dynamically allocating vectors (and tensors) using method 7 more
 * streamlined.
 */
#include <lux/tensor.h>

#define valloc(T, N) talloc(T, N)
#define vfree(P)     tfree(P, 1)
#define vdimof(P)    tgetdim(P, 1, 0)

#endif /* _LUX_VECTOR_H_ */
