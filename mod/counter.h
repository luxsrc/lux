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

#define CNT_NULL {ATOMIC_NULL}

struct cnt {
	atomic cnt;
};

static inline void
counter_set(struct cnt *cnt, int v)
{
	atomic_set(&cnt->cnt, v);
}

static inline int
counter_get(struct cnt *cnt)
{
	return atomic_get(&cnt->cnt);
}

static inline int
counter_inc(struct cnt *cnt)
{
	return atomic_add(&cnt->cnt, 1);
}

static inline int
counter_dec(struct cnt *cnt)
{
	return atomic_sub(&cnt->cnt, 1);
}

#endif /* _LUX_COUNTER_H_ */
