AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]|^dl|^fopencookie|^memfd_create|^strfailure'

lib_LTLIBRARIES = libux.la

libux_la_SOURCES  = api.h load.c setup.c log.c
libux_la_SOURCES += ../../mod/dlfcn.h ../../mod/dlib.h ../../mod/dltry.h ../../mod/dmod.h ../../mod/failed.h ../../mod/hash.h ../../mod/htab.h ../../mod/lazybuf.h ../../mod/libux.h ../../mod/load.h ../../mod/strutils.h ../../mod/timer.h ../../mod/vlog.h
libux_la_LIBADD   = ../../sys/libsys.la
