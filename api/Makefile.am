AM_LDFLAGS = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]|^dlhandle|^strfailure'

lib_LTLIBRARIES = libux.la

libux_la_SOURCES = ../mod/basename.h ../mod/dlfcn.h ../mod/dm/dmod.h ../mod/dm/dlib.h ../mod/dm/dltry.h ../mod/dm/load.h ../mod/failed.h ../mod/hash.h ../mod/htab.h ../mod/lazybuf.h ../mod/libux.h ../mod/ap/timer.h ../mod/vlog.h api.h abort.c load.c setup.c log.c
libux_la_LIBADD  = ../sys/libsys.la
