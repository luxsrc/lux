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
#ifndef _LUX_DMOD_H_
#define _LUX_DMOD_H_

#include <lux/dm/dlib.h>
#include <lux/dm/dltry.h>
#include <lux/failed.h>
#include <lux/lazybuf.h>
#if HAVE_STDDEF_H
#include <stddef.h> /* for NULL and size_t */
#else
#include <stdlib.h> /* for NULL and size_t */
#endif
#include <string.h> /* for strlen() and memcpy() */

#define DMOD_NULL {NULL, NULL}

struct dmod {
	void  *ins;         /* instance */
	void (*rm)(void *); /* remove   */
};

static inline struct dmod
mkdmod(struct dlib l, const char *restrict name, const void *opts)
{
	struct dmod m = DMOD_NULL;
	void *(*mk)(const void *);

	char lazybuf[256] = "luxC", *buf = lazybuf;
	const size_t nlen = strlen(name);

	buf = (char *)lzrealloc(buf, sizeof("luxC") + nlen);
	if(!buf)
		return m; /* == DMOD_NULL; no need to lzfree(buf);
		             failure code was set by lzrealloc() */
	(void)memcpy(buf + sizeof("luxC") - 1, name, nlen + 1);

	mk = (void *(*)(const void *))dltrysym(l.hdl, buf);
	if(mk) {
		buf[3] = 'D';
		m.rm   = dltrysym(l.hdl, buf);
		m.ins  = mk(opts);
		if(!m.ins)
			failed = F2CONS;
	} else {
		buf[3] = 'E';
		m.ins  = dltrysym(l.hdl, buf);
		if(!m.ins)
			failed = FNOSYM;
	}

	lzfree(buf);
	return m;
}

static inline void
rmdmod(struct dmod m)
{
	if(!m.ins)
		failed = FNOMOD;
	else if(m.rm)
		m.rm(m.ins);
}

#endif /* _LUX_DMOD_H_ */
