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
#include <lux/pvector.h>

#define A(E) lux_always_assert(E)

int
main(int argc, char *argv[])
{
	double *a;
	size_t  i;

	lux_setup(&argc, &argv);

	a = pvector(double, 30, 31, 32, 33, 34, 35);
	A(a && pgetd(a) == 1 && pgetn(a, 0) == 6);
	for(i = 0; i < pgetn(a, 0); ++i)
		A(a[i] == i + 30);
	pfree(a);

	return 0;
}
