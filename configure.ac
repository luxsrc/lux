dnl Initialization for autoconf and automake
AC_PREREQ(2.63)
AC_INIT([lux framework], m4_esyscmd([git rev-parse HEAD | head -c8]), [ckchan@cfa.harvard.edu], [lux])
AM_INIT_AUTOMAKE([1.11 -Wall -Werror gnu]) dnl cannot use gnits because of version number and the missing "THANKS" file

dnl Output files
AC_CONFIG_FILES(m4_esyscmd([find . -name 'Makefile.am' -type f | sed 's/\.\/\(.*\).am/\1/']))
AC_OUTPUT
