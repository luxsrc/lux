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
#ifndef _LUX_LAZYBUF_H_
#define _LUX_LAZYBUF_H_

#include <stdlib.h> /* for malloc(), realloc(), and free() */
#include <string.h> /* for memcpy() */

static inline void *
_malloc(void *b, size_t bsz, size_t sz)
{
	return sz > bsz ? malloc(sz) : b;
}

static inline void *
_realloc(void *b, size_t bsz, void *pt, size_t sz)
{
	if(pt != b) /* most likely, use system realloc() */
		return realloc(pt, sz);
	else if(sz == 0) /* emulate the system realloc() behavior */
		return NULL;
	else if(sz <= bsz) /* buffer is big enough, do nothing */
		return pt;
	else { /* i.e., pt == b && sz > bsz: allocate memory and copy data */
		b = malloc(sz);
		return b ? memcpy(b, pt, bsz) : b;
	}
}

static inline void
_free(void *b, void *pt)
{
	if(pt != b)
		free(pt);
}

#define MALLOC(sz)      _malloc(lazybuf, sizeof(lazybuf), sz)
#define REALLOC(pt, sz) _realloc(lazybuf, sizeof(lazybuf), pt, sz)
#define FREE(pt)        _free(lazybuf, pt)

#endif /* _LUX_LAZYBUF_H_ */
