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
#include <lux.h>
#include <lux/htab.h>
#include <stdio.h> /* for sprintf() */
#include <dlfcn.h> /* for dlopen(), dlsym(), and dlclose() */

struct load_node {
	struct htab_node super;
	void (*rm)(void *);
	void  *mod;
};

static struct htab ltab = HTAB_INIT; /* the loading table */

void *
lux_load(const char *restrict name)
{
	char buf[256]; /* FIXME: check string length */

	void   *mod;
	void *(*mk)(void);
	void  (*rm)(void *) = NULL;
	void   *obj = NULL;

	struct load_node *node;

	/* Try to load the module */
	(void)sprintf(buf, LUX_PREFIX "/lib/lux/%s.so", name);
	mod = dlopen(buf, RTLD_LAZY);
	if(!mod) {
		lux_error("lux_load(\"%s\"): failed to load module\n", name);
		goto cleanup;
	}

	/* Try to get the instance */
	(void)sprintf(buf, "luxC%s", name);
	mk = (void *(*)(void))dlsym(mod, buf);
	if(mk) {
		buf[3] = 'D';
		rm  = (void (*)(void *))dlsym(mod, buf);
		obj = mk();
	} else {
		buf[3] = 'E';
		obj = dlsym(mod, buf);
	}
	if(!obj) {
		lux_error("lux_load(\"%s\"): failed to instanize\n", name);
		goto cleanup;
	}

	/* Try to save module to a hash table */
	node = HADD(&ltab, obj, struct load_node);
	if(!node) {
		lux_error("lux_load(\"%s\"): failed to allocate node\n", name);
		goto cleanup;
	}

	node->rm  = rm;
	node->mod = mod;
	return obj;

 cleanup:
	if(rm && obj)
		rm(obj);
	if(mod)
		(void)dlclose(mod);
	return NULL;
}

void
lux_unload(void *obj)
{
	struct load_node *node = HPOP(&ltab, obj, struct load_node);
	if(node) {
		if(node->rm)
			node->rm(obj);
		(void)dlclose(node->mod);
		free(node);
	} else
		lux_error("lux_unload(%p): module not found\n", obj);
}
