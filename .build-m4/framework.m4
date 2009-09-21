# AC_PROG_CC_FRAMEWORK: modify from AC_PROG_CC_C_O
# ------------------------------------------------
AC_DEFUN([AC_PROG_CC_FRAMEWORK],
[AC_REQUIRE([AC_PROG_CC])dnl
if test "x$CC" != xcc; then
	AC_MSG_CHECKING([whether $CC and cc understand -framework])
else
	AC_MSG_CHECKING([whether cc understands -framework])
fi
set dummy $CC; ac_cc=`AS_ECHO(["$[2]"]) | sed 's/[[^a-zA-Z0-9_]]/_/g;s/^[[0-9]]/_/'`
AC_CACHE_VAL(ac_cv_prog_cc_${ac_cc}_framework,
[AC_LANG_CONFTEST([AC_LANG_PROGRAM([])])
# Make sure it works both with $CC and with simple cc.
# We do the test twice because AC_PROG_CC_C_O does it.
ac_try='$CC conftest.$ac_ext -o conftest2.$ac_exeext -framework CoreFoundation >&AS_MESSAGE_LOG_FD'
rm -f conftest2.*
if _AC_DO_VAR(ac_try) && test -f conftest2.$ac_exeext && _AC_DO_VAR(ac_try); then
	eval ac_cv_prog_cc_${ac_cc}_framework=yes
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
				eval ac_cv_prog_cc_${ac_cc}_framework=no
			fi
		fi
	fi
else
	eval ac_cv_prog_cc_${ac_cc}_framework=no
fi
rm -f core conftest*
])dnl
if eval test \$ac_cv_prog_cc_${ac_cc}_framework = yes; then
	AC_MSG_RESULT([yes])
	AC_DEFINE(SUPPORT_FRAMEWORK, 1, [Define to 1 if your C compiler supports -framework.])
else
	AC_MSG_RESULT([no])
fi
])# AC_PROG_CC_FRAMEWORK
