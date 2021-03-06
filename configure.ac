dnl Initialization for autoconf
AC_PREREQ([2.64])
AC_INIT([lux framework], m4_esyscmd([tr -d '\n' < .version]), [chanc@email.arizona.edu], [lux], [http://luxsrc.org/])
AC_CONFIG_AUX_DIR([.build-aux])
AC_CONFIG_MACRO_DIR([.build-m4])
AC_CONFIG_SRCDIR([lux.hin])

dnl First save manually set environmental variables
test -z "${MPICC+x}"  || manual_MPICC="$MPICC"
test -z "${CC+x}"     || manual_CC="$CC"
test -z "${CFLAGS+x}" || manual_CFLAGS="$CFLAGS"

dnl Determine default compiler
AC_PROG_CC

dnl Initialization for automake libtool
AM_INIT_AUTOMAKE([1.11.6 -Wall -Werror gnu nostdinc]) dnl cannot use gnits because of version number and the missing "THANKS" file
AM_PROG_AR dnl Must be called after setting manual_CC

dnl Make HDF5 a configure option (default auto)
AX_LIB_HDF5
if test "$with_hdf5" = "yes"; then
	if test $(basename "$H5CC") == h5cc; then
		dnl Series HDF5: update CC if it was not set manually
		if test -z "${manual_CC+x}"; then
			test -z "${manual_CFLAGS+x}" && unset CFLAGS
			CC="$HDF5_CC"
			AC_PROG_CC(["$CC"])
		fi
	else
		dnl Parallel HDF5: update MPICC if it was not set manually
		test -z "${manual_MPICC+x}" && MPICC="$HDF5_CC"
	fi
fi
AM_CONDITIONAL(USING_HDF5, test "$with_hdf5" = "yes")

dnl Make MPI a configure option (default auto)
AX_LIB_MPI
if test "$with_mpi" = "yes"; then
	dnl Update CC if it was determined automatically
	if test -z "${manual_CC+x}"; then
		test -z "${manual_CFLAGS+x}" && unset CFLAGS
		CC="$MPI_CC"
		AC_PROG_CC(["$CC"])
	fi
fi
AM_CONDITIONAL(USING_MPI, [test "$with_mpi" = "yes"])

dnl Make OpenCL a configure option (default auto)
AX_LIB_OPENCL
AM_CONDITIONAL(USING_OPENCL, [test "$with_opencl" = "yes"])

dnl Check if we may use OpenMP
AX_OPENMP([with_openmp=yes], [with_openmp=no])
AM_CONDITIONAL(USING_OPENMP, [test "$with_openmp" = "yes"])

dnl Check if we may use pthread
AX_PTHREAD([with_pthread=yes], [with_pthread=no]) # may cause problem on AIX; see comments in "tools/m4/ax_pthread.m4"
if test "$with_pthread" = "yes"; then
	# Enable pthread-capable compiler by default
	test -z "${manual_CFLAGS+x}" && unset CFLAGS
	CC="${CC:-PTHREAD_CC}"
	AC_PROG_CC(["$CC"])
fi
AM_CONDITIONAL(USING_PTHREAD, [test "$with_pthread" = "yes"])

dnl Initialization for libtool
LT_PREREQ([2.2.10])
LT_INIT([dlopen disable-static]) dnl must be call after AM_PROG_AR

dnl Check for programs and compiler characteristics
AC_PROG_CPP
AC_PROG_CC_STDC
AM_PROG_CC_C_O

AC_C_TYPEOF
AC_C_CONST
AC_C_INLINE
AC_C_RESTRICT

AC_PROG_LN_S

dnl Check for headers
AC_HEADER_TIME
AC_CHECK_HEADERS([sys/time.h c_asm.h intrinsics.h mach/mach_time.h])
AC_CHECK_HEADERS([stddef.h stdint.h inttypes.h ctype.h obstack.h sys/mman.h sys/stat.h fcntl.h])

dnl Check for types
AC_CHECK_TYPE([off64_t])
AC_CHECK_TYPE([hrtime_t], [AC_DEFINE(HAVE_HRTIME_T, 1, [Define to 1 if hrtime_t is defined in <sys/time.h>])], , [#if HAVE_SYS_TIME_H
#include <sys/time.h>
#endif])

dnl Check for functions
AC_FUNC_ALLOCA
AC_CHECK_FUNCS([BSDgettimeofday gettimeofday gethrtime read_real_time time_base_to_time clock_gettime mach_absolute_time])
AC_SEARCH_LIBS([dlinfo],  [dl], [AC_DEFINE(HAVE_DLINFO,  1, [Define to 1 if you have the `dlinfo' function.])],  [AC_MSG_WARN([failed to find the dlinfo() function])])
AC_SEARCH_LIBS([dladdr],  [dl], [AC_DEFINE(HAVE_DLADDR,  1, [Define to 1 if you have the `dladdr' function.])],  [AC_MSG_WARN([failed to find the dladdr() function])])
AC_SEARCH_LIBS([dlmopen], [dl], [AC_DEFINE(HAVE_DLMOPEN, 1, [Define to 1 if you have the `dlmopen' function.])], [AC_MSG_WARN([failed to find the dlmopen() function])])
AC_CHECK_FUNCS([fopencookie funopen vsnprintf memfd_create shm_open shm_unlink])

dnl Check for libraries
AC_CHECK_LIB([rt], [shm_open])

dnl Cray UNICOS _rtc() (real-time clock) intrinsic
AC_MSG_CHECKING([for _rtc intrinsic])
rtc_ok=yes
AC_TRY_LINK([#if HAVE_INTRINSICS_H
#include <intrinsics.h>
#endif], [_rtc()], [AC_DEFINE(HAVE__RTC, 1, [Define if you have the UNICOS _rtc() intrinsic.])], [rtc_ok=no])
AC_MSG_RESULT($rtc_ok)

dnl Find out the number of bits in integer types
AC_COMPUTE_INT([cbits], [                   CHAR_BIT], [#include <limits.h>], [AC_MSG_ERROR([failed to compute the size of int in bits])])
AC_COMPUTE_INT([ibits], [sizeof(int)      * CHAR_BIT], [#include <limits.h>], [AC_MSG_ERROR([failed to compute the size of int in bits])])
AC_COMPUTE_INT([ubits], [sizeof(unsigned) * CHAR_BIT], [#include <limits.h>], [AC_MSG_ERROR([failed to compute the size of unsigned in bits])])
AC_COMPUTE_INT([sbits], [sizeof(size_t)   * CHAR_BIT], [#include <limits.h>
#include <stdlib.h>], [AC_MSG_ERROR([failed to compute the size of size_t in bits])])

dnl Find out the number of bits required to hold all error codes
AC_COMPUTE_INT([elast], [ELAST],       [#include <errno.h>], [
AC_COMPUTE_INT([elast], [_LAST_ERRNO], [#include <errno.h>], [
AC_COMPUTE_INT([elast], [EDQUOT],      [#include <errno.h>], [
AC_COMPUTE_INT([elast], [ERFKILL],     [#include <errno.h>], [
	AC_MSG_ERROR([Failed to obtain the last error code; please describe your platform to <$PACKAGE_BUGREPORT>])
])])])])

if   test $elast -le  132; then fbits=8  # ERFKILL == 132 on generic architectures
elif test $elast -le  388; then fbits=9  # ERFKILL == 134, 138, and 167 on SPARC, Alpha, and MIPS, respectively
elif test $elast -le  900; then fbits=10 # _LAST_ERRNO ==  516 on PowerPC
elif test $elast -le 1924; then fbits=11 # EDQUOT == 1133 on MIPS
else
	AC_MSG_ERROR([ELAST == $elast is unexpectedly large; please describe your platform to <$PACKAGE_BUGREPORT>])
fi

hbits=`expr $ubits / 2`
if test $fbits -gt $hbits; then
	fbits=$hbits
fi
mbits=`expr $ubits - $hbits - $fbits` # works properly even if $ubits is odd, although C standard ensures that $ubits is 16, 32, or 64

dnl Find out the number of bits required to hold array dimensions
if   test $sbits -ge 64; then dbits=59 # at most rank 32
elif test $sbits -ge 32; then dbits=28 # at most rank 16
elif test $sbits -ge 16; then dbits=13 # at most rank 8, same as IDL ;-) there can be (2**13 - 1) ~ 8k elements in one dimension
else
	AC_MSG_ERROR([A ${sbits}-bit size_t is unexpectedly small; please describe your platform to <$PACKAGE_BUGREPORT>])
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
if test "x$prefix" != xNONE; then
	AC_DEFINE_UNQUOTED(LUX_MOD_PATH, "$prefix/lib/lux", [Define to the default module path.])
else
	AC_DEFINE_UNQUOTED(LUX_MOD_PATH, "$ac_default_prefix/lib/lux", [Define to the default module path.])
fi

dnl Define error code related macros
AC_DEFINE_UNQUOTED(LUX_ELAST,        $elast, [Define to the largest system error code.])
AC_DEFINE_UNQUOTED(LUX_STRINGID_BIT, $hbits, [Define to the required bits to hold a string key.])
AC_DEFINE_UNQUOTED(LUX_FACILITY_BIT, $mbits, [Define to the required bits to hold a facility key.])
AC_DEFINE_UNQUOTED(LUX_FAILURE_BIT,  $fbits, [Define to the required bits to hold a lux failure code.])
AC_DEFINE_UNQUOTED(LUX_CHAR_BIT,     $cbits, [Define to the number of bits of char.])
AC_DEFINE_UNQUOTED(LUX_INT_BIT,      $ibits, [Define to the number of bits of int.])
AC_DEFINE_UNQUOTED(LUX_UNSIGNED_BIT, $ubits, [Define to the number of bits of unsigned.])
AC_DEFINE_UNQUOTED(LUX_SIZE_T_BIT,   $sbits, [Define to the number of bits of int.])
AC_DEFINE_UNQUOTED(LUX_N_BIT,        $dbits, [Define to the number of bits of number of elements in each array dimension.])

dnl Make assertion a configure option (default disabled)
AC_ARG_ENABLE(assertion, [AC_HELP_STRING([--enable-assertion],[compile lux with assertion for debugging])], ok=$enableval, ok=no)
if test "$ok" = "yes"; then
	AC_DEFINE(LUX_ASSERTION, [1], [Define to 1 if to enable assertion for debugging])
fi
AM_CONDITIONAL(USING_ASSERTION, [test "$ok" = "yes"])

dnl Make single-precision a configure option (default disabled)
AC_ARG_ENABLE(single, [AC_HELP_STRING([--enable-single],[compile lux in single precision])], ok=$enableval, ok=no)
AC_ARG_ENABLE(float,  [AC_HELP_STRING([--enable-float], [synonym for --enable-single])],     ok=$enableval)
if test "$ok" = "yes"; then
	AC_DEFINE(LUX_SINGLE, [1], [Define to 1 if single precision is used as the default precision.])
fi
AM_CONDITIONAL(USING_SINGLE, [test "$ok" = "yes"])

AH_BOTTOM([/*===========================================================================*/

#ifndef countof
#define countof(A) (sizeof(A) / sizeof(A[0]))
#endif

#ifndef lengthof
#define lengthof(...) ({ size_t _arr_[] = {__VA_ARGS__}; countof(_arr_); })
#endif

#ifndef alignof
#define alignof(T) ((size_t)&((struct { char _chr_; T _elm_; } *)0)->_elm_)
#endif

#ifndef offsetof
#define offsetof(T, E) ((size_t)&((T *)0)->E)
#endif

#ifndef headerof
#define headerof(T, P, E) ((T *)((char *)(P) - offsetof(T, E)))
#endif

#ifndef localof
#define localof(T, I) ({ T _ = I; _; })
#endif

#ifndef TYPECODE
#define TYPECODE(C, B) (((int)(C)) | ((int)(B) << LUX_CHAR_BIT))
#endif

#ifndef typecodeof
#define typecodeof(X) ({ typeof(X) z = (typeof(X))-1.5; TYPECODE(((double)z == -1.5) ? 'f' : ((int)z == -1) ? 'i' : 'u', sizeof(X) * LUX_CHAR_BIT); })
#endif

/*===========================================================================*/
/* Public API */

extern void lux_setup(int *, char ***);
extern void lux_abort(void);

extern void lux_fatal(const void *, ...);
extern void lux_error(const void *, ...);
extern void lux_print(const void *, ...);
extern void lux_debug(const void *, ...);

extern void *lux_load(const char *, const void *);
extern void  lux_unload(void *);
extern const char *lux_fname(void *);

/*===========================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* _LUX_H_ */])

dnl Output files
AC_CONFIG_FILES(m4_esyscmd([find . -name 'Makefile.am'                 -type f | sed 's|\./\(.*\).am|\1|'    ]))
AC_CONFIG_FILES(m4_esyscmd([find . -name '*.?*in' -not -name 'lux.hin' -type f | sed 's|\./\(.*\)in|\1:\1in|']))
AC_CONFIG_FILES([tools/scripts/lux-build],   [chmod +x tools/scripts/lux-build])
AC_CONFIG_FILES([tools/scripts/lux-install], [chmod +x tools/scripts/lux-install])
AC_OUTPUT

dnl Hacks: in terms of source and build paths, there are only two variables
dnl        "$srcdir" and "$ac_pwd" that are accessible within `configure`.
dnl        All other path variables, e.g., "$top_srcdir", are empty within
dnl        `configure`.  Since "$ac_pwd" is always ".", the only useful
dnl        variable is "$srcdir".

# Ugly hack so '#include <lux/interface.h>' works
if [[ ! -e lux ]]; then
	$LN_S $srcdir/mod lux
fi

# Create our own copy of libtool for installation
if [[ ! -e tools/scripts/lux-libtool ]]; then
	$LN_S ../../libtool tools/scripts/lux-libtool
fi

# Print summary
echo "================================================================"
echo "Lux m4_esyscmd([tr -d '\n' < .version])"
echo "----------------------------------------------------------------"
echo "With HDF5     : $with_hdf5"
test "$with_hdf5" = "yes" && cat << EOF
	H5CC="$H5CC"
	HDF5_CC="$HDF5_CC"
	HDF5_CFLAGS="$HDF5_CFLAGS"
	HDF5_CPPFLAGS="$HDF5_CPPFLAGS"
	HDF5_LDFLAGS="$HDF5_LDFLAGS"
	HDF5_LIBS="$HDF5_LIBS"
EOF
echo "----------------------------------------------------------------"
echo "With MPI      : $with_mpi"
test "$with_mpi" = "yes" && cat << EOF
	MPICC="$MPICC"
	MPI_CC="$MPI_CC"
	MPI_CFLAGS="$MPI_CFLAGS"
	MPI_CPPFLAGS="$MPI_CPPFLAGS"
	MPI_LDFLAGS="$MPI_LDFLAGS"
	MPI_LIBS="$MPI_LIBS"
EOF
echo "----------------------------------------------------------------"
echo "With OpenCL   : $with_opencl"
test "$with_opencl" = "yes" && cat << EOF
	OPENCL_CPPFLAGS="$OPENCL_CPPFLAGS"
	OPENCL_LDFLAGS="$OPENCL_LDFLAGS"
EOF
echo "----------------------------------------------------------------"
echo "With OpenMP   : $with_openmp"
test "$with_openmp" = "yes" && cat << EOF
	OPENMP_CFLAGS="$OPENMP_CFLAGS"
EOF
echo "----------------------------------------------------------------"
echo "With pthread  : $with_pthread"
test "$with_pthread" = "yes" && cat << EOF
	PTHREAD_CC="$PTHREAD_CC"
	PTHREAD_CFLAGS="$PTHREAD_CFLAGS"
	PTHREAD_LIBS="$PTHREAD_LIBS"
EOF
echo "----------------------------------------------------------------"
echo "Combined flags:"
cat << EOF
	CC="$CC"
	CFLAGS="$CFLAGS"
	CPPFLAGS="$CPPFLAGS"
	LDFLAGS="$LDFLAGS"
	LIBS="$LIBS"
EOF

if test "$with_hdf5" = "yes"; then
	if test $(basename "$H5CC") = "h5cc"; then
		test "$CC" != "$HDF5_CC" && cat << EOF
WARNING CC="$CC" != HDF5_CC="$HDF5_CC"
EOF
	else
		test "$MPICC" != "$HDF5_CC" && cat << EOF
WARNING MPICC="$MPICC" != HDF5_CC="$HDF5_CC"
EOF
	fi
fi

if test "$with_mpi" = "yes"; then
	test "$CC" != "$MPI_CC" && cat << EOF
WARNING CC="$CC" != MPI_CC="$MPI_CC"
EOF
fi

echo "================================================================"
