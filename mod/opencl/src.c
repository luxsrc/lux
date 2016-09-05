/*
 * Copyright (C) 2016 Chi-kwan Chan
 * Copyright (C) 2016 Steward Observatory
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
#include <string.h>
#include "mod.h"

static FILE *
ftryopen(const char *name)
{
	const char *fmt[] = {"%s", "%s.cl"};

	size_t i;
	for(i = 0; i < countof(fmt); ++i) {
		FILE *f;
		char  buf[1024];
		sprintf(buf, fmt[i], name);

		f = fopen(buf, "r");
		if(f) {
			lux_print("Loaded kernel \"%s\"\n", buf);
			return f;
		}
	}
	return NULL;
}

static const char *
freadall(FILE *f)
{
	size_t l;
	char  *s;

	fseek(f, 0, SEEK_END);
	l = ftell(f);
	fseek(f, 0, SEEK_SET);

	s = malloc(l+1);
	fread(s, l, 1, f);
	s[l] = '\0';

	return s;
}

const char *
getsrc(const char *path, const char *name)
{
	char buf[1024];

	/* Always try to open without path */
	FILE *f = ftryopen(name);
	if(name[0] == '/' || /* done trying if aboslute or */
	   name[0] == '.')   /* explicit relative path     */
		goto nopath;

	if(!f) {
		/* Try to open with an associated directory named as
		   the base module */
		const char *sep = strrchr(path, '.');
		if(sep) {
			size_t l = sep - path;
			memcpy(buf, path, l);
			buf[l] = '/';
			strcpy(buf+l+1, name);
			f = ftryopen(buf);
		}
	}

	if(!f) {
		/* Try to open with at the same directory that
		   contains the base module */
		const char *sep = strrchr(path, '/');
		if(sep) {
			size_t l = sep - path;
			memcpy(buf, path, l);
			buf[l] = '/';
			strcpy(buf+l+1, name);
			f = ftryopen(buf);
		}
	}

 nopath:
	if(!f)
		return NULL;
	else {
		const char *s = freadall(f);
		fclose(f);
		return s;
	}
}
