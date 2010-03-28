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
#ifndef _LUX_WORKER_H_
#define _LUX_WORKER_H_

#include <lux/task.h>
#include <lux/sem.h>
#include <lux/thread.h>

#include <stdlib.h>

struct worker {
	Lux_task *task;
	sem       ready;
	sem       done;
};

#define WORKER_NULL {NULL, SEM_NULL, SEM_NULL}
#define SET(p, v) do { const typeof(*p) _ = v; *p = _; } while(0)

#define W ((struct worker *)w)
static void *
_execdrv(void *w)
{
	Lux_task *t;
	do {
		sem_wait(&W->ready);
		t = W->task;
		if(t)
			t->exec(t);
		sem_post(&W->done);
	} while(t);
	return NULL;
}
#undef W

static inline struct worker *
mkworker(size_t sz)
{
	struct worker *w = malloc(sz ? sz : sizeof(struct worker));
	SET(w, WORKER_NULL);
	(void)mkthread(_execdrv, w, THREAD_JOINABLE | THREAD_SYSTEM);
	sem_post(&w->done);
	return w;
}

static inline void
worker_submit(struct worker *w, Lux_task *t)
{
	sem_wait(&w->done);
	w->task = t;
	sem_post(&w->ready);
}

static inline Lux_task *
rmworker(struct worker *w)
{
	Lux_task *t = w->task;
	worker_submit(w, NULL);
	sem_wait(&w->done);
	free(w);
	return t;
}

#endif /* _LUX_WORKER_H_ */
