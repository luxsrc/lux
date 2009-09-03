AM_LDFLAGS = -export-dynamic -module

# Static modules
pkginclude_HEADERS  = algo.h basename.h bytewise.h cycle.h failed.h hash.h htab.h lazybuf.h load.h measure.h planner.h ringbuf.h ringlog.h shorthand.h solution.h solver.h switch.h task.h timer.h

# Interfaces to dynamic modules
pkginclude_HEADERS += sim.h
pkglib_LTLIBRARIES  = sim.la

sim_la_CPPFLAGS = -DLUX_MOD_NAME=sim
sim_la_SOURCES  = sim.h sim.c
sim_la_LIBADD   = ../api/libux.la
