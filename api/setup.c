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
#include <unistd.h> /* for getcwd() and getenv() */
#include <string.h> /* for strcat(), strcpy(), strdup(), and strlen() */
#include <stdlib.h> /* for atexit() */

#define COUNT_OF(a) (sizeof(a) / sizeof(a[0]))

struct libux libux = LIBUX_NULL;

static void
setup(void)
{
	char *home = getenv("HOME");
	char *path = (char *)malloc(strlen(home) + sizeof("/.lux/lib/lux"));

	libux.paths[0] = getcwd(NULL, 0);
	libux.paths[1] = strcat(strcpy(path, home), "/.lux/lib/lux");
	libux.paths[2] = strdup(LUX_MOD_PATH);

#if HAVE_TIMESTAMP
	libux.t0 = gettimestamp();
#endif

	/* Setup the lux environment */
}

static void
cleanup(void)
{
	/* Cleanup the lux environment */

	size_t i;
	for(i = 0; i < COUNT_OF(libux.paths); ++i)
		free((void *)libux.paths[i]);

#if HAVE_TIMESTAMP
	lux_debug("lux ran for %g sec.\n", elapsed_since(libux.t0));
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
