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
#ifndef _LUX_PARAFOR_H_
#define _LUX_PARAFOR_H_

#include <lux/alloca.h>
#include <lux/assert.h>
#if _OPENMP
#define LUX_OPENMP 1
#else
#define LUX_THREAD 1
#include <lux/thread.h>
#endif

#include <stddef.h> /* for NULL */

struct paratask {
	void (*func)(size_t, size_t, void *);
	void  *data;
	size_t n;
	size_t l;
	size_t u;
	#if LUX_THREAD
	thread_t t;
	#endif
};

#define T ((struct paratask *)t)
static void *
_execdrv(void *t)
{
	size_t i;
	for(i = T->l; i < T->u; ++i)
		T->func(T->n, i, T->data);
	return NULL;
}
#undef T

static inline size_t
min(size_t a, size_t b)
{
	return a < b ? a : b;
}

static inline void
parafor(size_t n, size_t nthread,
        void (*func)(size_t, size_t, void *), void *data)
{
	size_t bsz, i;
	struct paratask *task;

	lux_assert(nthread && func);

	bsz     = (n + nthread - 1) / nthread;
	nthread = (n + bsz     - 1) / bsz;

	task = alloca(sizeof(struct paratask) * nthread);

	#if LUX_OPENMP
	#pragma omp parallel for
	#endif
	for(i = 0; i < nthread; ++i) { /* spawn */
		struct paratask *t = task + i;

		t->func = func;
		t->data = data;
		t->n    = n;

		t->l   = i * bsz;
		t->u   = min((i+1)*bsz, n);

		#if LUX_THREAD
		if(i < nthread-1)
			t->t = mkthread(_execdrv, t,
			                THREAD_JOINABLE | THREAD_SYSTEM);
		else
		#endif
			(void)_execdrv(t); /* do the job ourself */
	}

	#if LUX_THREAD
	for(i = 0; i < nthread-1; ++i) /* sync */
		(void)thread_join(task[i].t);
	#endif
}

#endif /* _LUX_PARAFOR_H_ */
