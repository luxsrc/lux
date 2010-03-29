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
#ifndef _LUX_RING_H_
#define _LUX_RING_H_

#include <lux/list.h>
#include <lux/stack.h>

static inline struct slist_node *
ring_init(struct slist_node *h)
{
	return h->next = h;
}

static inline struct slist_node *
ring_ins(struct slist_node *h, struct slist_node *s)
{
	s->next = h->next;
	h->next = s;
	return h;
}

static inline struct slist_node *
ring_pop(struct slist_node *h)
{
	struct slist_node *s = h->next;
	h->next = s->next;
	return s;
}

#endif /* _LUX_RING_H_ */
