/*
 * Copyright (C) 2010 Chi-kwan Chan
 * Copyright (C) 2010 Harvard-Smithsonian Center for Astrophysics
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
#ifndef LUX_ASSERTION
#define LUX_ASSERTION 1
#endif

#include <lux.h>
#include <lux/assert.h>
#include <lux/cookie.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	size_t current, count;
	char data[1024];
} cookie_t;

static ssize_t
read(void *c, char *buf, size_t sz)
{
	char *data = ((cookie_t *)c)->data;

	size_t i = 0;
	size_t j = ((cookie_t *)c)->current;
	size_t n = ((cookie_t *)c)->count;
	for(; i < sz && j < n; ++i, ++j)
		buf[i] = data[j%1024];

	((cookie_t *)c)->current = j;
	return i;
}

static ssize_t
write(void *c, const char *buf, size_t sz)
{
	char  *data = ((cookie_t *)c)->data;

	size_t i =  0;
	size_t j = ((cookie_t *)c)->current;
	for(; i < sz; ++i, ++j)
		data[j%1024] = buf[i];

	((cookie_t *)c)->current = j;
	((cookie_t *)c)->count   = j;
	return i;
}

int
main()
{
	char out[1024] = "123";
	cookie_t cookie = {0};
	cookie_io_functions_t funcs = {0};
	FILE *f;

	funcs.read = read;
	f = fopencookie(&cookie, "w+", funcs);
#if HAVE_FUNOPEN
	lux_assert(f == NULL);
#else
	lux_assert(f != NULL);
	fclose(f);
#endif

	funcs.write = write;
	f = fopencookie(&cookie, "w+", funcs);
	lux_assert(f != NULL);

	fwrite("abc", 1, 4, f); fflush(f);
	lux_assert(strcmp(cookie.data, "abc") == 0);

	fread(out, 1, 4, f);
	lux_assert(strcmp(out, "123") == 0);

	lux_assert(fclose(f) == 0);

	return 0;
}
