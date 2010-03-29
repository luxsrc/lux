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
#ifndef _LUX_QUEUE_H_
#define _LUX_QUEUE_H_

#include <lux/list.h>
#include <lux/ring.h>
#include <stddef.h>

struct queue_head {
	struct slist_node *head;
	struct slist_node *tail;
};

#define QUEUE_HEAD_INIT(h) {(struct slist_node *)h, (struct slist_node *)h}
#define QUEUE_NODE_NULL    {NULL}

static inline void
enqueue(struct queue_head *h, struct slist_node *s)
{
	ring_ins(h->tail, s);
	h->tail = s;
}

static inline struct slist_node *
dequeue(struct queue_head *h)
{
	struct slist_node *s = ring_pop((struct slist_node *)h);
	if(s == (struct slist_node *)h) /* empty */
		return NULL;
	if(s->next == (struct slist_node *)h) /* last one */
		h->tail = (struct slist_node *)h;
	return s;
}

#endif /* _LUX_QUEUE_H_ */
