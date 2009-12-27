AM_LDFLAGS = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]|^dlhandle|^strfailure'

lib_LTLIBRARIES = libux.la

libux_la_SOURCES = api.h abort.c load.c setup.c log.c
libux_la_LIBADD  = ../sys/libsys.la
