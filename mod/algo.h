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
#ifndef _LUX_ALGO_H_
#define _LUX_ALGO_H_

/* Forward declaration only: problem specific interface */
typedef struct LuxSalgo Lux_algo;

struct LuxSalgo {
	int: 0; /* a hack for creating empty struct */

	/* Internally, a Lux_algo should contain everything, expect
	   the actual pointer to memory, for performing an algorithm.
	   These include the `driver`, `ispec`, and `xspec`, for a
	   driver function or the actual algorithm, internal derived
	   spec, and external input spec, respectively.  For
	   example, for FFT, we may define

	// driver
	int (*apply)(Lux_algo *, ...);

	// ispec
	Lux_mpi *mpi;

	// xspec
	Lux_dope *in;
	Lux_dope *out; */
};

#endif /* _LUX_ALGO_H_ */
