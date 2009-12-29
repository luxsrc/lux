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

#if HAVE_FOPENCOOKIE
/* Use the system fopencookie(); implement nothing */
#elif HAVE_FUNOPEN
/* Implement fopencookie() using funopen()
 *
 * Although BSD came up with funopen() first, it erroneously assumes
 * that fpos_t is an integral type.  If we had decided to make
 * funopen() portable, lux would have needed to create a wrapper
 * function anyway.  Therefore, we use fopencookie() as lux's
 * "standard".  Also, fopencookie() takes a "mode" argument, which
 * makes it more fopen()-like.
 */
#include <stdlib.h> /* for malloc() and free() */

#define W ((struct wrapper *)w)

/* Wrapper is the mapped cookie for funopen() */
struct wrapper {
	void                    *cookie;
	cookie_read_function_t  *read;
	cookie_write_function_t *write;
	cookie_seek_function_t  *seek;
	cookie_close_function_t *close;
};

/* The map_*() functions and mapped I/O functions for funopen() */
static int
map_read(void *w, char *buf, int sz)
{
	return W->read(W->cookie, buf, sz);
}

static int
map_write(void *w, const char *buf, int sz)
{
	return W->write(W->cookie, buf, sz);
}

static fpos_t
map_seek(void *w, fpos_t offset, int whence)
{
	off64_t off = offset; /* assume fpos_t an int when funopen() is used */
	int     err = W->seek(W->cookie, &off, whence);
	return err ? (fpos_t)err : (fpos_t)off;
}

static int
map_close(void *w)
{
	int err = W->close(W->cookie); /* free the actual cookie */
	free(w);                       /* free the cookie mapper */
	return err;
}

FILE *
fopencookie(void *cookie, const char *mode, cookie_io_functions_t iof)
{
	FILE *f;

	struct wrapper *w = (struct wrapper *)malloc(sizeof(struct wrapper));
	if(!w)
		goto cleanup1;

	w->cookie = cookie;
	w->read   = iof.read;
	w->write  = iof.write;
	w->seek   = iof.seek;
	w->close  = iof.close;

	f = funopen(w,
	            w->read  ? map_read  : NULL,
	            w->write ? map_write : NULL,
	            w->seek  ? map_seek  : NULL,
	            w->close ? map_close : NULL);
	if(!f)
		goto cleanup2;

	return f;

 cleanup2:
	free(w); /* free the cookie mapper only */
 cleanup1:
	return NULL;

	(void)mode; /* silence unused variable warning; ignore mode for now */
}
#else
# error neither fopencookie() nor funopen() are found;
# error no way to provide fopencookie().
#endif
