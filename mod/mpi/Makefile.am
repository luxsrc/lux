AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]' -module

pkglib_LTLIBRARIES = mpi.la

CC=@MPI_CC@

mpi_la_SOURCES  = ../mpi.h mod.c
mpi_la_SOURCES += ../mangle.h ../mutex.h
mpi_la_LIBADD   = $(PTHREAD_LIBS)   $(MPI_LIBS)     ../../api/c/libux.la
mpi_la_CFLAGS   = $(PTHREAD_CFLAGS) $(MPI_CFLAGS)
mpi_la_CPPFLAGS = $(AM_CPPFLAGS)    $(MPI_CPPFLAGS) -DLUX_MOD_NAME=mpi
mpi_la_LDFLAGS  = $(AM_LDFLAGS)     $(MPI_LDFLAGS)
