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
#include "api.h"
#include <lux/switch.h>
#include <stdlib.h> /* for exit() and EXIT_FAILURE */
#include <string.h> /* for strcmp() */

#define DROP(n) do {	                     \
	int j;                               \
	*pargc -= n;                         \
	for(j = i; j <= *pargc; ++j)         \
		(*pargv)[j] = (*pargv)[j+n]; \
} while(0)

#define FLAG(s) CASE(!strcmp((*pargv)[i], s)) DROP(1);

struct libux *LUX = NULL;

static void
setup(int *pargc, char ***pargv)
{
	int i = 1;

	LUX = mklibux();

	/* Setup the lux environment */
	while(i < *pargc) {
		SWITCH {
		FLAG("--debug")
			LUX->vlog.levels[7] &= ~VLOG_SUSPEND;
		DEFAULT
			++i;
		}
	}
}

static void
cleanup(void)
{
	/* Clean up the lux environment */

	rmlibux(LUX);
}

void
lux_setup(int *pargc, char ***pargv)
{
	if(!atexit(cleanup))
		setup(pargc, pargv);
	else
		lux_abort();
}

void
lux_abort(void)
{
	exit(EXIT_FAILURE);
}
