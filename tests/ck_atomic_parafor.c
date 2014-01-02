/*
 * Copyright (C) 2010,2014 Chi-kwan Chan
 * Copyright (C) 2010 Harvard-Smithsonian Center for Astrophysics
 * Copyright (C) 2014 Steward Observatory
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
#include <lux/atomic.h>
#include <lux/assert.h>
#include <lux/parafor.h>
#include <stdio.h>

void
func(size_t n, size_t i, void *data)
{
	size_t j;
	for(j = 0; j < 100000; ++j)
		atomic_add(data, i);
	(void)n; /* silence unused variable warning */
}

int
main()
{
	int i;
	for(i = 0; i < 32; ++i) {
		atomic_t atomic = ATOMIC_NULL;
		parafor(16, 15, func, (void *)&atomic);
		lux_assert(atomic_get(&atomic) == 12000000);
	}
	return 0;
}
