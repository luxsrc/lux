AM_LDFLAGS = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]' -module

pkginclude_HEADERS  = assert.h aver.h atomic.h check.h cond.h dlfcn.h failed.h mutex.h sem.h thread.h tpool.h worker.h
SUBDIRS             = ap dm

# Convenience static modules (typically for renaming stuffs)
pkginclude_HEADERS += mangle.h numeric.h shorthand.h switch.h

# Pure static modules (include only static inline functions and macros)
pkginclude_HEADERS += array.h hash.h parafor.h strutils.h tensor.h tfork.h vector.h

# Static modules (like pure static modules but also declare trivial types)
pkginclude_HEADERS += counter.h refcnt.h

# Resource allocator (allocate other resource that need to be freed)
pkginclude_HEADERS += alloca.h lazybuf.h zalloc.h

# Hybrid modules (dynamically allocate their own resources)
pkginclude_HEADERS += dope.h htab.h libux.h vlog.h

# Interfaces and implementations of dynamic modules
pkginclude_HEADERS += job.h
SUBDIRS            += job

# Dynamic modules
pkginclude_HEADERS += mpi.h
pkglib_LTLIBRARIES  = mpi.la

mpi_la_CPPFLAGS = -I$(top_builddir) -DLUX_MOD_NAME=mpi
mpi_la_SOURCES  = mpi.h mangle.h mutex.h mpi.c
mpi_la_CFLAGS   = $(PTHREAD_CFLAGS)
mpi_la_LIBADD   = $(PTHREAD_LIBS) $(MPILIBS) ../api/libux.la
