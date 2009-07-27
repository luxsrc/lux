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

AH_BOTTOM([/*===========================================================================*/
/* Public API */

extern void lux_print(const char *restrict, ...); /* print message to stdout */

/*===========================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* _LUX_H_ */])

dnl Output files
AC_CONFIG_FILES(m4_esyscmd([find . -name 'Makefile.am' -type f | sed 's/\.\/\(.*\).am/\1/']))
AC_OUTPUT
