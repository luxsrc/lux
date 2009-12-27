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
#include <lux/lazybuf.h>
#include <lux/load.h>
#include <string.h> /* for strcat(), strcpy(), and strlen() */

#define COUNT_OF(a) (sizeof(a) / sizeof(a[0]))

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

	for(i=1, maxlen=strlen(libux.paths[0]); i < COUNT_OF(libux.paths); ++i)
		maxlen = max(maxlen, strlen(libux.paths[i]));

	/* Try to allocate more memory if name is too long */
	buf = (char *)MALLOC(maxlen + sizeof("/") + strlen(name));
	if(!buf)
		return NULL; /* failure code was set by MALLOC() */

	/* Try to load the module */
	for(i = 0, ins = NULL; i < COUNT_OF(libux.paths) && !ins; ++i) {
		(void)strcat(strcat(strcpy(buf, libux.paths[i]), "/"), name);
		ins = vload(&libux.ltab, buf, opts);
	}

	FREE(buf);
	return ins;
}

void
lux_unload(void *ins)
{
	uload(&libux.ltab, ins);
}
