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

#include <lux/cond.h>
#include <lux/dynamic.h>
#include <lux/mutex.h>
#include <lux/queue.h>
#include <lux/task.h>
#include <lux/thread.h>
#include <stdlib.h>

struct tnode {
	struct slist_node super;
	Lux_task *task;
};

struct tpool {
	struct queue_head super;
	mutex_t lock;
	cond_t  done;
	volatile size_t njob;
	volatile size_t nactive;
	volatile size_t nthread;
};

/* Forward declaration */
static Lux_task *tpool_dequeue(struct tpool *);

#define Q ((struct tpool *)q)
static void *
_execdrv(void *q)
{
	mutex_lock(&Q->lock);
	++Q->nactive;
	mutex_unlock(&Q->lock);

	for(;;) {
		Lux_task *t = tpool_dequeue(q);
		if(t) {
			t->exec(t);
			mutex_lock(&Q->lock);
			--Q->njob;
			cond_signal(&Q->done);
			mutex_unlock(&Q->lock);
		}
		mutex_lock(&Q->lock);
		if(Q->nactive > Q->nthread)
			break;
		mutex_unlock(&Q->lock); /* <---+                             */
	}                               /*     |                             */
	--Q->nactive;                   /*     +---- these two unlocks match */
	cond_signal(&Q->done);          /*     |                             */
	mutex_unlock(&Q->lock);         /* <---+                             */
	return NULL;
}
#undef Q

static struct tpool *
mktpool(size_t nthread)
{
	struct tpool *q = malloc(sizeof(struct tpool));
	q->super   = localof(struct queue_head, QUEUE_HEAD_INIT(&q->super));
	q->lock    = localof(mutex_t, MUTEX_NULL);
	q->done    = localof(cond_t,  COND_NULL);
 	q->njob    = 0;
	q->nactive = 0;
	q->nthread = nthread;

	while(nthread--)
		(void)mkthread(_execdrv, q, THREAD_JOINABLE | THREAD_SYSTEM);

	return q;
}

static void
tpool_enqueue(struct tpool *q, Lux_task *t)
{
	struct tnode *n = dynamic(struct tnode, {{NULL}, t});

	mutex_lock(&q->lock);
	enqueue(&q->super, &n->super);
	++q->njob;
	mutex_unlock(&q->lock);
}

static Lux_task *
tpool_dequeue(struct tpool *q)
{
	struct slist_node *s;

	mutex_lock(&q->lock);
	s = dequeue(&q->super);
	mutex_unlock(&q->lock);

	if(s) {
		struct tnode *n = headerof(struct tnode, s, super);
		Lux_task *t = n->task;
		free(n);
		return t;
	} else
		return NULL;
}

static void
tpool_wait(struct tpool *q)
{
	mutex_lock(&q->lock);
	while(q->njob)
		cond_wait(&q->done, &q->lock);
	mutex_unlock(&q->lock);
}

static void
rmtpool(struct tpool *q)
{
	tpool_wait(q);

	mutex_lock(&q->lock);
	q->nthread = 0;
	while(q->nactive)
		cond_wait(&q->done, &q->lock);
	mutex_unlock(&q->lock);

	free(q);
}

#endif /* _LUX_TPOOL_H_ */
