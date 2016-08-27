AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]' -module

pkglib_LTLIBRARIES = opencl.la

opencl_la_SOURCES  = ../opencl.h mod.h mod.c src.c kernel.c
opencl_la_CPPFLAGS = $(AM_CPPFLAGS) -DLUX_MOD_NAME=opencl
opencl_la_LDFLAGS  = $(AM_LDFLAGS)
if SUPPORT_FRAMEWORK
opencl_la_LDFLAGS += -framework OpenCL
else
opencl_la_LDFLAGS += -lOpenCL
endif
opencl_la_LIBADD   = ../../api/c/libux.la
