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
#include <lux/basename.h>
#include <lux/htab.h>
#include <lux/lazybuf.h>
#include <string.h> /* for strlen() */
#include <stdio.h>  /* for sprintf() */
#include <dlfcn.h>  /* for dlopen(), dlsym(), and dlclose() */

struct load_node {
	struct htab_node super;
	void (*rm)(void *);
	void  *mod;
};

static struct htab ltab = HTAB_INIT; /* the loading table */

#define FAILED_TO(s) do {                                               \
		lux_error("lux_load(\"%s\"): failed to " s "\n", name); \
		goto cleanup;                                           \
	} while(0)

void *
lux_load(const char *restrict name)
{
	char lazybuf[256], *buf;

	void   *mod = NULL;
	void *(*mk)(void);
	void  (*rm)(void *) = NULL;
	void   *obj = NULL;

	struct load_node *node;

	/* Try to allocate more memory if name is too long */
	buf = (char *)MALLOC(sizeof(LUX_PREFIX) + 12 + strlen(name));
	if(!buf)
		FAILED_TO("allocate string");

	/* Try to load the module */
	(void)sprintf(buf, LUX_PREFIX "/lib/lux/%s.so", name);
	mod = dlopen(buf, RTLD_LAZY);
	if(!mod)
		FAILED_TO("load module");

	/* Try to get the instance */
	(void)sprintf(buf, "luxC%s", basename(name));
	mk = (void *(*)(void))dlsym(mod, buf);
	if(mk) {
		buf[3] = 'D';
		rm  = (void (*)(void *))dlsym(mod, buf);
		obj = mk();
	} else {
		buf[3] = 'E';
		obj = dlsym(mod, buf);
	}
	if(!obj)
		FAILED_TO("instanize module");

	/* Try to save module to a hash table */
	node = HADD(&ltab, obj, struct load_node);
	if(!node)
		FAILED_TO("allocate loading table node");

	node->rm  = rm;
	node->mod = mod;
	FREE(buf);
	return obj;

 cleanup:
	if(rm && obj)
		rm(obj);
	if(mod)
		(void)dlclose(mod);
	if(buf)
		FREE(buf);
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
