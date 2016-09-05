AM_CPPFLAGS = -I$(top_builddir)

check_PROGRAMS = ck_opencl

ck_opencl_SOURCES  = ck_opencl.c ../../mod/opencl.h
ck_opencl_LDADD    =                $(OPENCL_LIBS) ../../api/c/libux.la
ck_opencl_CPPFLAGS = $(AM_CPPFLAGS) $(OPENCL_CPPFLAGS)
ck_opencl_LDFLAGS  =                $(OPENCL_LDFLAGS)

TESTS = $(check_PROGRAMS)
