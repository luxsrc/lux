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
#include <lux/cookie.h>
#include <string.h> /* for strcmp() */

#if HAVE_FOPENCOOKIE
/* Use the system fopencookie(); implement nothing */
#elif HAVE_FUNOPEN
/* Implement fopencookie() using funopen() */
#include <stdlib.h> /* for malloc() and free() */

#define W           ((struct wrapper *)w)
#define READ        (1U << 0)
#define WRITE       (1U << 1)
#define SEEK        (1U << 2)
#define MATCH(a, b) (!strcmp(a, b))

/* This internal wrapper is the mapped cookie for funopen() */
struct wrapper {
	void                    *cookie;
	cookie_read_function_t  *read;
	cookie_write_function_t *write;
	cookie_seek_function_t  *seek;
	cookie_close_function_t *close;
	unsigned                 flags;
};

/* The map_*() functions and mapped I/O functions for funopen() */
static int
map_read(void *w, char *buf, int sz)
{
	return W->flags & READ ? W->read(W->cookie, buf, sz) : -1;
	/* TODO: set errno or failed */
}

static int
map_write(void *w, const char *buf, int sz)
{
	return W->flags & WRITE ? W->write(W->cookie, buf, sz) : -1;
	/* TODO: set errno or failed */
}

static fpos_t
map_seek(void *w, fpos_t offset, int whence)
{
	off64_t off = offset; /* assume fpos_t an int when funopen() is used */
	int     err = W->flags & SEEK ? W->seek(W->cookie, &off, whence) : -1;
	return  err ? (fpos_t)err : (fpos_t)off;
	/* TODO: set errno or failed; check overflow */
}

static int
map_close(void *w)
{
	int err = W->close ? W->close(W->cookie) : 0;
	free(w); /* always free the cookie mapper to avoid memory leakage */
	return err;
}

FILE *
fopencookie(void *cookie, const char *mode, cookie_io_functions_t iof)
{
	struct wrapper *w;
	FILE *f;
	unsigned flags;

	/* Check mode[]; FIXME: how to use SEEK? */
	if(!mode)
		goto cleanup1;

	switch(mode[0]) {
	case 'r': flags = READ;  break;
	case 'w': /* fall-thought */
	case 'a': flags = WRITE; break;
	default : goto cleanup1;
	}

	     if(MATCH(mode+1, ""  )||
	        MATCH(mode+1, "b" ) ) /* do nothing */;
	else if(MATCH(mode+1, "+" )||
	        MATCH(mode+1, "+b")||
	        MATCH(mode+1, "b+") ) flags |= (READ | WRITE);
	else                          goto cleanup1;

	/* mode[] is valid; allocate and initialize a wrapper */
	w = (struct wrapper *)malloc(sizeof(struct wrapper));
	if(!w)
		goto cleanup1;

	w->cookie = cookie;
	w->read   = iof.read;
	w->write  = iof.write;
	w->seek   = iof.seek;
	w->close  = iof.close;
	w->flags  = flags;

	/* Use funopen() to create a stream */
	f = funopen(w,
	            w->read  ? map_read  : NULL,
	            w->write ? map_write : NULL,
	            w->seek  ? map_seek  : NULL,
	            map_close); /* always pass map_close() */
	if(!f)
		goto cleanup2;

	return f;

 cleanup2:
	free(w); /* free the cookie mapper only */
 cleanup1:
	return NULL;
}
#else
# error neither fopencookie() nor funopen() are found;
# error no way to implement fopencookie().
#endif
