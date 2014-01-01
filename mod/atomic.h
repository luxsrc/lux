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
#ifndef _LUX_ATOMIC_H_
#define _LUX_ATOMIC_H_

#define ATOMIC_NULL {0}

typedef struct {
	int v; /* it is usually safe to assume int is atomic */
} atomic_t;

static inline void
atomic_set(atomic_t *a, int v)
{
	a->v = v;
}

static inline int
atomic_get(atomic_t *a)
{
	return a->v;
}

static inline int
atomic_add(atomic_t *a, int v)
{
	return __sync_add_and_fetch(&a->v, v);
}

static inline int
atomic_sub(atomic_t *a, int v)
{
	return __sync_sub_and_fetch(&a->v, v);
}

#endif /* _LUX_ATOMIC_H_ */
