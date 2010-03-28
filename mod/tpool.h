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
#include <lux/mutex.h>
#include <stdlib.h>

struct tnode {
	struct tnode *next;
	Lux_task     *task;
};

struct tpool {
	struct tnode *head; /* == next */
	struct tnode *tail;
	mutex lock;
};

/* Forward declaration */
static Lux_task *dequeue(struct tpool *);

#define W ((struct worker *)w)
static void *
_execdrv(void *q)
{
	for(;;) {
		Lux_task *t = dequeue(q);
		if(t)
			t->exec(t);
	}
	return NULL;
}
#undef W

static struct tpool *
mktpool(size_t nthread)
{
	struct tpool *q = malloc(sizeof(struct tpool));
	q->tail = q->head = (struct tnode *)q;
	q->lock = ({ mutex _ = MUTEX_NULL; _; });

	while(nthread--)
		(void)mkthread(_execdrv, q, THREAD_JOINABLE | THREAD_SYSTEM);

	return q;
}

static void
enqueue(struct tpool *q, Lux_task *t)
{
	struct tnode *n = malloc(sizeof(struct tnode));
	n->task = t;
	n->next = (struct tnode *)q; /* == q->tail->next always */

	mutex_lock(&q->lock);
	q->tail = q->tail->next = n;
	mutex_unlock(&q->lock);
}

static Lux_task *
dequeue(struct tpool *q)
{
	struct tnode *n = NULL;

	mutex_lock(&q->lock);
	if(q->head != (struct tnode *)q) {
		n = q->head;
		if(n->next != (struct tnode *)q)
			q->head = n->next;
		else
			q->tail = q->head = n->next;
	}
	mutex_unlock(&q->lock);

	if(n) {
		Lux_task *t = n->task;
		free(n);
		return t;
	} else
		return NULL;
}

#endif /* _LUX_TPOOL_H_ */
