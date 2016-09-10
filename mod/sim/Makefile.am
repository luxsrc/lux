AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]' -module

pkglib_LTLIBRARIES = sim.la

sim_la_SOURCES  = ../job.h mod.c
sim_la_SOURCES += ../lazybuf.h ../mangle.h
sim_la_LIBADD   = ../../api/c/libux.la
sim_la_CPPFLAGS = $(AM_CPPFLAGS) -DLUX_MOD_NAME=sim
