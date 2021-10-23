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
#include <lux/parray.h>

#define A(E) lux_always_assert(E)

int
main(int argc, char *argv[])
{
	struct xyz {
		float x, y, z;
	} *a;

	size_t n[] = {10, 11, 12, 13, 14, 15};
	size_t i;

	lux_setup(&argc, &argv);

	a = pallocdn(struct xyz, 6, n);
	A(a && pgetd(a) == 6);
	for(i = 0; i < pgetd(a); ++i)
		A(pgetn(a, i) == n[i]);
	pfree(a);

	a = palloc(struct xyz, 20, 21, 22, 23, 24, 25);
	A(a && pgetd(a) == 6);
	for(i = 0; i < pgetd(a); ++i)
		A(pgetn(a, i) == n[i] + 10);
	pfree(a);

	return 0;
}