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
#ifndef _LUX_MUTEX_H_
#define _LUX_MUTEX_H_

#include <pthread.h>

#define MUTEX_NULL {PTHREAD_MUTEX_INITIALIZER}

typedef struct {
	pthread_mutex_t m;
} mutex_t;

static inline int
mutex_lock(mutex_t *m)
{
	return pthread_mutex_lock(&m->m);
}

static inline int
mutex_trylock(mutex_t *m)
{
	return pthread_mutex_trylock(&m->m);
}

static inline int
mutex_unlock(mutex_t *m)
{
	return pthread_mutex_unlock(&m->m);
}

#endif /* _LUX_MUTEX_H_ */
