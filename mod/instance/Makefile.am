AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]' -module

pkglib_LTLIBRARIES = instance.la

instance_la_SOURCES  = ../instance.h mod.h mod.c
instance_la_LIBADD   = ../../api/c/libux.la
instance_la_CPPFLAGS = $(AM_CPPFLAGS) -DLUX_MOD_NAME=instance
instance_la_LDFLAGS  = $(AM_LDFLAGS)
