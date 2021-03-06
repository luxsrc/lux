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
#ifndef _LUX_ZALLOC_H_
#define _LUX_ZALLOC_H_ 1 /* allocate zeroed memory */

#include <stdlib.h> /* for malloc() */
#include <string.h> /* for memset() */

static inline void *
zalloc(size_t sz)
{
	void *b = malloc(sz);
	return b ? memset(b, 0, sz) : b;
}

#endif /* _LUX_ZALLOC_H_ */
