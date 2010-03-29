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

struct qhead {
	struct snode *head;
	struct snode *tail;
};

static inline struct qhead *
queue_init(struct qhead *h)
{
	return (struct qhead *)(h->tail = h->head = (struct snode *)h);
}

static inline struct snode *
enqueue(struct qhead *h, struct snode *s)
{
	ring_ins(h->tail, s);
	h->tail = s;
	return s;
}

static inline struct snode *
dequeue(struct qhead *h)
{
	struct snode *s = (struct snode *)ring_pop((struct snode *)h);
	if(s->next == (struct snode *)h) /* last one or empty */
		h->tail = (struct snode *)h;
	return s;
}

#endif /* _LUX_QUEUE_H_ */
