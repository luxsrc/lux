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
#ifndef _LUX_REFCNT_H_
#define _LUX_REFCNT_H_

#define REF_NULL {0}

struct ref {
	int cnt; /* it is usually safe to assume int is atomic */
};

static inline void
refcnt_set(struct ref *ref, int cnt)
{
	ref->cnt = cnt;
}

static inline int
refcnt_get(struct ref *ref)
{
	return ref->cnt;
}

static inline int
refcnt_inc(struct ref *ref)
{
	return __sync_add_and_fetch(&ref->cnt, 1);
}

static inline int
refcnt_dec(struct ref *ref)
{
	return __sync_sub_and_fetch(&ref->cnt, 1);
}

#endif /* _LUX_REFCNT_H_ */
