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
#ifndef _LUX_COOKIE_H_
#define _LUX_COOKIE_H_

#if HAVE_FOPENCOOKIE
/* Use the system fopencookie() in <stdio.h>; declare nothing */
# define _GNU_SOURCE
# include <stdio.h>
#elif HAVE_FUNOPEN
/* Declare fopencookie() and related types
 *
 * Although BSD came up with funopen() first, it erroneously assumes
 * that fpos_t is an integral type.  If we had decided to make
 * funopen() portable, lux would have needed to create a wrapper
 * function anyway.  Therefore, we use fopencookie() as lux's
 * "standard".  Also, fopencookie() takes a "mode" argument, which
 * makes it more fopen()-like.
 */
# include <stdio.h>
# if HAVE_OFF64_T
#  if HAVE_SYS_TYPES_H
#   include <sys/types.h> /* for off64_t */
#  elif HAVE_UNISTD_H
#   include <unistd.h> /* for off64_t */
#  else
#   error neither <sys/types.h> nor <unistd.h> are found;
#   error please report definition of off64_t to <ckchan@cfa.harvard.edu>
#  endif
# else
#  if HAVE_STDINT_H
#   include <stdint.h> /* for int64_t */
    typedef int64_t off64_t;
#  else
    typedef long long off64_t; /* long long is at least 64 bits in size */
#  endif
# endif

typedef ssize_t (cookie_read_function_t )(void *,       char *, size_t);
typedef ssize_t (cookie_write_function_t)(void *, const char *, size_t);
typedef int     (cookie_seek_function_t )(void *,    off64_t *, int);
typedef int     (cookie_close_function_t)(void *);

typedef struct {
	cookie_read_function_t  *read;
	cookie_write_function_t *write;
	cookie_seek_function_t  *seek;
	cookie_close_function_t *close;
} cookie_io_functions_t;

extern FILE *fopencookie(void *, const char *, cookie_io_functions_t);

#else
# error neither fopencookie() nor funopen() are found;
# error no way to provide fopencookie().
#endif

#endif /* _LUX_COOKIE_H_ */
