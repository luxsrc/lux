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

#include <lux/dlfcn.h>
#include <lux/dlib.h>
#include <lux/dmod.h>
#include <lux/failed.h>
#include <lux/htab.h>
#include <lux/strutils.h>
#include <stdlib.h> /* for NULL, malloc(), and free() */

#define LOAD_NULL {LM_ID_BASE, RTLD_LAZY | RTLD_LOCAL, NULL, HTAB_NULL}

struct load_node {
	struct htab_node super;
	void (*rm)(void *);
};

struct load {
	Lmid_t namespace; /* system link map */
	int    mode;
	const  char *paths;
	struct htab  tab; /* last because of flexible array member */
};

static inline void *
vload(struct load *load, const char *restrict name, const void *opts)
{
	struct dlib l;
	struct dmod m;
	struct load_node *node;

	l = mkdlib(load->namespace, load->paths, name, load->mode);
	if(!l.hdl)
		goto cleanup1; /* failure code was set by mkdlib() */

	m = mkdmod(l, basename(name), opts);
	if(!m.ins)
		goto cleanup2; /* failure code was set by mkdmod() */

	node = (struct load_node *)malloc(sizeof(struct load_node));
	if(!node)
		goto cleanup3; /* failure code was set by malloc() */

	node->rm = m.rm;
	hadd(&load->tab, (uintptr_t)m.ins, &node->super);
	return m.ins;

 cleanup3:
	rmdmod(m);
 cleanup2:
	rmdlib(l);
 cleanup1:
	return NULL;
}

static inline void
uload(struct load *load, void *ins)
{
	struct load_node *node = (struct load_node *)hpop(&load->tab,
	                                                  (uintptr_t)ins);
	if(node) {
		struct dmod m = {ins, node->rm};
		struct dlib l = {dlhandle(load->namespace,
		                          m.rm ? (void *)m.rm : m.ins)};
		rmdmod(m);
		rmdlib(l);
		free(node);
	} else
		failed = FNOMOD;
}

static inline const char *
load_fname(struct load *load, void *ins)
{
	struct load_node *node = (struct load_node *)hget(&load->tab,
	                                                  (uintptr_t)ins);
	if(node)
		return dlfname(node->rm ? (void *)node->rm : ins);
	else
		return NULL;
}

#endif /* _LUX_LOAD_H_ */
