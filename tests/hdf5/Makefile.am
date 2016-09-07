AM_CPPFLAGS = -I$(top_builddir)

check_PROGRAMS = ck_hdf5 ck_type ck_io

CC=@HDF5_CC@

ck_hdf5_SOURCES  = ck_hdf5.c ../../mod/hdf5.h
ck_hdf5_LDADD    =                $(HDF5_LIBS)     ../../api/c/libux.la
ck_hdf5_CFLAGS   =                $(HDF5_CFLAGS)
ck_hdf5_CPPFLAGS = $(AM_CPPFLAGS) $(HDF5_CPPFLAGS)
ck_hdf5_LDFLAGS  =                $(HDF5_LDFLAGS)

ck_type_SOURCES  = ck_type.c ../../mod/hdf5.h
ck_type_LDADD    =                $(HDF5_LIBS)     ../../api/c/libux.la
ck_type_CFLAGS   =                $(HDF5_CFLAGS)
ck_type_CPPFLAGS = $(AM_CPPFLAGS) $(HDF5_CPPFLAGS)
ck_type_LDFLAGS  =                $(HDF5_LDFLAGS)

ck_io_SOURCES  = ck_io.c ../../mod/hdf5.h
ck_io_LDADD    =                $(HDF5_LIBS)     ../../api/c/libux.la
ck_io_CFLAGS   =                $(HDF5_CFLAGS)
ck_io_CPPFLAGS = $(AM_CPPFLAGS) $(HDF5_CPPFLAGS)
ck_io_LDFLAGS  =                $(HDF5_LDFLAGS)
