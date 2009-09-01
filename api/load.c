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
#include <unistd.h> /* for getcwd() */
#include <string.h> /* for strcat(), strcpy(), and strlen() */
#include <stdarg.h>

#define COUNT_OF(a) (sizeof(a) / sizeof(a[0]))

static const char *paths[] = {NULL, LUX_MOD_PATH};

static inline size_t
max(size_t a, size_t b)
{
	return a > b ? a : b;
}

void *
lux_load(const char *restrict name, ...)
{
	size_t i, maxlen;

	char lazybuf[256], *buf;

	va_list ap;
	void   *obj;

	if(!paths[0])
		paths[0] = getcwd(NULL, 0);

	for(i = 1, maxlen = strlen(paths[0]); i < COUNT_OF(paths); ++i)
		maxlen = max(maxlen, strlen(paths[i]));

	/* Try to allocate more memory if name is too long */
	buf = (char *)MALLOC(maxlen + sizeof("/") + strlen(name));
	if(!buf) {
		lux_error("lux_load(\"%s\"): failed to allocate string\n",
		          name);
		return NULL;
	}

	/* Try to load the module */
	va_start(ap, name);
	for(i = 0, obj = NULL; i < COUNT_OF(paths) && !obj; ++i) {
		(void)strcat(strcat(strcpy(buf, paths[i]), "/"), name);
		obj = vload(buf, ap);
	}
	va_end(ap);

	FREE(buf);
	return obj;
}

void
lux_unload(void *obj)
{
	uload(obj);
}
