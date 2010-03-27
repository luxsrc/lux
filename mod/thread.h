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
#ifndef _LUX_THREAD_H_
#define _LUX_THREAD_H_

#include <pthread.h>

typedef struct {
	pthread_t id;
} thread;

static inline thread
mkthread(void *(*pro)(void *), void *arg)
{
	thread t;
	(void)pthread_create(&t.id, NULL, pro, arg);
	return t;
}

static inline void
thread_exit(void *status)
{
	pthread_exit(status);
}

static inline void *
thread_join(thread t)
{
	void *status;
	(void)pthread_join(t.id, &status);
	return status;
}

static inline void
rmthread(thread t)
{
	(void)pthread_cancel(t.id);
}

#endif /* _LUX_THREAD_H_ */
