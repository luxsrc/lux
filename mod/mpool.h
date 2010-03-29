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

#include <lux/ap/ticks.h>
#include <stdio.h>
#include <unistd.h>

#if HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif

#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#if HAVE_FCNTL_H
#include <fcntl.h>
#endif

struct mpool {
	int    fd;
	size_t sz;
};

static inline struct mpool
mkmpool(size_t sz)
{
	struct mpool mp;
	int err;

	ticks t = getticks();

	char name[32]; /* 64-bit integer has at most 20 digits */
	(void)snprintf(name, sizeof(name), "/lux-%u", (unsigned)t);

	mp.fd = shm_open(name, O_RDWR|O_CREAT|O_EXCL|O_NOFOLLOW, 0600);
	if(mp.fd != -1) {
		err = ftruncate(mp.fd, sz);
		(void)shm_unlink(name);
	} else
		err = 1;
	mp.sz = err ? 0 : sz;

	return mp;
}

static inline void
rmmpool(struct mpool mp)
{
	close(mp.fd);
}

#endif /* _LUX_MPOOL_H_ */
