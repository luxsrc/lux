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

#define MUTEX_NULL PTHREAD_MUTEX_INITIALIZER

typedef pthread_mutex_t mutex;

static inline void
mutex_lock(mutex *l)
{
	(void)pthread_mutex_lock(l);
}

static inline void
mutex_unlock(mutex *l)
{
	(void)pthread_mutex_unlock(l);
}

#endif /* _LUX_MUTEX_H_ */
