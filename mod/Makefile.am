pkginclude_HEADERS  = assert.h dlfcn.h failed.h
SUBDIRS             = ap

# Convenience static modules (typically for renaming stuffs)
pkginclude_HEADERS += shorthand.h switch.h

# Pure static modules (include only static inline functions and macros)
pkginclude_HEADERS += basename.h hash.h match.h modman.h

# Static modules (like pure static modules but also declare trivial types)
pkginclude_HEADERS += timer.h

# Resource allocator (allocate other resource that need to be freed)
pkginclude_HEADERS += lazybuf.h load.h zalloc.h

# Hybrid modules (dynamically allocate their own resources)
pkginclude_HEADERS += dope.h htab.h libux.h vlog.h

# Interfaces and implementations of dynamic modules
pkginclude_HEADERS += job.h
SUBDIRS            += job
