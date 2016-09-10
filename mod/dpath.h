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

#endif /* _LUX_DMOD_H_ */
