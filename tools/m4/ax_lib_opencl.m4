# AX_LIB_OPENCL: motivated by AX_LIB_MPI
# --------------------------------------
#
# SYNOPSIS
#
#   AX_LIB_OPENCL
#
# DESCRIPTION
#
#   This macro provides tests of the availability of OpenCL library.
#
#   The macro adds a --with-opencl option accepting one of three values:
#
#     no   - do not check for the OpenCL library.
#     yes  - do check for OpenCL library in standard locations.
#     path - prefix of OpenCL library.
#
#   If OpenCL is successfully found, this macro calls
#
#     AC_SUBST(OPENCL_CPPFLAGS)
#     AC_SUBST(OPENCL_LDFLAGS)
#     AC_DEFINE(HAVE_OPENCL)
#
#   and sets with_opencl="yes".
#
#   If OpenCL is disabled or not found, this macros sets with_opencl="no".
#
#   Your configuration script can test $with_opencl to take any further
#   actions. OPENCL_{CPPFLAGS,LDFLAGS,LIBS} may be used when building with C.
#
#   To use the macro, one would code the following in "configure.ac"
#   before AC_OUTPUT:
#
#     dnl Check for OpenCL support
#     AX_LIB_OPENCL
#
#   One could test $with_opencl for the outcome or display it as follows
#
#     echo "OpenCL support:  $with_opencl"
#
#   You could also for example, override the default CC in "configure.ac" to
#   enforce compilation with the compiler that OpenCL uses:
#
#     AX_LIB_OPENCL
#     if test "$with_opencl" = "yes"; then
#             CC="$OPENCL_CC"
#     else
#             AC_MSG_ERROR([Unable to find OpenCL.])
#     fi
#
# LICENSE
#
#   Copyright (C) 2016 Chi-kwan Chan
#   Copyright (C) 2016 Steward Observatory
#
#   This program is free software: you can redistribute it and/or
#   modify it under the terms of the GNU General Public License as
#   published by the Free Software Foundation, either version 3 of the
#   License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#   General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright
#   owner gives unlimited permission to copy, distribute and modify
#   the configure scripts that are the output of Autoconf when
#   processing the Macro.  You need not follow the terms of the GNU
#   General Public License when using or distributing such scripts,
#   even though portions of the text of the Macro appear in them.  The
#   GNU General Public License (GPL) does govern all other use of the
#   material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the
#   Autoconf Macro released by the Autoconf Archive.  When you make
#   and distribute a modified version of the Autoconf Macro, you may
#   extend this special exception to the GPL to apply to your modified
#   version as well.

AC_DEFUN([AX_LIB_OPENCL], [

dnl Add a default --with-opencl configuration option.
AC_ARG_WITH([opencl],
	AS_HELP_STRING(
		[--with-opencl=[yes/no/PATH]],
		[location of OpenCL library]
  	),
	[if test "$withval" = "no"; then
		with_opencl="no"
	elif test "$withval" = "yes"; then
		with_opencl="yes"
	else
		with_opencl="yes"
		OPENCL_PREFIX="${withval}"
	fi],
	[with_opencl="yes"]
)

dnl Set defaults to blank
OPENCL_CPPFLAGS=""
OPENCL_LDFLAGS=""
OPENCL_LIBS=""

dnl Determine compile/link flags
if test "$with_opencl" = "yes"; then
	dnl Determine if we are on Mac and need to compile/link OpenCL as Apple framework
	AC_REQUIRE([AX_PROG_CC_FRAMEWORK])

	if test "$ac_cv_prog_cc_framework" = "yes"; then
		if test -n "$OPENCL_PREFIX"; then
			OPENCL_CPPFLAGS+=" -F${OPENCL_PREFIX}"
			OPENCL_LDFLAGS+=" -F${OPENCL_PREFIX}"
		fi
		OPENCL_CPPFLAGS+=" -framework OpenCL"
		OPENCL_LDFLAGS+=" -framework OpenCL"
	else
		if test -n "$OPENCL_PREFIX"; then
			OPENCL_CPPFLAGS+=" -I${OPENCL_PREFIX}/include"
			OPENCL_LDFLAGS+=" -L${OPENCL_PREFIX}/lib64 -L${OPENCL_PREFIX}/lib"
		fi
		OPENCL_LIBS+=" OpenCL"
	fi

	dnl Define variables
	AC_SUBST([OPENCL_CPPFLAGS])
	AC_SUBST([OPENCL_LDFLAGS])
	AC_SUBST([OPENCL_LIBS])
	AC_DEFINE([HAVE_OPENCL], [1], [Defined if you have OpenCL support])
fi
])
