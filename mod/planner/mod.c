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
#include <lux.h>
#include <lux/dpath.h>
#include <lux/estimate.h>
#include <lux/lazybuf.h>
#include <lux/mangle.h>
#include <lux/measure.h>
#include <lux/pvector.h>
#include <lux/solver.h>
#include <lux/zalloc.h>

#include <stdlib.h>
#include <math.h>

#include "../planner.h"

#define EGO ((struct planner *)ego)

struct planner {
	Lux_planner super;
	size_t      n;
	Lux_solver *solve[1];
};

struct solution {
	Lux_solution super;
	double       ecost;
	struct mcost mcost;
};

static inline size_t
occur(const char *s, char c)
{
	size_t n;
	for(n = 0; *s; ++s) if(*s == c) ++n;
	return n;
}

static int
cmp(const void *a, const void *b)
{
	return (((const struct solution *)a)->ecost -
	        ((const struct solution *)b)->ecost);
}

static size_t
min(size_t a, size_t b)
{
	return a < b ? a : b;
}

static struct basealgo
plan(Lux_planner *ego, Lux_problem *prob, unsigned flags)
{
	size_t N = 8;
	struct solution *sols = malloc(sizeof(struct solution) * N);

	size_t i, n = 0;
	double bestcost;
	struct basealgo best;

	/* Gather solutions from all the solvers */
	for(i = 0; i < EGO->n; ++i) {
		Lux_solution *s = EGO->solve[i](prob, flags);

		size_t j, m = pgetn(s, 0);
		while(n + m > N)
			sols = realloc(sols, sizeof(struct solution) * (N*=2));

		for(j = 0; j < m; ++n, ++j)
			sols[n].super = s[j];

		pfree(s);
	}

	/* Always estimate performance based on operation counts */
	for(i = 0; i < n; ++i)
		sols[i].ecost = estimate(&sols[i].super.opcnt);

	/* Sort according to estimated performance */
	qsort(sols, n, sizeof(struct solution), cmp);

	/* How many plans should we measure performance? */
	switch(flags) {
	case LUX_PLAN_ESTIMATE:   N = 0;                  break;
	case LUX_PLAN_MEASURE:    N = min(n, EGO->n * 2); break;
	case LUX_PLAN_PATIENT:    N = min(n, EGO->n * 4); break;
	case LUX_PLAN_EXHAUSTIVE: N = n;                  break;
	}

	/* Measure performance */
	for(i = 0; i < N; ++i) {
		Lux_task *task = mkluxbasetask(sols[i].super.task);
		double    t    = measure(task);
		free(task);
		sols[i].mcost.n   = 1;
		sols[i].mcost.tot = t;
		sols[i].mcost.min = t;
		sols[i].mcost.max = t;
		lux_debug("Lux_planner.plan(): measure %p %p %p: %g\n",
		          sols[i].super.task.algo.driver,
		          sols[i].super.task.algo.spec,
		          sols[i].super.task.args,
		          sols[i].mcost.tot/sols[i].mcost.n);
	}

	/* Find the best solution */
	bestcost = HUGE_VAL;
	for(i = 0; i < N; ++i) {
		if(bestcost > sols[i].mcost.min) {
			bestcost = sols[i].mcost.min;
			best     = sols[i].super.task.algo;
		}
	}

	/* Free unused specs, args, and the list of solutions */
	for(i = 0; i < N; ++i) {
		Lux_args *a = sols[i].super.task.args;
		Lux_spec *s = sols[i].super.task.algo.spec;

		int freed = 0;
		for(n = 0; n < i; ++n)
			freed = (a == sols[n].super.task.args);
		if(!freed)
			free(a);

		if(s != best.spec)
			free(s);
	}
	free(sols);

	/* Done and return the best solution */
	return best;
}

void *
LUX_MKMOD(const void *opts)
{
	struct planner *ego = NULL;
	const  char    *path;
	       char    *mods;
	       size_t   n;

	path = dpath(opts);
	lux_debug("Lux_planner: path = \"%s\"\n", path);
	if(!path)
		goto cleanup0;

	mods = dsubmods(path); /* is writable */
	lux_debug("Lux_planner: mods = \"%s\"\n", mods);
	if(!mods)
		goto cleanup1;

	n    = occur(mods, ':') + 1;
	ego  = zalloc(      sizeof(struct planner)
	              -     sizeof(Lux_solver *)
	              + n * sizeof(Lux_solver *));
	if(ego) {
		      size_t i;
		const char  *m;

		char lazybuf[256], *buf;
		size_t plen = strlen(path);
		buf = lzmalloc(plen + sizeof("/"));
		strcpy(buf, path);
		strcpy(buf + plen, "/");

		for(i=0, m=strtok(mods, ":"); m; ++i, m=strtok(NULL, ":")) {
			size_t mlen = strlen(m);
			buf = lzrealloc(buf, plen + sizeof("/") + mlen);
			strcpy(buf + plen + 1, m);
			lux_debug("Lux_planner: submodule = \"%s\"\n", buf);
			ego->solve[i] = lux_load(buf, NULL);
		}

		ego->super.plan = plan;
		ego->n          = n;
	}

	free(mods);
cleanup1:
	free((void *)path);
cleanup0:
	return ego;
}

void
LUX_RMMOD(void *ego)
{
	size_t i;
	for(i = 0; i < EGO->n; ++i)
		lux_unload(EGO->solve[i]);

	free(ego);
}