AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]|^dl|^fopencookie|^memfd_create|^strfailure'

lib_LTLIBRARIES = libux.la

libux_la_SOURCES = ../mod/timer.h ../mod/dlfcn.h ../mod/dmod.h ../mod/dlib.h ../mod/dltry.h ../mod/load.h ../mod/failed.h ../mod/hash.h ../mod/htab.h ../mod/lazybuf.h ../mod/libux.h ../mod/strutils.h ../mod/vlog.h api.h load.c setup.c log.c
libux_la_LIBADD  = ../sys/libsys.la
