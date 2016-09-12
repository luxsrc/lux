/*
 * Copyright (C) 2009 Chi-kwan Chan
 * Copyright (C) 2009 Harvard-Smithsonian Center for Astrophysics
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
#ifndef _LUX_TASK_H_
#define _LUX_TASK_H_

#include <lux/algo.h>
#include <lux/args.h>
#include <lux/spec.h>
#include <stdlib.h> /* for malloc */

/* Forward declaration common interface */
typedef struct LuxStask Lux_task;

struct LuxStask {
	/* Because problem and algo are problem specific, the implementation
	   of exec() below is also problem specific.  */
	int (*exec)(Lux_task *);

	/* Internally, a Lux_task should embed the problem specific
	   algo, params, ro, rw, and wo.  */
};

/* Base structure that can be returned from functions */
struct basetask {
	struct basealgo algo;
	Lux_args *args;
};

/* Forward declaration base implementation */
typedef struct LuxSbasetask Lux_basetask;

struct LuxSbasetask {
	Lux_task        super;
	struct basetask base;
};

static inline int
btexec(Lux_task *t)
{
	#define T ((Lux_basetask *)t)
	return T->base.algo.driver(T->base.algo.spec, T->base.args);
	#undef T
}

static inline Lux_task *
mkluxbasetask(struct basetask b)
{
	Lux_basetask *t = malloc(sizeof(Lux_basetask));
	if(t) {
		t->super.exec = btexec;
		t->base = b;
	};
	return &t->super;
}

#endif /* _LUX_TASK_H_ */
