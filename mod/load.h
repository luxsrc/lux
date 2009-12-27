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

#include <lux/basename.h>
#include <lux/failed.h>
#include <lux/htab.h>
#include <lux/lazybuf.h>
#include <lux/zalloc.h>
#include <string.h> /* for strcat(), strcpy(), and strlen() */
#include <dlfcn.h>  /* for dlopen(), dlsym(), and dlclose() */

struct load_node {
	struct htab_node super;
	void  *mod;
	void (*rm)(void *);
};

#define FAILED_TO(s) do {                                          \
		int f = failed;                                    \
		lux_debug("vload(\"%s\"): failed to " s " [%s]\n", \
		          name, strfailure(f));                    \
		goto cleanup;                                      \
	} while(0)

#define FAILED_AS(f, s) do {                                       \
		failed = f;                                        \
		lux_debug("vload(\"%s\"): failed to " s " [%s]\n", \
		          name, strfailure(f));                    \
		goto cleanup;                                      \
	} while(0)

static inline void *
vload(struct htab *ltab, const char *restrict name, const void *opts)
{
	char lazybuf[256], *buf;

	struct load_node *node = NULL;

	void *(*mk)(const void *);
	void   *ins = NULL;

	/* Try to allocate more memory if name is too long: note that
	   5 == sizeof("luxC") > sizeof(".so")*/
	buf = (char *)MALLOC(5 + strlen(name));
	if(!buf)
		FAILED_TO("allocate string");

	/* Try to allocate memory for load node */
	node = (struct load_node *)zalloc(sizeof(struct load_node));
	if(!node)
		FAILED_TO("allocate loading table node");

	/* Try to load the module */
	(void)strcat(strcpy(buf, name), ".so");
	node->mod = dlopen(buf, RTLD_LAZY | RTLD_LOCAL);
	if(!node->mod)
		FAILED_AS(FNOMOD, "load module");

	/* Try to get the instance */
	(void)strcat(strcpy(buf, "luxC"), basename(name));
	mk = (void *(*)(const void *))dlsym(node->mod, buf);
	if(mk) {
		int fsv = failed;
		buf[3] = 'D';
		node->rm = (void (*)(void *))dlsym(node->mod, buf);
		if(!node->rm) {
			failed = fsv;
			(void)dlerror();
		}
		ins = mk(opts);
		if(!ins)
			FAILED_AS(F2CONS, "construct");
	} else {
		buf[3] = 'E';
		ins = dlsym(node->mod, buf);
		if(!ins)
			FAILED_AS(FNOSYM, "load entry point");
	}

	/* Save module to a hash table; would not fail */
	hadd(ltab, (uintptr_t)ins, &node->super);
	FREE(buf);
	return ins;

 cleanup:
	if(node) {
		if(node->rm && ins)
			node->rm(ins);
		if(node->mod)
			(void)dlclose(node->mod); /* FIXME: should not clean
			                             up dlerror() */
		free(node);
	}
	if(buf)
		FREE(buf);
	return NULL;
}

static inline void
uload(struct htab *ltab, void *ins)
{
	struct load_node *node = (struct load_node *)hpop(ltab, (uintptr_t)ins);
	if(node) {
		if(node->rm)
			node->rm(ins);
		(void)dlclose(node->mod);
		free(node);
	} else
		failed = FNOMOD;
}

#endif /* _LUX_LOAD_H_ */
