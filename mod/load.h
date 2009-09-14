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

#include <lux.h>
#include <lux/basename.h>
#include <lux/failed.h>
#include <lux/htab.h>
#include <lux/lazybuf.h>
#include <string.h> /* for strcat(), strcpy(), and strlen() */
#include <dlfcn.h>  /* for dlopen(), dlsym(), and dlclose() */

struct load_node {
	struct htab_node super;
	void (*rm)(void *);
	void  *mod;
};

static struct htab ltab = HTAB_INIT; /* the loading table */

#define FAILED_TO(s) do {                                          \
		int f = failed;                                    \
		lux_debug("vload(\"%s\"): failed to " s " [%s]\n", \
		          name, failure_msg(f));                   \
		goto cleanup;                                      \
	} while(0)

#define FAILED_AS(f, s) do {                                       \
		failed = f;                                        \
		lux_debug("vload(\"%s\"): failed to " s " [%s]\n", \
		          name, failure_msg(f));                   \
		goto cleanup;                                      \
	} while(0)

static inline void *
vload(const char *restrict name, va_list ap)
{
	char lazybuf[256], *buf;

	void   *mod = NULL;
	void *(*mk)(va_list);
	void  (*rm)(void *) = NULL;
	void   *ins = NULL;

	struct load_node *node;

	/* Try to allocate more memory if name is too long: note that
	   5 == sizeof("luxC") > sizeof(".so")*/
	buf = (char *)MALLOC(5 + strlen(name));
	if(!buf)
		FAILED_TO("allocate string");

	/* Try to load the module */
	(void)strcat(strcpy(buf, name), ".so");
	mod = dlopen(buf, RTLD_LAZY);
	if(!mod)
		FAILED_AS(FNOMOD, "load module");

	/* Try to get the instance */
	(void)strcat(strcpy(buf, "luxC"), basename(name));
	mk = (void *(*)(va_list))dlsym(mod, buf);
	if(mk) {
		int fsv = failed;
		buf[3] = 'D';
		rm = (void (*)(void *))dlsym(mod, buf);
		if(!rm) {
			failed = fsv;    /* hide error emitted by dlsym() */
			(void)dlerror(); /* clear libdl error message     */
		}
		ins = mk(ap);
		if(!ins)
			FAILED_AS(F2CONS, "construct");
	} else {
		buf[3] = 'E';
		ins = dlsym(mod, buf);
		if(!ins)
			FAILED_AS(FNOSYM, "load entry point");
	}

	/* Try to save module to a hash table */
	node = HADD(&ltab, ins, struct load_node);
	if(!node)
		FAILED_TO("allocate loading table node");

	node->rm  = rm;
	node->mod = mod;
	FREE(buf);
	return ins;

 cleanup:
	if(rm && ins)
		rm(ins);
	if(mod)
		(void)dlclose(mod); /* FIXME: should not clean up dlerror() */
	if(buf)
		FREE(buf);
	return NULL;
}

static inline void
uload(void *ins)
{
	struct load_node *node = HPOP(&ltab, ins, struct load_node);
	if(node) {
		if(node->rm)
			node->rm(ins);
		(void)dlclose(node->mod);
		free(node);
	} else
		failed = FNOMOD;
}

#endif /* _LUX_LOAD_H_ */
