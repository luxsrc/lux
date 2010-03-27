AM_LDFLAGS = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]' -module

pkginclude_HEADERS  = assert.h aver.h atomic.h check.h dlfcn.h failed.h
SUBDIRS             = ap dm

# Convenience static modules (typically for renaming stuffs)
pkginclude_HEADERS += mangle.h numeric.h shorthand.h switch.h

# Pure static modules (include only static inline functions and macros)
pkginclude_HEADERS += array.h hash.h strutils.h tensor.h vector.h

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

mpi_la_CPPFLAGS = -DLUX_MOD_NAME=mpi
mpi_la_SOURCES  = mpi.h mangle.h mpi.c
mpi_la_LIBADD   = $(MPILIBS) ../api/libux.la
