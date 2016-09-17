/*
 * Copyright (C) 2010 Chi-kwan Chan
 * Copyright (C) 2010 Harvard-Smithsonian Center for Astrophysics
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
#include <lux.h>
#include <lux/assert.h>
#include <lux/dope.h>
#include <lux/parray.h>
#include <stdio.h>

#define A(E) lux_always_assert(E)

#define ALIGN 256
#define N1 5
#define N2 7
#define N3 11

int
main(int argc, char *argv[])
{
	struct dope *dp;

	lux_setup(&argc, &argv);

	dp = mkdope(double [5], ALIGN, N1, N2, N3);

	lux_print("dim(dope): %zu\n", pgetd(dp));
	A(pgetd(dp) == 1);

	lux_print("dim: %zu\n", pgetn(dp, 0));
	A(pgetn(dp, 0) == 3);

	lux_print("shape: %zu %zu %zu\n",
	          dope_getn(dp+0), dope_getn(dp+1), dope_getn(dp+2));
	A(dope_getn(dp+0) == N1);
	A(dope_getn(dp+1) == N2);
	A(dope_getn(dp+2) == N3);

	lux_print("strides: %zu %zu %zu\n",
	          dope_gets(dp+0), dope_gets(dp+1), dope_gets(dp+2));
	A(dope_gets(dp+0) == ALIGN * 2 * N2);
	A(dope_gets(dp+1) == ALIGN * 2);
	A(dope_gets(dp+2) == sizeof(double [5]));

	lux_print("size: %zu\n",
	          dope_getsz(dp));
	A(dope_getsz(dp) == ALIGN * 2 * N2 * N1);

	rmdope(dp);
	return 0;
}
