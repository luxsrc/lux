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
#include <string.h> /* for strcat(), strcpy(), and strlen() */
#include <stdarg.h>

static const char *path = LUX_MOD_PATH;

void *
lux_load(const char *restrict name, ...)
{
	char lazybuf[256], *buf;

	va_list ap;
	void   *obj;

	/* Try to allocate more memory if name is too long */
	buf = (char *)MALLOC(strlen(path) + sizeof("/") + strlen(name));
	if(!buf) {
		lux_error("lux_load(\"%s\"): failed to allocate string\n",
		          name);
		return NULL;
	}
	(void)strcat(strcat(strcpy(buf, path), "/"), name);

	/* Try to load the module */
	va_start(ap, name);
	obj = vload(buf, ap);
	va_end(ap);

	FREE(buf);
	return obj;
}

void
lux_unload(void *obj)
{
	uload(obj);
}
