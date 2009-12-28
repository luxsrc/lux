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
#ifndef _LUX_LIBUX_H_
#define _LUX_LIBUX_H_

#include <lux/ap/timer.h>
#include <lux/dm/load.h>
#include <lux/vlog.h>

#include <unistd.h> /* for getcwd() and getenv() */
#include <string.h> /* for strlen()              */
#include <stdio.h>  /* for sprintf()             */
#include <stdlib.h> /* for malloc() and free()   */

#if HAVE_TIMESTAMP
#define LIBUX_NULL {TIMESTAMP_NULL, VLOG_NULL, LOAD_NULL}
#else
#define LIBUX_NULL {VLOG_NULL, LOAD_NULL}
#endif

struct libux {
#if HAVE_TIMESTAMP
	timestamp t0;
#endif
	struct vlog vlog;
	struct load load;
};

static inline const char *
getpaths(void)
{
	const char fmt[] = "%s:%s/.lux/lib/lux:" LUX_MOD_PATH;

	char *cwd   = getcwd(NULL, 0);
	char *home  = getenv("HOME");
	char *paths = (char *)malloc(strlen(cwd)  +
	                             strlen(home) +
	                             sizeof(fmt)  - 4);
	(void)sprintf(paths, fmt, cwd, home);

	free(cwd);
	/* no need to free home */
	return paths;
}

static inline struct libux *
mklibux(void)
{
	static struct libux libux = LIBUX_NULL;

#if HAVE_TIMESTAMP
	libux.t0 = gettimestamp();
#endif
	libux.load.paths = getpaths();

	return &libux;
}

static inline void
rmlibux(struct libux *lux)
{
	free((void *)lux->load.paths);

#if HAVE_TIMESTAMP
	lux_debug("lux ran for %g sec.\n", elapsed_since(lux->t0));
#endif
}

#endif /* _LUX_LIBUX_H_ */
