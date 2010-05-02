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
#ifndef _LUX_PARRAY_H_
#define _LUX_PARRAY_H_
/*
 * There are 6 ways to keep track of the shapes/lengths/counts of
 * arrays that make sense in standard C.  For convenience, we use 1D
 * arrays as example here:
 *
 * 1. Use two variables manually: GOOD if everything is in the same
 *    scope; cannot be returned; BAD to pass as argument
 *
 *	size_t n;
 *	char   a[n];
 *
 * 2. Use pointer for the array: FINE if everything is in the same
 *    scope; cannot be returned; OK to pass as argument
 *
 *	size_t n;
 *	char  *a; // -> char _a[n];
 *
 * 3. struct with flexible array element: same as 1---large struct to
 *    passed (for pointer to this struct, see method 5)
 *
 *	struct {
 *		size_t n;
 *		char   a[n];
 *	} a;
 *
 * 4. struct with pointer: FINE to return result; GOOD to pass
 *    argument
 *
 *	struct {
 *		size_t n;
 *		char  *a; // -> char _a[n];
 *	} a;
 *
 * 5. Pointer to struct with flexible array element: GOOD to return
 *    result, FINE to pass argument
 *
 *	struct {
 *		size_t n;
 *		char   a[n];
 *	} *a;
 *
 * 6. Pointer to struct with pointer: BAD because of double
 *    de-referencing---not much point to use it
 *
 *	struct string {
 *		size_t n;
 *		char  *a; // -> char _a[n];
 *	} *a;
 *
 * Traditionally, C APIs use methods 2, which can only be "returned"
 * by using arguments:
 *
 *	void mkarray(size_t *n_out, char **a_out);
 *
 * and is cumbersome.  In lux, we have used both methods 4 and 5,
 * depending on if passing or returning is more important.
 *
 * For this static module, we introduce yet another method using a
 * "hack".  The array is allocated in memory using method 3.  But
 * instead of referencing is using method 5, we return the reference
 * to the flexible length array.  I.e.,
 *
 * 7. "prefixed-shape" method: GOOD to return result and pass result;
 *    act like normal pointers; slightly slower in order to look up
 *    length
 *
 *	struct {
 *		size_t n;
 *		char   a[n];
 *	} _a; // in memory
 *
 *	char *a = _a.a; // pointer to a[] instead of _a.
 *
 * In fact, because of this "prefixed-shape" method return an ordinary
 * pointer, it can be combined with method 2 and 4, which makes it
 * very flexible.  We call these "prefixed-shape arrays", or
 * "p-array".
 *
 * In principle, we can let pfree() and pgetn() determine the rank in
 * runtime.  Nevertheless, we intend to use <lux/parray.h> even in
 * performance critical inner loops.  Hence, we keep <lux/parray.h>
 * static to ensure it can be optimized at compile time.  The
 * programmer must keep track of the p-array rank at compile time.
 *
 * For dynamic-dimension arrays, the programmer can use <lux/dope.h>
 * and <lux/array.h>.  For static-dimension arrays, the programmer
 * should simply use fixed shape arryas, e.g., `array[16][16][16]`.
 */
#include <lux/assert.h>
#include <lux/aver.h>
#include <lux/dope.h>
#include <stdlib.h> /* for malloc() and free() */

#if !HAVE_TYPEOF
# error typeof() is not available; <lux/parray.h> cannot be used as is
#endif

#define STRUCTOF(T)    struct { size_t dn[1]; T e[1]; }
#define OFFSETOF(T, D) (offsetof(STRUCTOF(T), e) + ((D)-1) * sizeof(size_t))
#define HEADEROF(P, D) (headerof(STRUCTOF(typeof(*P)), P, e)->dn - ((D)-1))

#define palloc(T, ...) ({             \
	size_t _n_[] = {__VA_ARGS__}; \
	size_t _d_   = countof(_n_);  \
	pallocdn(T, _d_, _n_);        \
})

#define pallocdn(T, D, Ns) ({                          \
	size_t _i_, _c_, _o_, *_p_;                    \
	lux_aver(D <= DOPE_D_MAX);                     \
	for(_i_ = 0, _c_ = 1; _i_ < D; ++_i_) {        \
		lux_assert(Ns[_i_] <= DOPE_N_MAX);     \
		_c_ *= Ns[_i_];                        \
	}                                              \
	_o_ = OFFSETOF(T, D);                          \
	_p_ = malloc(_o_ + sizeof(T) * _c_);           \
	if(_p_)                                        \
		for(_i_ = 0; _i_ < D; ++_i_)           \
			_p_[_i_] = PKDN(_i_, Ns[_i_]); \
	(T *)((char *)_p_ + (_p_ ? _o_ : 0));          \
})

#define pfree(P) free(HEADEROF(P, pgetd(P)))

#define pgetd(P) (GETD(HEADEROF(P, 1)[0])+1)

#define pgetn(P, J) ({	           \
	size_t _d_ = pgetd(P);     \
	lux_assert(J < _d_);       \
	GETN(HEADEROF(P, _d_)[J]); \
})

#endif /* _LUX_PARRAY_H_ */
