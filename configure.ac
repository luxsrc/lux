dnl Initialization for autoconf
AC_PREREQ([2.64])
AC_INIT([lux framework], m4_esyscmd([tr -d '\n' < .version]), [ckchan@cfa.harvard.edu], [lux], [http://luxsrc.org/])
AC_CONFIG_AUX_DIR([.build-aux])
AC_CONFIG_MACRO_DIR([.build-m4])
AC_CONFIG_SRCDIR([lux.hin])

dnl Initialization for automake
AM_INIT_AUTOMAKE([1.11.6 -Wall -Werror gnu nostdinc]) dnl cannot use gnits because of version number and the missing "THANKS" file
AM_PROG_AR

dnl Check for programs and compiler characteristics
AC_PROG_LN_S

dnl Initialization for libtool
LT_PREREQ([2.2.10])
LT_INIT([dlopen disable-static]) dnl must be call after AM_PROG_AR

dnl Find out the number of bits required to hold all error codes
AC_COMPUTE_INT([elast], [ELAST],       [#include <errno.h>], [
AC_COMPUTE_INT([elast], [_LAST_ERRNO], [#include <errno.h>], [
AC_COMPUTE_INT([elast], [EDQUOT],      [#include <errno.h>], [
AC_COMPUTE_INT([elast], [ERFKILL],     [#include <errno.h>], [
	AC_MSG_ERROR([Failed to obtain the last error code; please describe your platform to <$PACKAGE_BUGREPORT>])
])])])])

if   test $elast -le  132; then ebits=8  # ERFKILL == 132 on generic architectures
elif test $elast -le  388; then ebits=9  # ERFKILL == 134, 138, and 167 on SPARC, Alpha, and MIPS, respectively
elif test $elast -le  900; then ebits=10 # _LAST_ERRNO ==  516 on PowerPC
elif test $elast -le 1924; then ebits=11 # EDQUOT == 1133 on MIPS
else
	AC_MSG_ERROR([ELAST == $elast is unexpectedly large; please describe your platform to <$PACKAGE_BUGREPORT>])
fi

dnl Customize "config.h" and rename it to "lux.h"
# The `configure` script produces a customized public header "lux.h"
# instead of the standard configuration header "config.h".  This
# header is installed into "$prefix/include" to allow additional
# modules to be compiled using the exact same configuration as lux
# without rerunning `configure`.
AC_CONFIG_HEADERS([lux.h:lux.hin])

AH_TOP([/*
 * Copyright 2017 Chi-kwan Chan
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _LUX_H_
#define _LUX_H_

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================*/])

AC_DEFINE_UNQUOTED(LUX_ELAST,         $elast, [Define to the largest system error code.])
AC_DEFINE_UNQUOTED(LUX_EXCEPTION_BIT, $ebits, [Define to the required bits to hold a lux exception code.])

AH_BOTTOM([/*===========================================================================*/

void lux_info(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* _LUX_H_ */])

dnl Output files
AC_CONFIG_FILES(m4_esyscmd([find . -name 'Makefile.am'                 -type f | sed 's|\./\(.*\).am|\1|'    ]))
AC_CONFIG_FILES(m4_esyscmd([find . -name '*.?*in' -not -name 'lux.hin' -type f | sed 's|\./\(.*\)in|\1:\1in|']))
AC_OUTPUT

dnl Hack: in terms of source and build paths, there are only two
dnl variables "$srcdir" and "$ac_pwd" that are accessible within
dnl `configure`.  All other path variables, e.g., "$top_srcdir", are
dnl empty within `configure`.  Since "$ac_pwd" is always ".", the only
dnl useful variable is "$srcdir".
# Ugly hack so '#include <lux/interface.h>' works
if [[ ! -e lux ]]; then
	$LN_S $srcdir/mod lux
fi
