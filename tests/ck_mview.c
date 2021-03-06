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
#include <lux/mview.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define A(E) lux_always_assert(E)

#define ALIGN 256
#define N1 5
#define N2 7
#define N3 11

int
main()
{
	char *v1, *v2;

	struct mpool *mp = mkmpool(20000);
	struct dope  *dp = mkdope(double [5], ALIGN, N1, N2, N3);

	v1 = mkmview(mp, 0, dp);
	v2 = mkmview(mp, 0, dp);

	printf("%p %p\n", v1, v2);
	A(v1 != v2);

	(void)strcpy(v1, "testing");
	printf("v1: \"%s\"\n", v1);
	printf("v2: \"%s\"\n", v2);
	A(!strcmp(v1, v2));

	rmmview(mp, v1);
	rmmview(mp, v2);

	rmmpool(mp);
	rmdope(dp);

	return 0;
}
