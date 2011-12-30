# AX_PROG_CC_FRAMEWORK: modify from AC_PROG_CC_C_O
# ------------------------------------------------
#
# SYNOPSIS
#
#   AX_PROG_CC_FRAMEWORK
#
# DESCRIPTION
#
#   This macro tests if "$CC" and `cc` understand the flag
#   "-framework", which provides a hint on whether we are running on
#   Mac OS X (Darwin).
#
# LICENSE
#
#   Copyright (C) 2009 Chi-kwan Chan
#   Copyright (C) 2009 Harvard-Smithsonian Center for Astrophysics
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

AC_DEFUN([AX_PROG_CC_FRAMEWORK],
[AC_REQUIRE([AC_PROG_CC])dnl
if test "x$CC" != xcc; then
	AC_MSG_CHECKING([whether $CC and cc understand -framework])
else
	AC_MSG_CHECKING([whether cc understands -framework])
fi
set dummy $CC; ac_cc=`AS_ECHO(["$[2]"]) | sed 's/[[^a-zA-Z0-9_]]/_/g;s/^[[0-9]]/_/'`
AC_CACHE_VAL(ax_cv_prog_cc_${ac_cc}_framework,
[AC_LANG_CONFTEST([AC_LANG_PROGRAM([])])
# Make sure it works both with $CC and with simple cc.
# We do the test twice because AC_PROG_CC_C_O does it.
ac_try='$CC conftest.$ac_ext -o conftest2.$ac_exeext -framework CoreFoundation >&AS_MESSAGE_LOG_FD'
rm -f conftest2.*
if _AC_DO_VAR(ac_try) && test -f conftest2.$ac_exeext && _AC_DO_VAR(ac_try); then
	eval ax_cv_prog_cc_${ac_cc}_framework=yes
	if test "x$CC" != xcc; then
		# Test first that cc exists at all.
		if _AC_DO_TOKENS(cc conftest.$ac_ext -o conftest2.$ac_exeext >&AS_MESSAGE_LOG_FD); then
			ac_try='cc conftest.$ac_ext -o conftest2.$ac_exeext -framework CoreFoundation >&AS_MESSAGE_LOG_FD'
			rm -f conftest2.*
			if _AC_DO_VAR(ac_try) && test -f conftest2.$ac_exeext && _AC_DO_VAR(ac_try); then
				# cc works too.
				:
			else
				# cc exists but doesn't like -o.
				eval ax_cv_prog_cc_${ac_cc}_framework=no
			fi
		fi
	fi
else
	eval ax_cv_prog_cc_${ac_cc}_framework=no
fi
rm -f core conftest*
])dnl
if eval test \$ax_cv_prog_cc_${ac_cc}_framework = yes; then
	AC_MSG_RESULT([yes])
	AC_DEFINE(SUPPORT_FRAMEWORK, 1, [Define to 1 if your C compiler supports -framework.])
else
	AC_MSG_RESULT([no])
fi
])# AX_PROG_CC_FRAMEWORK
