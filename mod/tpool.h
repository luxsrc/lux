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
#ifndef _LUX_TPOOL_H_
#define _LUX_TPOOL_H_

#include <lux/ap/task.h>
#include <stdlib.h>

struct tnode {
	struct tnode *next;
	Lux_task     *task;
};

struct tpool {
	struct tnode *head; /* == next */
	struct tnode *tail;
};

static struct tpool *
mktpool()
{
	struct tpool *q = malloc(sizeof(struct tpool));
	q->tail = q->head = (struct tnode *)q;
	return q;
}

static void
enqueue(struct tpool *q, Lux_task *t)
{
	struct tnode *n = malloc(sizeof(struct tnode));
	n->task = t;
	n->next = (struct tnode *)q; /* == q->tail->next */
	q->tail->next = n;
	q->tail       = n;
}

static Lux_task *
dequeue(struct tpool *q)
{
	if(q->head == (struct tnode *)q)
		return NULL;
	else {
		struct tnode *n = q->head;
		Lux_task     *t = n->task;
		q->head = n->next;
		free(n);
		return t;
	}
}

#endif /* _LUX_TPOOL_H_ */
