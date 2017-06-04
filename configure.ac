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

dnl Check for functions
AC_SEARCH_LIBS([dladdr],  [dl], [AC_DEFINE(HAVE_DLADDR,  1, [Define to 1 if you have the `dladdr' function.])],      [AC_MSG_WARN([failed to find the dladdr() function])]     )
AC_SEARCH_LIBS([dlinfo],  [dl], [AC_DEFINE(HAVE_DLINFO,  1, [Define to 1 if you have the `dlinfo' function.])],      [AC_MSG_WARN([failed to find the dlinfo() function])]     )
AC_SEARCH_LIBS([dlvsym],  [dl], [AC_DEFINE(HAVE_DLVSYM,  1, [Define to 1 if you have the `dlvsym' function.])],      [AC_MSG_WARN([failed to find the dlvsym() function])]     )
AC_SEARCH_LIBS([dlmopen], [dl], [AC_DEFINE(HAVE_DLMOPEN, 1, [Define to 1 if you have the GNU `dlmopen' function.])], [AC_MSG_WARN([failed to find the GNU dlmopen() function])])
AC_SEARCH_LIBS([fdlopen], [dl], [AC_DEFINE(HAVE_FDLOPEN, 1, [Define to 1 if you have the BSD `fdlopen' function.])], [AC_MSG_WARN([failed to find the BSD fdlopen() function])])
AC_SEARCH_LIBS([dlfunc],  [dl], [AC_DEFINE(HAVE_DLFUNC,  1, [Define to 1 if you have the BSD `dlfunc' function.])],  [AC_MSG_WARN([failed to find the BSD dlfunc() function])] )

dnl Find out the number of bits in int types
AC_COMPUTE_INT([cbits], [                    CHAR_BIT], [#include <limits.h>], [AC_MSG_ERROR([failed to compute the size of char in bits])])
AC_COMPUTE_INT([ubits], [sizeof(unsigned)  * CHAR_BIT], [#include <limits.h>], [AC_MSG_ERROR([failed to compute the size of unsigned in bits])])
AC_COMPUTE_INT([ibits], [sizeof(int)       * CHAR_BIT], [#include <limits.h>], [AC_MSG_ERROR([failed to compute the size of int in bits])])
AC_COMPUTE_INT([sbits], [sizeof(size_t)    * CHAR_BIT], [#include <limits.h>
#include <stdlib.h>], [AC_MSG_ERROR([failed to compute the size of size_t in bits])])
AC_COMPUTE_INT([pbits], [sizeof(ptrdiff_t) * CHAR_BIT], [#include <limits.h>
#include <stddef.h>], [AC_MSG_ERROR([failed to compute the size of ptrdiff_t in bits])])

dnl Find out the largest errno/exception
AC_COMPUTE_INT([elast],  [ELAST],     [#include <errno.h>], [
AC_COMPUTE_INT([elast],  [EHWPOISON], [#include <errno.h>], [
AC_COMPUTE_INT([elast],  [ERFKILL],   [#include <errno.h>], [
	AC_MSG_ERROR([Failed to obtain the last error code; please describe your platform to <$PACKAGE_BUGREPORT>])
])])
AC_COMPUTE_INT([edquot], [EDQUOT],    [#include <errno.h>], [edquot=0])
if test $elast -le $edquot; then elast=$edquot; fi
])

dnl Find out the number of bits required to hold all error codes
if   test $elast -le  133; then ebits=8  # EHWPOISON == 133 on generic architectures
elif test $elast -le  389; then ebits=9  # EHWPOISON == 135, 139, 168, 257 on SPARC, Alpha, MIPS, and PA-RISC, respectively
elif test $elast -le  901; then ebits=10 # No known system
elif test $elast -le 1925; then ebits=11 # EDQUOT == 1133 on MIPS
else
	AC_MSG_ERROR([LUX_ELAST == $elast is unexpectedly large; please report your platform to <$PACKAGE_BUGREPORT>])
fi

mbits=`expr \( $ibits - $ebits \) / 4 + 4`
if test $mbits -lt 6; then
	AC_MSG_ERROR([LUX_MESSAGE_BIT == $mbits is unexpectedly small; please report your platform to <$PACKAGE_BUGREPORT>])
fi

fbits=`expr $ibits - $mbits - $ebits`
if test $fbits -lt 2; then
	AC_MSG_ERROR([LUX_FACILITY_BIT == $fbits is unexpectedly small; please report your platform to <$PACKAGE_BUGREPORT>])
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

AC_DEFINE_UNQUOTED(LUX_CHAR_BIT,      $cbits, [Define to the number of bits of char.])
AC_DEFINE_UNQUOTED(LUX_INT_BIT,       $ibits, [Define to the number of bits of int.])
AC_DEFINE_UNQUOTED(LUX_UNSIGNED_BIT,  $ubits, [Define to the number of bits of unsigned.])
AC_DEFINE_UNQUOTED(LUX_SIZE_T_BIT,    $sbits, [Define to the number of bits of size_t.])
AC_DEFINE_UNQUOTED(LUX_PTRDIFF_T_BIT, $pbits, [Define to the number of bits of ptrdiff_t.])

AC_DEFINE_UNQUOTED(LUX_ELAST,         $elast, [Define to the largest system error code.])
AC_DEFINE_UNQUOTED(LUX_FACILITY_BIT,  $fbits, [Define to the number of required bits to hold a facility id.])
AC_DEFINE_UNQUOTED(LUX_MESSAGE_BIT,   $mbits, [Define to the number of required bits to hold a message id.])
AC_DEFINE_UNQUOTED(LUX_EXCEPTION_BIT, $ebits, [Define to the number of required bits to hold a lux exception code.])

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
