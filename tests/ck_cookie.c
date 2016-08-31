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
#include <lux.h>
#include <lux/assert.h>
#include <lux/cookie.h>
#include <stdio.h>
#include <string.h>

#define A(E) lux_always_assert(E)

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
	char out[1024];
	cookie_t cookie = {0, 0, {0}};
	cookie_io_functions_t funcs = {NULL, NULL, NULL, NULL};
	FILE *f;

	funcs.read = read;
	f = fopencookie(&cookie, "w+", funcs);
#if HAVE_FUNOPEN
	A(f == NULL);
	printf("Succeeded skipping read-only stream %p\n", f);
#else
	A(f != NULL);
	printf("Succeeded opening read-only stream %p\n", f);
	fclose(f);
#endif

	funcs.write = write;
	f = fopencookie(&cookie, "w+", funcs);
	A(f != NULL);
	printf("Succeeded opening read-write stream %p\n", f);

	fwrite("abc", 1, 4, f); fflush(f);
	A(strcmp(cookie.data, "abc") == 0);
	printf("Succeeded printing string \"%s\" to stream %p\n", cookie.data, f);

	cookie.current = 0; /* reset position to head */
	fread(out, 1, 4, f);
	A(strcmp(out, "abc") == 0);
	printf("Succeeded reading string \"%s\" from stream %p\n", out, f);

	A(fclose(f) == 0);
	printf("Succeeded closing stream %p\n", f);

	return 0;
}
