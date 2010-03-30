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

#include <lux/memfd.h>
#include <unistd.h>

struct mpool {
	int    fd;
	size_t psz, sz;
};

#define MPOOL_NULL {-1, 0, 0}

static inline struct mpool
mkmpool(size_t sz)
{
	struct mpool mp = MPOOL_NULL;
	int fd, psz, err;

	fd = memfd_create("mpool", 0);
	if(fd == -1)
		return mp;

	psz = getpagesize();
	sz  = ((sz + psz - 1) / psz) * psz;
	err = ftruncate(fd, sz);
	if(err) {
		close(fd);
		return mp;
	}

	mp.fd  = fd;
	mp.psz = psz;
	mp.sz  = sz;
	return mp;
}

static inline void
rmmpool(struct mpool mp)
{
	close(mp.fd);
}

#endif /* _LUX_MPOOL_H_ */
