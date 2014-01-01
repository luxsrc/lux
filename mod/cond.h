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
#ifndef _LUX_COND_H_
#define _LUX_COND_H_

#include <lux/mutex.h> /* already includes <pthread.h> */

#define COND_NULL PTHREAD_COND_INITIALIZER

typedef struct {
	pthread_cond_t c;
} cond_t;

static inline int
cond_broadcast(cond_t *c)
{
	return pthread_cond_broadcast(&c->c);
}

static inline int
cond_signal(cond_t *c)
{
	return pthread_cond_signal(&c->c);
}

static inline int
cond_wait(cond_t *restrict c, mutex_t *restrict m)
{
	return pthread_cond_wait(&c->c, &m->m);
}

#endif /* _LUX_COND_H_ */
