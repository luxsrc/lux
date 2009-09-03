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
#ifndef _LUX_DOPE_H_
#define _LUX_DOPE_H_

#include <stdlib.h> /* for malloc() and free() */

typedef struct {
	Lux_int r; /* rank/dimension */
	Lux_int c; /* count/size */
	Lux_int s; /* stride */
} Lux_dope;

static inline Lux_dope *
mkdope(size_t sz, int rank)
{
	Lux_dope *head = (Lux_dope *)malloc(rank * sizeof(Lux_dope) + sz);
	if(head) {
		Lux_int r;
		for(r = 0; r < rank; ++r)
			head[r].r = r;
		if(sz >= sizeof(Lux_int))
			head[r].r = r; /* r == rank here */
		return head + rank;
	} else
		return NULL;
}

static inline void
rmdope(Lux_dope *dope)
{
	free(dope - dope->r);
}

#endif /* _LUX_DOPE_H_ */
