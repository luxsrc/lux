# AX_REQUIRE_LUX
# --------------
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
