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
#ifndef _LUX_SEM_H_
#define _LUX_SEM_H_

#include <lux/mutex.h>
#include <lux/cond.h>

#define SEM_NULL {0, MUTEX_NULL, COND_NULL}

typedef struct {
	int   super;
	mutex m;
	cond  c;
} sem;

static inline void
sem_post(sem *s)
{
	mutex_lock(&s->m);
	++s->super;
	cond_signal(&s->c);
	mutex_unlock(&s->m);
}

static inline void
sem_wait(sem *s)
{
	mutex_lock(&s->m);
	while(s->super <= 0)
		cond_wait(&s->c, &s->m);
	--s->super;
	mutex_unlock(&s->m);
}

#endif /* _LUX_SEM_H_ */
