/*
 * Copyright (C) 2016 Chi-kwan Chan
 * Copyright (C) 2016 Steward Observatory
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

#define A(E) lux_always_assert(E)

int
main(int argc, char *argv[])
{
	int      i;
	unsigned u;
	float    f;
	double   d;
	int      tc;

	lux_setup(&argc, &argv);

	tc = typecodeof(i);
	lux_print("int      -> '%c' %d\n", tc, tc >> 8);
	A(tc == TYPECODE('i', sizeof(i) * 8));

	tc = typecodeof(u);
	lux_print("unsigned -> '%c' %d\n", tc, tc >> 8);
	A(tc == TYPECODE('u', sizeof(u) * 8));

	tc = typecodeof(f);
	lux_print("float    -> '%c' %d\n", tc, tc >> 8);
	A(tc == TYPECODE('f', sizeof(f) * 8));

	tc = typecodeof(d);
	lux_print("double   -> '%c' %d\n", tc, tc >> 8);
	A(tc == TYPECODE('f', sizeof(d) * 8));

	return 0;
}
