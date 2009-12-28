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

#define CNT_NULL {0}

struct cnt {
	int cnt; /* it is usually safe to assume int is atomic */
};

static inline void
counter_set(struct cnt *cnt, int v)
{
	cnt->cnt = v;
}

static inline int
counter_get(struct cnt *cnt)
{
	return cnt->cnt;
}

static inline int
counter_inc(struct cnt *cnt)
{
	return __sync_add_and_fetch(&cnt->cnt, 1);
}

static inline int
counter_dec(struct cnt *cnt)
{
	return __sync_sub_and_fetch(&cnt->cnt, 1);
}

#endif /* _LUX_COUNTER_H_ */
