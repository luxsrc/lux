dnl Initialization for autoconf
AC_PREREQ([2.64])
AC_INIT([lux framework], m4_esyscmd([tr -d '\n' < .version]), [ckchan@cfa.harvard.edu], [lux], [http://luxsrc.org/])
AC_CONFIG_AUX_DIR([.build-aux])
AC_CONFIG_SRCDIR([lux.hin])

dnl Initialization for automake
AM_INIT_AUTOMAKE([1.11.6 -Wall -Werror gnu nostdinc]) dnl cannot use gnits because of version number and the missing "THANKS" file
AM_PROG_AR

dnl Check for programs and compiler characteristics
AC_PROG_RANLIB
AC_PROG_LN_S

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

AH_BOTTOM([/*===========================================================================*/

extern void lux_print(const char *, ...);

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
