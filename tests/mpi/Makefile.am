AM_CPPFLAGS = -I$(top_builddir)

check_PROGRAMS = ck_mpi

CC=@MPI_CC@

ck_mpi_SOURCES  = ck_mpi.c ../../mod/mpi.h
ck_mpi_LDADD    = $(PTHREAD_LIBS)   $(MPI_LIBS)     ../../api/c/libux.la
ck_mpi_CFLAGS   = $(PTHREAD_CFLAGS) $(MPI_CFLAGS)
ck_mpi_CPPFLAGS = $(AM_CPPFLAGS)    $(MPI_CPPFLAGS)
ck_mpi_LDFLAGS  =                   $(MPI_LDFLAGS)
