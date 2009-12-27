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
#include <lux/modman.h>
#include <string.h> /* for memcpy() */

void *
lux_load(const char *restrict name, const void *opts)
{
	void *ins = NULL;

	char lazybuf[256], *buf = lazybuf;
	const size_t nlen  = strlen(name);
	const char  *paths = libux.paths;

	while(paths && !ins) {
		const char  *psep = strchr(paths, ':');
		const size_t plen = psep ? (size_t)(psep-paths) : strlen(paths);

		void *tmp = REALLOC(buf, plen + sizeof("/") + nlen);
		if(tmp)
			buf = (char *)tmp;
		else {
			FREE(buf);   /* as REALLOC() may succeed a few times */
			return NULL; /* failure code was set by REALLOC()    */
		}

		(void)memcpy(buf, paths, plen);
		buf[plen] = '/';
		(void)memcpy(buf + plen + 1, name, nlen);
		buf[plen + 1 + nlen] = '\0';

		/* Try to load the module */
		ins = vmkmod(&libux.ltab, buf, opts);

		paths = psep ? psep + 1 : NULL;
	}

	FREE(buf);
	return ins;
}

void
lux_unload(void *ins)
{
	rmmod(&libux.ltab, ins);
}
