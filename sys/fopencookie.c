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

#if HAVE_FOPENCOOKIE
/* Use the system fopencookie(); implement nothing */
#elif HAVE_FUNOPEN
/* Implement fopencookie() using funopen() */
#include <stdlib.h> /* for malloc() and free() */

#define W ((struct wrapper *)w)

#define READ   (1U << 0)
#define WRITE  (1U << 1)
#define APPEND (1U << 2)
#define BINARY (1U << 3) /* not used in Unux/Linux */
#define RDWR   (READ|WRITE)

#define SET(f, F) (((f) & (F)) == (F)) /* check if F is set or not; work even
                                          when F is not a single bit */
static inline unsigned
mode2flags(const char *mode)
{
	unsigned flags = 0;

	if(mode)
		switch(mode[0]) {
		case 'r': flags = READ;         break;
		case 'w': flags = WRITE;        break;
		case 'a': flags = WRITE|APPEND; break;
		}

	while(flags && *++mode)
		switch(mode[0]) {
		case '+': flags = SET(flags, RDWR)   ? 0 : flags|RDWR;   break;
		case 'b': flags = SET(flags, BINARY) ? 0 : flags|BINARY; break;
		default : flags = 0;                                     break;
		}

	return flags;
}

/* This internal wrapper is the mapper cookie for funopen() */
struct wrapper {
	void                    *cookie;
	cookie_read_function_t  *read;
	cookie_write_function_t *write;
	cookie_seek_function_t  *seek;
	cookie_close_function_t *close;
};

/* Mapper I/O functions for funopen().  No internal checking is needed
   for {read,write,seek}fn() because they are only passed to funopen()
   if the corresponding functions exist.  However, internal checking
   of W->close is needed in closefn_safe() because it is always passed
   to funopen() to free the mapper cookie. */
static int
readfn(void *w, char *buf, int sz)
{
	return W->read(W->cookie, buf, sz);
}

static int
writefn(void *w, const char *buf, int sz)
{
	return W->write(W->cookie, buf, sz);
}

static fpos_t
seekfn(void *w, fpos_t offset, int whence)
{
	off64_t off = offset; /* assume fpos_t an int when funopen() is used */
	int     err = W->seek(W->cookie, &off, whence);
	return  err ? (fpos_t)err : (fpos_t)off;
}

static int
closefn_safe(void *w)
{
	int err = W->close ? W->close(W->cookie) : 0;
	free(w); /* always free the cookie mapper to avoid memory leakage */
	return err;
}

FILE *
fopencookie(void *cookie, const char *mode, cookie_io_functions_t iof)
{
	unsigned flags;
	struct wrapper *w;
	FILE *f;

	/* Convert mode to flags */
	flags = mode2flags(mode);
	if(!flags)
		goto cleanup1;

	/* Make sure that the required functions are available */
	if((flags & READ   && !iof.read )||
	   (flags & WRITE  && !iof.write)||
	   (flags & APPEND && !iof.seek ))
		goto cleanup1;

	/* Allocate and initialize a wrapper */
	w = (struct wrapper *)malloc(sizeof(struct wrapper));
	if(!w)
		goto cleanup1;

	w->cookie = cookie;
	w->read   = iof.read;
	w->write  = iof.write;
	w->seek   = iof.seek;
	w->close  = iof.close;

	/* Use funopen() to create a stream; go to the end if in append mode */
	f = funopen(w,
	            flags & READ  ? readfn  : NULL,
	            flags & WRITE ? writefn : NULL,
	            w->seek       ? seekfn  : NULL,
	            closefn_safe); /* always pass closefn_safe() */
	if(!f)
		goto cleanup2;

	if(flags & APPEND && fseek(f, 0L, SEEK_END) < 0) {
		(void)fclose(f);
		goto cleanup1; /* fclose() calls closefn_safe(), which frees
		                  the cookie; no need to use free() again */
	}
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
