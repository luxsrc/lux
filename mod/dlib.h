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
#if HAVE_STDDEF_H
#include <stddef.h> /* for NULL and size_t */
#else
#include <stdlib.h> /* for NULL and size_t */
#endif
#include <string.h> /* for strlen(), memcpy(), and strchr() */

#define DLIB_NULL {NULL}

struct dlib {
	void *hdl; /* handle */
};

static inline struct dlib
mkdlib(Lmid_t namespace,
       const char *restrict paths, const char *restrict name, int mode)
{
	struct dlib l = DLIB_NULL;

	char lazybuf[256], *buf = lazybuf;
	const size_t nlen = name ? strlen(name) : 0;

	if(!name) {
		/* If name is NULL, then the returned handle is for
		   the main program */

		l.hdl = dlmopen(namespace, NULL, mode);
	} else if(name[0] == '/') {
		/* If `name` starts with '/', assume it corresponds to
		   an absolute path and ignore variable `paths` */

		buf = lzmalloc(nlen + sizeof(".so"));
		if(!buf)
			return l; /* == DLIB_NULL; no need to lzfree(buf);
			             failure code was set by lzmalloc() */
		(void)memcpy(buf, name, nlen);
		(void)memcpy(buf + nlen, ".so", sizeof(".so"));

		l.hdl = dltryopen(namespace, buf, mode);
	} else while(paths && !l.hdl) {
		/* If `name` does not start with '/', use paths within
		   variable `paths` as path-prefixes */

		const char  *psep = strchr(paths, ':');
		const size_t plen = psep ? (size_t)(psep-paths) : strlen(paths);

		void *tmp = lzrealloc(buf, plen + sizeof("/.so") + nlen);
		if(tmp)
			buf = (char *)tmp;
		else {
			lzfree(buf); /* lzrealloc() may succeed a few times */
			return l;    /* == DLIB_NULL;
			                failure code was set by lzrealloc() */
		}
		(void)memcpy(buf, paths, plen);
		buf[plen] = '/';
		(void)memcpy(buf + plen + 1, name, nlen);
		(void)memcpy(buf + plen + 1 + nlen, ".so", sizeof(".so"));

		paths = psep ? psep + 1 : NULL;
		l.hdl = dltryopen(namespace, buf, mode);
	}

	lzfree(buf);
	if(!l.hdl)
		failed = FNOLIB;
	return l;
}

static inline void
rmdlib(struct dlib l)
{
	if(dlclose(l.hdl)) {
		(void)dlerror(); /* TODO: log failure message */
		failed = FNOLIB;
	}
}

#endif /* _LUX_DLIB_H_ */
