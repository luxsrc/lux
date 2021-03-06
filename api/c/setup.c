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
#include <stdlib.h> /* for exit() and EXIT_FAILURE */

struct libux *LUX = NULL;

static void
setup(void)
{
	LUX = mklibux();

	/* Setup the lux environment */
}

static void
cleanup(void)
{
	/* Clean up the lux environment */

	rmlibux(LUX);
}

void
lux_setup(int *argc, char ***argv)
{
	if(!atexit(cleanup))
		setup();
	else
		lux_abort();

	(void)argc; /* silence unused variable warning */
	(void)argv; /* silence unused variable warning */
}

void
lux_abort(void)
{
	exit(EXIT_FAILURE);
}
