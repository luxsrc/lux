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
#ifndef _LUX_LOAD_H_
#define _LUX_LOAD_H_

#include <lux/libux.h>
#include <lux/dlfcn.h>
#include <lux/dm/dlib.h>
#include <lux/dm/dmod.h>
#include <lux/failed.h>
#include <lux/htab.h>
#include <lux/strutils.h>
#include <stdlib.h> /* for NULL, malloc(), and free() */

#define LOAD_NULL {NULL, HTAB_NULL}

struct load_node {
	struct htab_node super;
	void (*rm)(void *);
};

struct load {
	const  char *paths;
	struct htab  tab; /* last because of flexible array member */
};

static inline void *
vload(struct libux *lux, const char *restrict name, const void *opts)
{
	struct dlib l;
	struct dmod m;
	struct load_node *node;

	l = mkdlib(lux, name);
	if(!l.hdl)
		goto cleanup1; /* failure code was set by mkdlib() */

	m = mkdmod(l, basename(name), opts);
	if(!m.ins)
		goto cleanup2; /* failure code was set by mkdmod() */

	node = (struct load_node *)malloc(sizeof(struct load_node));
	if(!node)
		goto cleanup3; /* failure code was set by malloc() */

	node->rm = m.rm;
	hadd(&lux->load.tab, (uintptr_t)m.ins, &node->super);
	return m.ins;

 cleanup3:
	rmdmod(m);
 cleanup2:
	rmdlib(l);
 cleanup1:
	return NULL;
}

static inline void
uload(struct libux *lux, void *ins)
{
	struct load_node *node = (struct load_node *)hpop(&lux->load.tab,
	                                                  (uintptr_t)ins);
	if(node) {
		struct dmod m = {ins, node->rm};
		struct dlib l = {dlhandle(m.rm ? (void *)m.rm : m.ins)};
		rmdmod(m);
		rmdlib(l);
		free(node);
	} else
		failed = FNOMOD;
}

#endif /* _LUX_LOAD_H_ */
