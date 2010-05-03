/*
 * Copyright (C) 2010 Chi-kwan Chan
 * Copyright (C) 2010 Harvard-Smithsonian Center for Astrophysics
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
#ifndef _LUX_MPOOL_H_
#define _LUX_MPOOL_H_

#include <lux/htab.h>
#include <lux/memfd.h>
#include <lux/zalloc.h> /* zalloc() and free() */
#include <unistd.h>     /* getpagesize(), ftruncate(), and close() */

struct mpool {
	int    fd;
	size_t psz, sz;
	struct htab tab; /* last because of flexible array member */
};

#define MPOOL_NULL {-1, 0, 0, HTAB_NULL}

static inline struct mpool *
mkmpool(size_t sz)
{
	struct mpool *mp;
	int fd, psz, err;

	mp = zalloc(sizeof(struct mpool));
	if(!mp)
		goto cleanup1;
	mp->tab.count = HTAB_COUNT;

	fd = memfd_create("mpool", 0);
	if(fd == -1)
		goto cleanup2;

	psz = getpagesize();
	sz  = ((sz + psz - 1) / psz) * psz;
	err = ftruncate(fd, sz);
	if(err)
		goto cleanup3;

	mp->fd  = fd;
	mp->psz = psz;
	mp->sz  = sz;
	return mp;

 cleanup3:
	close(fd);
 cleanup2:
	free(mp);
 cleanup1:
	return NULL;
}

static inline void
rmmpool(struct mpool *mp)
{
	close(mp->fd);
	free(mp);
}

#endif /* _LUX_MPOOL_H_ */
