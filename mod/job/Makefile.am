AM_LDFLAGS = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]' -module

pkglib_LTLIBRARIES = sim.la

sim_la_CPPFLAGS = -DLUX_MOD_NAME=sim
sim_la_SOURCES  = ../job.h sim.c
sim_la_LIBADD   = ../../api/libux.la
