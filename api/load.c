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
#include <lux/lazybuf.h>
#include <lux/load.h>
#include <unistd.h> /* for getcwd() and getenv() */
#include <string.h> /* for strcat(), strcpy(), and strlen() */

#define COUNT_OF(a) (sizeof(a) / sizeof(a[0]))

static const char *paths[] = {NULL, NULL, LUX_MOD_PATH};

static inline size_t
max(size_t a, size_t b)
{
	return a > b ? a : b;
}

void *
lux_load(const char *restrict name, const void *opts)
{
	size_t i, maxlen;

	char lazybuf[256], *buf;

	void *ins;

	if(!paths[0])
		paths[0] = getcwd(NULL, 0);
	if(!paths[1]) {
		char *home = getenv("HOME");
		char *path = (char *)malloc(strlen(home) +
		                          sizeof("/.lux/lib/lux"));
		paths[1] = strcat(strcpy(path, home), "/.lux/lib/lux");
	}
	/* TODO: free paths[0] and paths[1] in lux-wise cleanup */

	for(i = 1, maxlen = strlen(paths[0]); i < COUNT_OF(paths); ++i)
		maxlen = max(maxlen, strlen(paths[i]));

	/* Try to allocate more memory if name is too long */
	buf = (char *)MALLOC(maxlen + sizeof("/") + strlen(name));
	if(!buf)
		return NULL; /* failure code was set by MALLOC() */

	/* Try to load the module */
	for(i = 0, ins = NULL; i < COUNT_OF(paths) && !ins; ++i) {
		(void)strcat(strcat(strcpy(buf, paths[i]), "/"), name);
		ins = vload(buf, opts);
	}

	FREE(buf);
	return ins;
}

void
lux_unload(void *ins)
{
	uload(ins);
}
