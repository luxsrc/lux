AM_LDFLAGS = -export-dynamic -module

# Static modules
pkginclude_HEADERS  = assert.h basename.h cycle.h dope.h failed.h hash.h htab.h lazybuf.h load.h measure.h planner.h shorthand.h solution.h solver.h switch.h task.h timer.h vlog.h

# Interfaces to dynamic modules
pkginclude_HEADERS += job.h
pkglib_LTLIBRARIES  = sim.la

sim_la_CPPFLAGS = -DLUX_MOD_NAME=sim
sim_la_SOURCES  = job.h sim.c
sim_la_LIBADD   = ../api/libux.la
