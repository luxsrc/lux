AM_CPPFLAGS = -I$(top_builddir)

check_PROGRAMS = ck_hdf5

CC=@HDF5_CC@

ck_hdf5_SOURCES  = ck_hdf5.c ../../mod/hdf5.h
ck_hdf5_LDADD    =                $(HDF5_LIBS)     ../../api/c/libux.la
ck_hdf5_CFLAGS   =                $(HDF5_CFLAGS)
ck_hdf5_CPPFLAGS = $(AM_CPPFLAGS) $(HDF5_CPPFLAGS)
ck_hdf5_LDFLAGS  =                $(HDF5_LDFLAGS)

TESTS = $(check_PROGRAMS)
