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
#ifndef _LUX_DPATH_H_
#define _LUX_DPATH_H_

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

static int
isdir(const char *path)
{
	struct stat s;
	return stat(path, &s) ? 0 : S_ISDIR(s.st_mode);
}

static const char *
dpath(const char *name)
/* Given a module name "name", construct a colon seperated string of
   submodule names that can be load by lux_load() */
{
	const char *mod = NULL;
	size_t nlen = name ? strlen(name) : 0;

	if(name && name[0] == '/') {
		if(isdir(name))
			mod = strdup(name);
	} else {
		char *paths = strdup(lux_mpaths());
		const char *p;
		for(p = strtok(paths, ":"); p; p = strtok(NULL, ":")) {
			if(name) {
				size_t plen = strlen(p);
				char  *path = malloc(plen + nlen + sizeof("/"));
				(void)memcpy(path, p, plen);
				path[plen] = '/';
				(void)memcpy(path + plen + 1, name, nlen);
				path[plen + nlen + 1] = '\0';
				if(isdir(path)) {
					mod = path;
					break;
				}
				free((void *)path);
			} else {
				if(isdir(p))
					mod = strdup(p);
			}
		}
		free(paths);
	}
	return mod;
}

static const char *
dsubmods(const char *path)
/* List submoduels that can be loaded by lux_load() */
{
	size_t sz   = 0;
	char  *mods = NULL;

	DIR *d = opendir(path); /* assume path is valid dir */

	struct dirent *e;
	while((e = readdir(d))) {
		int slen = (int)strlen(e->d_name) - 3;
		if(slen >= 0 && !strcmp(e->d_name + slen, ".la")) {
			size_t newsz = sz + slen + 1;
			mods = realloc(mods, newsz);
			if(sz && mods)
				mods[sz-1] = ':';
			(void)memcpy(mods + sz, e->d_name, slen);
			mods[newsz-1] = '\0';
			sz = newsz;
		}
	}

	closedir(d);

	return mods;
}

#endif /* _LUX_DMOD_H_ */
