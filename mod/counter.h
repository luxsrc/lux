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
#ifndef _LUX_COUNTER_H_
#define _LUX_COUNTER_H_

#include <lux/atomic.h>

#define COUNTER_NULL {ATOMIC_NULL}

struct counter {
	atomic_t v;
};

static inline void
counter_set(struct counter *c, int v)
{
	atomic_set(&c->v, v);
}

static inline int
counter_get(struct counter *c)
{
	return atomic_get(&c->v);
}

static inline int
counter_inc(struct counter *c)
{
	return atomic_add(&c->v, 1);
}

static inline int
counter_dec(struct counter *c)
{
	return atomic_sub(&c->v, 1);
}

#endif /* _LUX_COUNTER_H_ */
