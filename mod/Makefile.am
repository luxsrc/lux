AM_LDFLAGS = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]' -module

# Platform modules (create a portable platform)
pkginclude_HEADERS  = atomic.h cond.h cookie.h dlfcn.h failed.h memfd.h mutex.h sem.h thread.h tpool.h

#==============================================================================
# Safety modules (for error checking/handling)
pkginclude_HEADERS += assert.h aver.h check.h

# Convenience static modules (typically for renaming stuffs)
pkginclude_HEADERS += mangle.h numeric.h shorthand.h switch.h

# Pure static modules (include only static inline functions and macros)
pkginclude_HEADERS += hash.h match.h obstack.h parafor.h strutils.h tfork.h

# Static modules (like pure static modules but also declare trivial types)
pkginclude_HEADERS += counter.h list.h queue.h refcnt.h ring.h stack.h

# Resource allocator (allocate other resource that need to be freed)
pkginclude_HEADERS += alloca.h dynamic.h lazybuf.h parray.h zalloc.h

# Hybrid modules (dynamically allocate their own resources)
pkginclude_HEADERS += dope.h htab.h libux.h mpool.h mview.h vlog.h

#==============================================================================
# Adaptive performance
pkginclude_HEADERS += algo.h measure.h planner.h problem.h solver.h task.h ticks.h timer.h wisdom.h

# Dynamic-module
pkginclude_HEADERS += dlib.h dltry.h dmod.h load.h

#==============================================================================
# Interfaces and implementations of dynamic modules
pkginclude_HEADERS += job.h message.h resource.h
SUBDIRS             = job

# Dynamic modules
pkginclude_HEADERS +=
pkglib_LTLIBRARIES  =

if USING_MPI
pkginclude_HEADERS += mpi.h
pkglib_LTLIBRARIES += mpi.la
mpi_la_CPPFLAGS = -I$(top_builddir) -DLUX_MOD_NAME=mpi
mpi_la_SOURCES  = mpi.h mangle.h mutex.h mpi.c
mpi_la_CFLAGS   = $(PTHREAD_CFLAGS)
mpi_la_LIBADD   = $(PTHREAD_LIBS) $(MPILIBS) ../api/c/libux.la
endif
