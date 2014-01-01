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
} thread_t;

#define THREAD_JOINABLE (1U << 0)
#define THREAD_DETACHED (1U << 1)
#define THREAD_STATE    (THREAD_JOINABLE | THREAD_DETACHED)

#define THREAD_SYSTEM   (1U << 2)
#define THREAD_PROCESS  (1U << 3)
#define THREAD_SCOPE    (THREAD_SYSTEM | THREAD_PROCESS)

static inline thread_t
mkthread(void *(*pro)(void *), void *arg, unsigned flags)
{
	pthread_attr_t attr;
	thread_t t;

	(void)pthread_attr_init(&attr);
	if(flags & THREAD_STATE)
		(void)pthread_attr_setdetachstate
			(&attr,
			 flags & THREAD_JOINABLE ?
			 PTHREAD_CREATE_JOINABLE :
			 PTHREAD_CREATE_DETACHED);
	if(flags & THREAD_SCOPE)
		(void)pthread_attr_setscope
			(&attr,
			 flags & THREAD_SYSTEM ?
			 PTHREAD_SCOPE_SYSTEM  :
			 PTHREAD_SCOPE_PROCESS);

	(void)pthread_create(&t.id, &attr, pro, arg);

	(void)pthread_attr_destroy(&attr);
	return t;
}

static inline void
thread_exit(void *status)
{
	pthread_exit(status);
}

static inline void *
thread_join(thread_t t)
{
	void *status;
	(void)pthread_join(t.id, &status);
	return status;
}

static inline void
rmthread(thread_t t)
{
	(void)pthread_cancel(t.id);
}

#endif /* _LUX_THREAD_H_ */
