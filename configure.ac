dnl Initialization for autoconf, automake, and libtool
AC_PREREQ([2.63])
AC_INIT([lux framework], m4_esyscmd([head -c8 .version]), [ckchan@cfa.harvard.edu], [lux])
AC_CONFIG_AUX_DIR([.build-aux])
AC_CONFIG_MACRO_DIR([.build-m4])
AC_CONFIG_SRCDIR([lux.hin])
LT_PREREQ([2.2.6b])
LT_INIT([dlopen])
AM_INIT_AUTOMAKE([1.11 -Wall -Werror gnu]) dnl cannot use gnits because of version number and the missing "THANKS" file

dnl Checks for programs and compiler characteristics
AC_PROG_CPP
AC_PROG_CC_STDC
AM_PROG_CC_C_O

AC_C_CONST
AC_C_INLINE
AC_C_RESTRICT

AC_PROG_LN_S

dnl Check for headers
AC_CHECK_HEADERS([stddef.h])

dnl Check for types
AC_COMPUTE_INT([ibits], [sizeof(int) * CHAR_BIT], [#include <limits.h>], [AC_MSG_ERROR([failed to compute the size of int in bits])])

dnl Find out the number of bits required to hold all error codes
AC_COMPUTE_INT([elast], [ELAST],       [#include <errno.h>], [
AC_COMPUTE_INT([elast], [_LAST_ERRNO], [#include <errno.h>], [
AC_COMPUTE_INT([elast], [EDQUOT],      [#include <errno.h>], [
AC_COMPUTE_INT([elast], [ERFKILL],     [#include <errno.h>], [
	AC_MSG_ERROR([Failed to obtain the last error code; please describe your platform to <$PACKAGE_BUGREPORT>])
])])])])

if   test $elast -lt  124; then fbits=8
elif test $elast -lt  380; then fbits=9
elif test $elast -lt  892; then fbits=10
elif test $elast -lt 1916; then fbits=11
else
	AC_MSG_ERROR([ELAST == $elast is unexpectedly large; please describe your platform to <$PACKAGE_BUGREPORT>])
fi

xbits=`expr $ibits - $fbits`
if test $xbits -lt 8; then
	AC_MSG_ERROR([$xbits bits are not enough to encode lux-specific failure; please describe your platform to <$PACKAGE_BUGREPORT>])
fi

dnl Customize "config.h" and rename it to "lux.h"
# The `configure` script produces a customized public header "lux.h"
# instead of the standard configuration header "config.h".  This
# header is installed into "$prefix/include" so additional modules,
# schemes, and runs can use it.
AC_CONFIG_HEADERS([lux.h:lux.hin])

AH_TOP([/*
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
#ifndef _LUX_H_
#define _LUX_H_

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================*/])

dnl Define module path
AC_DEFINE_UNQUOTED(LUX_MOD_PATH, "$prefix/lib/lux", [Define to the default module path.])

dnl Define error code related macros
AC_DEFINE_UNQUOTED(LUX_ELAST,       $elast, [Define to the largest system error code.])
AC_DEFINE_UNQUOTED(LUX_FAILURE_BIT, $fbits, [Define to the required bits to hold all lux failure codes.])
AC_DEFINE_UNQUOTED(LUX_INT_BIT,     $ibits, [Define to the size of int in bits.])

dnl Make single-precision a configure option
AC_ARG_ENABLE(single, [AC_HELP_STRING([--enable-single],[compile lux in single precision])], ok=$enableval, ok=no)
AC_ARG_ENABLE(float,  [AC_HELP_STRING([--enable-float], [synonym for --enable-single])],     ok=$enableval)
if test "$ok" = "yes"; then
	AC_DEFINE(LUX_SINGLE, [1], [Define to 1 if single precision is used as the default precision.])
fi
AM_CONDITIONAL(USING_SINGLE, test "$ok" = "yes")

AH_BOTTOM([/*===========================================================================*/
/* Macros for token transformation */

#define LUX_STR1NG(s) #s
#define LUX_STRING(s) LUX_STR1NG(s)

#define LUX_C0NCAT(s, t) s ## t
#define LUX_CONCAT(s, t) LUX_C0NCAT(s, t)

/*---------------------------------------------------------------------------*/
/* Name mangling by passing a cpp-flag "-DLUX_MOD_NAME=name" */

#ifdef LUX_MOD_NAME
#define LUXC LUX_CONCAT(luxC, LUX_MOD_NAME) /* constructor */
#define LUXD LUX_CONCAT(luxD, LUX_MOD_NAME) /* destructor  */
#define LUXE LUX_CONCAT(luxE, LUX_MOD_NAME) /* entry-point */
#endif

/*===========================================================================*/
/* Include a standard header file to get size_t */

#if HAVE_STDDEF_H
#include <stddef.h>
#elif HAVE_SYS_TYPES_H
#include <sys/types.h>
#else
#include <stdio.h>
#endif

/*---------------------------------------------------------------------------*/
/* Typedef numerical types */

#ifdef HAVE_STDDEF_H
typedef size_t    Lux_whole;
typedef ptrdiff_t Lux_int;
#else
typedef unsigned  Lux_whole;
typedef int       Lux_int;
#endif

#ifdef LUX_SINGLE
typedef float  Lux_real;
#else
typedef double Lux_real;
#endif

typedef double Lux_xreal; /* real numbers with possibly extra precision */

typedef struct {Lux_int   n, m;} Lux_rational;
typedef struct {Lux_real  r, i;} Lux_complex;
typedef struct {Lux_xreal r, i;} Lux_xcomplex;

/*===========================================================================*/
/* Public API */
#include <stdarg.h> /* for va_list */

extern void lux_setup(void);
extern void lux_abort(void);

extern void lux_vlog(int, const char *restrict, va_list);

extern void *lux_load(const char *restrict, ...);
extern void  lux_unload(void *);

/*===========================================================================*/
/* Convenience Message Logging Functions */

extern int lux_log_debug, lux_log_print, lux_log_error;

#define LUX_DEF_MSGLOG(level) static inline void    \
	lux_##level(const char *restrict fmt, ...)  \
	{                                           \
		va_list ap;                         \
		va_start(ap, fmt);                  \
		lux_vlog(lux_log_##level, fmt, ap); \
		va_end(ap);                         \
	}

LUX_DEF_MSGLOG(debug)
LUX_DEF_MSGLOG(print)
LUX_DEF_MSGLOG(error)

#undef LUX_DEF_MSGLOG

/*===========================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* _LUX_H_ */])

dnl Output files
AC_CONFIG_FILES(m4_esyscmd([find . -name 'Makefile.am' -type f | sed 's/\.\/\(.*\).am/\1/']))
AC_OUTPUT

# Ugly hack so '#include <lux/interface.h>' works
if [[ ! -e $ac_pwd/lux ]]; then
	$LN_S $srcdir/mod $ac_pwd/lux
fi
