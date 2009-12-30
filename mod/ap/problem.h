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
#ifndef _LUX_PROBLEM_H_
#define _LUX_PROBLEM_H_

/* Forward declaration only: problem specific interface */
typedef struct LuxSproblem Lux_problem;

struct LuxSproblem {
	int: 0; /* a hack for creating empty struct */

	/* Internally, a Lux_problem should contain at least `xstate`
	   and `params`, which are external input states such as array
	   size that affect the performance and input parameters such
	   as time step that do not affect the performance.  For
	   example, for FFT, we may define

	// xstate
	Lux_dope *in;
	Lux_dope *out;

	// params
	Lux_real scale;

	// optional memory addresses
	const Lux_real *restrict ro; // input
	      Lux_real *restrict rw; // buffer
	      Lux_real *restrict wo; // output */
};

#endif /* _LUX_PROBLEM_H_ */
