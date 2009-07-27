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
#include <stdio.h> /* for sprintf() */
#include <dlfcn.h> /* for dlopen(), dlsym(), and dlclose() */

void *
lux_load(const char *restrict name)
{
	char buf[256]; /* FIXME: check string length */
	void *mod;

	(void)sprintf(buf, LUX_PREFIX "/lib/lux/%s.so", name);
	mod = dlopen(buf, RTLD_LAZY);
	if(mod) {
		void *(*mk)(void);
		void *obj;

		(void)sprintf(buf, "luxC%s", name);
		mk = (void *(*)(void))dlsym(mod, buf);
		if(mk)
			obj = mk();
		else {
			buf[3] = 'E';
			obj = dlsym(mod, buf);
		}

		if(obj)
			return obj;

		(void)dlclose(mod);
	}
	return NULL;
}

void
lux_unload(void *obj)
{
	lux_error("[FIXME] lux_unload(%p): module needs to be freed\n", obj);
}
