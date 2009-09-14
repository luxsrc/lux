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
#include <lux.h>
#include <lux/timer.h>
#include <stdlib.h> /* for atexit() */

#if HAVE_TIMESTAMP
static timestamp t0;
#endif

static void
setup(void)
{
#if HAVE_TIMESTAMP
	t0 = gettimestamp();
#endif

	/* Setup the lux environment */
}

static void
cleanup(void)
{
	/* Cleanup the lux environment */

#if HAVE_TIMESTAMP
	lux_debug("lux ran for %g sec.\n", elapsed_since(t0));
#endif
}

void
lux_setup(void)
{
	if(!atexit(cleanup))
		setup();
	else
		lux_abort();
}
