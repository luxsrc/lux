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
	char     c;

	lux_setup(&argc, &argv);

	c = typecodeof(i);
	lux_print("int      -> '%c'\n", c);
	A(c == 'i');

	c = typecodeof(u);
	lux_print("unsigned -> '%c'\n", c);
	A(c == 'u');

	c = typecodeof(f);
	lux_print("float    -> '%c'\n", c);
	A(c == 'f');

	c = typecodeof(d);
	lux_print("double   -> '%c'\n", c);
	A(c == 'f');

	return 0;
}
