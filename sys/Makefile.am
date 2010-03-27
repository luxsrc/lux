AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined

# Remember to add the symbols defined below to ../api/Makefile.am
noinst_LTLIBRARIES = libsys.la
libsys_la_SOURCES  = ../mod/dlfcn.h dlmopen.c dlfname.c dllmid.c dlhandle.c
libsys_la_SOURCES += ../mod/failed.h strfailure.c
libsys_la_LIBADD   = -ldl
