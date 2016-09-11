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
#include <lux/parray.h>
#include <lux/solver.h>
#include <lux/zalloc.h>

#include <stdlib.h>
#include <float.h>

#include "../planner.h"

#define EGO ((struct planner *)ego)

struct planner {
	Lux_planner super;

	size_t      n;
	Lux_solver *solver[1];
};

static inline size_t
occur(const char *s, char c)
{
	size_t n;
	for(n = 0; *s; ++s) if(*s == c) ++n;
	return n;
}

static int
cmp(const void *pa, const void *pb)
{
	Lux_solution *a = *(Lux_solution **)pa;
	Lux_solution *b = *(Lux_solution **)pb;
	return a->ecost - b->ecost;
}

static size_t
min(size_t a, size_t b)
{
	return a < b ? a : b;
}

static Lux_task *
plan(Lux_planner *ego, void *prob, unsigned flags)
{
	size_t N = 8;
	Lux_solution **sols = malloc(sizeof(Lux_solution *) * N);

	size_t i, n = 0;
	double    bestcost;
	Lux_task *best;

	/* Gather solutions from all the solvers */
	for(i = 0; i < EGO->n; ++i) {
		Lux_solver    *s  = EGO->solver[i];
		Lux_solution **ss = s->solve(s, prob, flags);

		size_t j, m = pgetn(ss, 0);

		while(n + m > N)
			sols = realloc(sols, sizeof(Lux_solution *) * (N*=2));

		for(j = 0; j < m; ++n, ++j)
			sols[n] = ss[j];

		pfree(ss);
	}

	/* Always estimate performance based on operation counts */
	for(i = 0; i < n; ++i)
		sols[i]->ecost = estimate(&sols[i]->opcnt);

	/* Sort according to estimated performance */
	qsort(sols, n, sizeof(Lux_solution *), cmp);

	/* How many plans should we measure performance? */
	switch(flags) {
	case LUX_PLAN_ESTIMATE:   N = 0;                  break;
	case LUX_PLAN_MEASURE:    N = min(n, EGO->n * 2); break;
	case LUX_PLAN_PATIENT:    N = min(n, EGO->n * 4); break;
	case LUX_PLAN_EXHAUSTIVE: N = n;                  break;
	}

	/* Measure performance */
	for(i = 0; i < N; ++i) {
		double t = measure(sols[i]->task);
		sols[i]->mcost.n   = 1;
		sols[i]->mcost.tot = t;
		sols[i]->mcost.min = t;
		sols[i]->mcost.max = t;
	}

	/* Find the best solution */
	bestcost = DBL_MAX;
	best     = sols[0]->task;
	for(i = 0; i < N; ++i) {
		if(bestcost > sols[i]->mcost.min) {
			bestcost = sols[i]->mcost.min;
			best     = sols[i]->task;
		}
	}

	/* Free every solution and unused tasks */
	for(i = 0; i < n; ++i) {
		if(sols[i]->task != best)
			free(sols[i]->task);
		free(sols[i]);
	}
	free(sols);

	/* Done and return the best solution */
	return best;
}

void *
LUX_MKMOD(const void *opts)
{
	const char  *path = dpath(opts);
	      char  *mods = path ? dsubmods(path) : NULL;
	const size_t n    = mods ? occur(mods, ':') + 1 : 0;

	struct planner *ego = zalloc(      sizeof(struct planner)
	                             -     sizeof(Lux_solver *)
	                             + n * sizeof(Lux_solver *));
	if(ego) {
		size_t i;
		const char *m;

		char lazybuf[256], *buf;

		size_t plen = strlen(path);
		buf = lzmalloc(plen + sizeof("/"));
		strcpy(buf, path);
		strcpy(buf + plen, "/");

		for(i=0, m=strtok(mods, ":"); m; ++i, m=strtok(NULL, ":")) {
			size_t mlen = strlen(m);
			buf = lzrealloc(buf, plen + sizeof("/") + mlen);
			strcpy(buf + plen + 1, m);
			ego->solver[i] = lux_load(buf, NULL);
		}

		ego->super.plan = plan;
		ego->n          = n;
	}

	if(mods)
		free(mods);
	if(path)
		free((void *)path);

	return ego;
}

void
LUX_RMMOD(void *ego)
{
	size_t i;
	for(i = 0; i < EGO->n; ++i)
		lux_unload(EGO->solver[i]);

	free(ego);
}
