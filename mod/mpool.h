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
	size_t sz;
};

static inline struct mpool
mkmpool(size_t sz)
{
	struct mpool mp = {memfd_create("mpool", 0), sz};
	if(mp.fd == -1) /* file open error */
		mp.sz = 0;
	else {
		int err = ftruncate(mp.fd, sz);
		if(err) {
			close(mp.fd);
			mp.fd = -1;
			mp.sz =  0;
		}
	}
	return mp;
}

static inline void
rmmpool(struct mpool mp)
{
	close(mp.fd);
}

#endif /* _LUX_MPOOL_H_ */
