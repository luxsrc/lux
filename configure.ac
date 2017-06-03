dnl Initialization for autoconf
AC_PREREQ([2.64])
AC_INIT([lux framework], m4_esyscmd([tr -d '\n' < .version]), [ckchan@cfa.harvard.edu], [lux], [http://luxsrc.org/])
AC_CONFIG_AUX_DIR([.build-aux])
AC_CONFIG_SRCDIR([lux.hin])

dnl Initialization for automake
AM_INIT_AUTOMAKE([1.11.6 -Wall -Werror gnu nostdinc]) dnl cannot use gnits because of version number and the missing "THANKS" file
AM_PROG_AR

dnl Check for programs and compiler characteristics
AC_PROG_LN_S

dnl Customize "config.h" and rename it to "lux.h"
# The `configure` script produces a customized public header "lux.h"
# instead of the standard configuration header "config.h".  This
# header is installed into "$prefix/include" to allow additional
# modules to be compiled using the exact same configuration as lux
# without rerunning `configure`.
AC_CONFIG_HEADERS([lux.h:lux.hin])

dnl Output files
AC_CONFIG_FILES(m4_esyscmd([find . -name 'Makefile.am' -type f | sed 's|\./\(.*\).am|\1|']))
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
