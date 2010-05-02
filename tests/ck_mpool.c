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
#include <lux/mpool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static inline size_t
roundup(size_t sz, size_t psz)
{
	return ((sz + psz - 1) / psz) * psz;
}

int
main(int argc, char *argv[])
{
	size_t psz = getpagesize();
	size_t sz  = argc > 1 ? atoi(argv[1]) : 5000;

	struct mpool mp = mkmpool(sz);

	printf("memory pool size: %zu -> %zu\n", sz, mp.sz);
	lux_assert(mp.sz == roundup(sz, psz));

	rmmpool(mp);
	return 0;
}
