# AX_REQUIRE_LUX
# --------------
#
# SYNOPSIS
#
#   AX_REQUIRE_LUX
#
# DESCRIPTION
#
#   This macro tries to find the `lux` program from $PATH or standard
#   system directories such as "/usr/local/bin/".  It prints an error
#   message and abort autoconf if `lux` is not found.
#
# LICENSE
#
#   Copyright (C) 2011 Chi-kwan Chan
#   Copyright (C) 2011 Nordic Institute for Theoretical Physics
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

AC_DEFUN([AX_REQUIRE_LUX],
[
AC_REQUIRE([AC_PROG_CPP])
AC_REQUIRE([AC_PROG_CC_STDC])

AC_C_TYPEOF
AC_C_CONST
AC_C_INLINE
AC_C_RESTRICT

if which lux > /dev/null; then
	LUX=`which lux`
else
	for f in /{usr/,opt/}{,local/,local/lux/}bin/lux; do
		if test -f $f; then
			LUX="$f"
			break
		fi
	done
fi

if test -n "$LUX"; then
	AC_MSG_RESULT([`lux` found; its full path is "$LUX"])
	AC_SUBST([LUX], [$LUX])
else
	AC_MSG_ERROR([`lux` not found])
fi
])# AX_REQUIRE_LUX
