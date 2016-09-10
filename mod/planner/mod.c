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
#include <lux/lazybuf.h>
#include <lux/mangle.h>
#include <lux/solver.h>
#include <lux/zalloc.h>

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

static Lux_task *
plan(Lux_planner *ego, void *prob, unsigned flag)
{
	return NULL;

	(void)ego;  /* silence unused variable warning */
	(void)prob; /* silence unused variable warning */
	(void)flag; /* silence unused variable warning */
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
