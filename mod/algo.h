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
#ifndef _LUX_ALGO_H_
#define _LUX_ALGO_H_

#include <lux/args.h>
#include <lux/spec.h>
#include <stdlib.h> /* for malloc */

/* Forward declaration */
typedef struct LuxSalgo Lux_algo;

struct LuxSalgo {
	/* Because problem is problem specific, the implementation of
	   apply() below is also problem specific.  */
	int (*apply)(Lux_algo *, Lux_args *);

	/* Internally, a Lux_algo should embed the problem specific
	   external and internal specifications.  */
};

/* Base structure that can be returned from functions */
struct basealgo {
	int (*driver)(Lux_spec *spec, Lux_args *args);
	Lux_spec *spec;
	/* basealgo do not store args */
};

/* Forward declaration base implementation */
typedef struct LuxSbasealgo Lux_basealgo;

struct LuxSbasealgo {
	Lux_algo        super;
	struct basealgo base;
};

static inline int
baseapply(Lux_algo *a, Lux_args *args)
{
	#define A ((Lux_basealgo *)a)
	return A->base.driver(A->base.spec, args);
	#undef A
}

static inline Lux_algo *
mkluxbasealgo(struct basealgo b)
{
	Lux_basealgo *a = malloc(sizeof(Lux_basealgo));
	if(a) {
		a->super.apply = baseapply;
		a->base = b;
	}
	return &a->super;
}

#endif /* _LUX_ALGO_H_ */
