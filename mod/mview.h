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
#ifndef _LUX_MVIEW_H_
#define _LUX_MVIEW_H_

#include <lux/mpool.h>
#include <unistd.h>

static inline void *
mkmview(struct mpool *mp, size_t lower, size_t length)
{
	mpool_resize(mp, lower+length);
	return mmap(NULL, length,
	            PROT_READ | PROT_WRITE,
	            MAP_SHARED,
	            mp->fd, lower);
}

static inline void
rmmview(struct mpool *mp, void *v, size_t length)
{
	(void)munmap(v, length);
	(void)mp; /* silence unused variable warning */
}

#endif /* _LUX_MVIEW_H_ */
