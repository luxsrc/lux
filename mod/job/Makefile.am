AM_LDFLAGS = -export-dynamic -module

pkglib_LTLIBRARIES = sim.la

sim_la_CPPFLAGS = -DLUX_MOD_NAME=sim
sim_la_SOURCES  = ../job.h ../lazybuf.h sim.c
sim_la_LIBADD   = ../../api/libux.la
