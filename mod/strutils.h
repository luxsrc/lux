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
#ifndef _LUX_STRUTILS_H_
#define _LUX_STRUTILS_H_

#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	size_t l;
	char  *s;
} pstr_t; /* pascal string */

#define pstr(s) {sizeof(s)-1, s}

static inline char *
alnumcat(char *d, const char *s)
{
	char *t = d + strlen(d);
	while(isalnum(*s))
		*t++ = *s++;
	return d;
}

#define basename(x) _basename(x)
static inline const char *
_basename(const char *s)
{
	const char *h = s;
	while(*h)
		if('/' == *h++)
			s = h;
	return s;
}

static inline double *
strtoda(const char *str, char **endptr)
{
	size_t  c  = 8;
	size_t  n  = 0;
	double *da = malloc(sizeof(double) * c);
	char   *end;

	for(;;) {
		double d = strtod(str, &end);
		if(*end == '_' || *end == ',' || *end == ';')
			++end;
		if(str == end)
			break;
		da[n++] = d;
		if(n == c) {
			c *= 2;
			da = realloc(da, sizeof(double) * c);
		}
		str = end;
	}

	da[n++] = NAN; /* end of array */
	*endptr = end;
	return da;
}

#endif /* _LUX_STRUTILS_H_ */
