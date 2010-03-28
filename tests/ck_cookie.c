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
#ifndef LUX_ASSERTION
#define LUX_ASSERTION 1
#endif

#include <lux.h>
#include <lux/assert.h>
#include <lux/cookie.h>
#include <stdio.h>

static char cookie[1024];

static ssize_t
dummy(void *c, const char *buf, size_t sz)
{
	return 0;
}

int
main()
{
	cookie_io_functions_t funcs = {0};
	FILE *f;

	funcs.read = dummy;
	f = fopencookie(cookie, "w+", funcs);
	lux_assert(f == NULL);

	funcs.write = dummy;
	f = fopencookie(cookie, "w+", funcs);
	lux_assert(f != NULL);

	lux_assert(fclose(f) == 0);

	return 0;
}
