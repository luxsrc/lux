AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]' -module

pkglib_LTLIBRARIES = opencl.la

opencl_la_SOURCES  = ../opencl.h mod.h mod.c strerr.c info.c ctx.c src.c pro.c kernel.c mm.c utils.c
opencl_la_LIBADD   =                $(OPENCL_LIBS)     ../../api/c/libux.la
opencl_la_CPPFLAGS = $(AM_CPPFLAGS) $(OPENCL_CPPFLAGS) -DLUX_MOD_NAME=opencl
opencl_la_LDFLAGS  = $(AM_LDFLAGS)  $(OPENCL_LDFLAGS)
