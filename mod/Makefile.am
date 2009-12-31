pkginclude_HEADERS  = assert.h aver.h atomic.h check.h dlfcn.h failed.h
SUBDIRS             = ap dm

# Convenience static modules (typically for renaming stuffs)
pkginclude_HEADERS += mangle.h numeric.h shorthand.h switch.h

# Pure static modules (include only static inline functions and macros)
pkginclude_HEADERS += array.h hash.h strutils.h tensor.h vector.h

# Static modules (like pure static modules but also declare trivial types)
pkginclude_HEADERS += counter.h refcnt.h

# Resource allocator (allocate other resource that need to be freed)
pkginclude_HEADERS += lazybuf.h zalloc.h

# Hybrid modules (dynamically allocate their own resources)
pkginclude_HEADERS += dope.h htab.h libux.h vlog.h

# Interfaces and implementations of dynamic modules
pkginclude_HEADERS += job.h
SUBDIRS            += job
