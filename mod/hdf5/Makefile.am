AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]' -module

pkglib_LTLIBRARIES = hdf5.la

CC=@HDF5_CC@

hdf5_la_SOURCES  = ../hdf5.h mod.h mod.c
hdf5_la_SOURCES += ../file.h ../io.h ../mangle.h
hdf5_la_LIBADD   =                $(HDF5_LIBS)     ../../api/c/libux.la
hdf5_la_CFLAGS   =                $(HDF5_CFLAGS)
hdf5_la_CPPFLAGS = $(AM_CPPFLAGS) $(HDF5_CPPFLAGS) -DLUX_MOD_NAME=hdf5
hdf5_la_LDFLAGS  = $(AM_LDFLAGS)  $(HDF5_LDFLAGS)
