AM_LDFLAGS = -export-dynamic -module

# Convenience static modules (typically for renaming stuffs)
pkginclude_HEADERS  = assert.h failed.h shorthand.h switch.h

# Pure static modules (include only static inline functions and macros)
pkginclude_HEADERS += basename.h hash.h match.h measure.h

# Static modules (like pure static modules but also declare trivial types)
pkginclude_HEADERS += cycle.h timer.h

# Resource allocator (allocate other resource that need to be freed)
pkginclude_HEADERS += lazybuf.h load.h zalloc.h

# Hybrid modules (dynamically allocate their own resources)
pkginclude_HEADERS += dope.h htab.h vlog.h

# Interfaces to dynamic modules
pkginclude_HEADERS += job.h mpi.h planner.h solution.h solver.h spawn.h task.h
pkglib_LTLIBRARIES  = sim.la

sim_la_CPPFLAGS = -DLUX_MOD_NAME=sim
sim_la_SOURCES  = job.h sim.c
sim_la_LIBADD   = ../api/libux.la
