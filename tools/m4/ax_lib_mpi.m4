# AX_LIB_MPI: motivated by AX_LIB_HDF5
# ------------------------------------
#
# SYNOPSIS
#
#   AX_LIB_MPI
#
# DESCRIPTION
#
#   This macro provides tests of the availability of MPI library.
#
#   The macro looks for MPI installations via `mpicc`.
#
#   The macro adds a --with-mpi option accepting one of three values:
#
#     no   - do not check for the MPI library.
#     yes  - do check for MPI library in standard locations.
#     path - complete path to the MPI helper script `mpicc`.
#
#   If MPI is successfully found, this macro calls
#
#     AC_SUBST(MPI_CC)
#     AC_SUBST(MPI_VERSION)
#     AC_SUBST(MPI_CFLAGS)
#     AC_SUBST(MPI_CPPFLAGS)
#     AC_SUBST(MPI_LDFLAGS)
#     AC_SUBST(MPI_LIBS)
#     AC_DEFINE(HAVE_MPI)
#
#   and sets with_mpi="yes".
#
#   If MPI is disabled or not found, this macros sets with_mpi="no".
#
#   Your configuration script can test $with_mpi to take any further
#   actions. MPI_{C,CPP,LD}FLAGS may be used when building with C.
#
#   To use the macro, one would code the following in "configure.ac"
#   before AC_OUTPUT:
#
#     dnl Check for MPI support
#     AX_LIB_MPI
#
#   One could test $with_mpi for the outcome or display it as follows
#
#     echo "MPI support:  $with_mpi"
#
#   You could also for example, override the default CC in "configure.ac" to
#   enforce compilation with the compiler that MPI uses:
#
#     AX_LIB_MPI
#     if test "$with_mpi" = "yes"; then
#             CC="$MPI_CC"
#     else
#             AC_MSG_ERROR([Unable to find MPI, we need MPI.])
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

AC_DEFUN([AX_LIB_MPI], [

dnl Add a default --with-mpi configuration option.
AC_ARG_WITH([mpi],
	AS_HELP_STRING(
		[--with-mpi=[yes/no/PATH]],
		[location of mpicc for MPI configuration]
  	),
	[if test "$withval" = "no"; then
		with_mpi="no"
	elif test "$withval" = "yes"; then
		with_mpi="yes"
	else
		with_mpi="yes"
		MPICC="$withval"
	fi],
	[with_mpi="yes"]
)

dnl Set defaults to blank
MPI_CC=""
MPI_VERSION=""
MPI_CFLAGS=""
MPI_CPPFLAGS=""
MPI_LDFLAGS=""
MPI_LIBS=""

dnl Try and find MPI compiler tools and options.
if test "$with_mpi" = "yes"; then
	if test -z "$MPICC"; then
		dnl Check to see if mpicc is in the path.
		AC_PATH_PROGS([MPICC], [mpicc])
	else
		AC_MSG_CHECKING([Using provided MPI C wrapper])
		AC_MSG_RESULT([$MPICC])
	fi

	AC_MSG_CHECKING([for MPI libraries])
	if test ! -f "$MPICC" || test ! -x "$MPICC"; then
		AC_MSG_RESULT([no])
		AC_MSG_WARN([
Unable to locate MPI compilation helper scripts 'mpicc'.
Please specify --with-mpi=<LOCATION> as the full path to mpicc.
MPI support is being disabled (equivalent to --with-mpi=no).
])
		with_mpi="no"
	else
		dnl Get the flags using OpenMPI "--showme:*"
		MPI_CC=$($MPICC --showme:command)
		MPI_VERSION=$(mpicc --showme:versione | cut -d' ' -f2-)
		MPI_CPPFLAGS=$(for d in $(mpicc -showme:incdirs); do printf " -I$d"; done)
		MPI_LDFLAGS=$(for d in $(mpicc -showme:libdirs); do printf " -L$d"; done)
		MPI_LIBS=$(for l in $(mpicc -showme:libs); do printf " -l$l"; done)
		AC_MSG_RESULT([yes (version $[MPI_VERSION])])

		dnl Check header and library
		AC_LANG_PUSH([C])

			ax_lib_mpi_save_CC=$CC
			ax_lib_mpi_save_CPPFLAGS=$CPPFLAGS
			ax_lib_mpi_save_LDFLAGS=$LDFLAGS
			ax_lib_mpi_save_LIBS=$LIBS

			CC=$MPI_CC
			CPPFLAGS=$MPI_CPPFLAGS
			LDFLAGS=$MPI_LDFLAGS
			LIBS=$MPI_LIBS

			AC_CHECK_HEADER([mpi.h], [ac_cv_mpi_h=yes], [ac_cv_mpi_h=no])
			AC_CHECK_LIB([mpi], [MPI_Init], [ac_cv_libmpi=yes], [ac_cv_libmpi=no])
			if test "$ac_cv_mpi_h" = "no" || test "$ac_cv_libmpi" = "no" ; then
				AC_MSG_WARN([Unable to compile MPI test program])
			fi

			CC=$ax_lib_mpi_save_CC
			CPPFLAGS=$ax_lib_mpi_save_CPPFLAGS
			LDFLAGS=$ax_lib_mpi_save_LDFLAGS
			LIBS=$ax_lib_mpi_save_LIBS

		AC_LANG_POP([C])

		dnl Define variables
		AC_SUBST([MPI_CC])
		AC_SUBST([MPI_VERSION])
		AC_SUBST([MPI_CFLAGS])
		AC_SUBST([MPI_CPPFLAGS])
		AC_SUBST([MPI_LDFLAGS])
		AC_SUBST([MPI_LIBS])
		AC_DEFINE([HAVE_MPI], [1], [Defined if you have MPI support])
	fi
fi
])
