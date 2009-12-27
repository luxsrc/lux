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
#ifndef _LUX_DLIB_H_
#define _LUX_DLIB_H_

#include <lux/dltry.h>
#include <lux/failed.h>
#include <lux/lazybuf.h>
#include <string.h> /* for memcpy(), strchr(), and strlen() */

static inline void *
mkdlib(const char *restrict paths, const char *restrict name)
{
	void *dlib = NULL;

	char lazybuf[256], *buf = lazybuf;
	const size_t nlen = strlen(name);

	if(name && name[0] == '/') {
		buf = MALLOC(nlen + sizeof(".so"));
		if(!buf)
			return NULL; /* failure code was set by MALLOC() */

		(void)memcpy(buf, name, nlen);
		(void)memcpy(buf + nlen, ".so", sizeof(".so"));

		dlib = dltryopen(buf, RTLD_LAZY | RTLD_LOCAL);
	} else while(paths && !dlib) {
		const char  *psep = strchr(paths, ':');
		const size_t plen = psep ? (size_t)(psep-paths) : strlen(paths);

		void *tmp = REALLOC(buf, plen + sizeof("/.so") + nlen);
		if(tmp)
			buf = (char *)tmp;
		else {
			FREE(buf);   /* as REALLOC() may succeed a few times */
			return NULL; /* failure code was set by REALLOC()    */
		}
		(void)memcpy(buf, paths, plen);
		buf[plen] = '/';
		(void)memcpy(buf + plen + 1, name, nlen);
		(void)memcpy(buf + plen + 1 + nlen, ".so", sizeof(".so"));

		paths = psep ? psep + 1 : NULL;
		dlib  = dltryopen(buf, RTLD_LAZY | RTLD_LOCAL);
	}

	FREE(buf);
	if(!dlib)
		failed = FNOLIB;
	return dlib;
}

static inline void
rmdlib(void *dlib)
{
	if(dlclose(dlib)) {
		(void)dlerror(); /* TODO: log failure message */
		failed = FNOLIB;
	}
}

#endif /* _LUX_DLIB_H_ */
