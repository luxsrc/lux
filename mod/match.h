/*
 * Copyright (C) 2011 Chi-kwan Chan
 * Copyright (C) 2011 Nordic Institute for Theoretical Physics
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
#ifndef _LUX_MATCH_H_
#define _LUX_MATCH_H_

#include <string.h>
#if HAVE_CTYPE_H
#include <ctype.h>
#else
#include <string.h>
static inline int
isspace(int c)
{
	return c && strchr(" \t\n\v\f\r", c); /* '\0' is not white space */
}
#endif

static inline const char *
skipfnc(const char *a, int (*iscls)(int))
{
	while(iscls(*a))
		++a;
	return a;
}

static inline const char *
skipstr(const char *a, const char *s)
{
	while(*s && *s != ':') {
		if(*s != *a)
			return NULL;
		++s;
		++a;
	}
	return a;
}

static inline const char *
next(const char *s)
{
	s = strchr(s, ':');
	return s ? s + 1 : NULL;
}

static inline const char *
match(const char *sym, const char *arg)
{
	const char *a = arg;
	if((a = skipfnc(a, isspace)) &&
	   (a = skipstr(a, sym    )) &&
	   (a = skipfnc(a, isspace)) &&
	   (a = skipstr(a, "="    )) &&
	   (a = skipfnc(a, isspace)))
		return a;

	sym = next(sym);
	if(sym)
		return match(sym, arg); /* recursion */

	return NULL;
}

#endif /* _LUX_MATCH_H_ */
