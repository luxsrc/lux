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
#ifndef _LUX_VECTOR_H_
#define _LUX_VECTOR_H_

#include <lux/xof.h>

#define _VECTOROF(T) struct { size_t d; T e[8]; } /* vector of */
#define _PSIZEOFD(T) offsetof(_VECTOROF(T), e)    /* padded size of dimension */
#define _HEADOF(P)   headof(_VECTOROF(typeof(*P)), P, e)

static inline void *
_valloc(size_t dsz, size_t esz, size_t d)
{
	char *p = malloc(dsz + esz * d);
	*(size_t *)p = d;
	return p + dsz;
}

#define valloc(T, D) ((T *)_valloc(_PSIZEOFD(T), sizeof(T), D))
#define vfree(P) free(_HEADOF(P))
#define dimof(P) (_HEADOF(P)->d)

#endif /* _LUX_VECTOR_H_ */
