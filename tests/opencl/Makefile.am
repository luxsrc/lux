AM_CPPFLAGS = -I$(top_builddir)

EXTRA_DIST = dummy.cl

check_PROGRAMS = ck_new ck_copy

ck_new_SOURCES  = ck_new.c ../../mod/opencl.h
ck_new_LDADD    =                $(OPENCL_LIBS) ../../api/c/libux.la
ck_new_CPPFLAGS = $(AM_CPPFLAGS) $(OPENCL_CPPFLAGS)
ck_new_LDFLAGS  =                $(OPENCL_LDFLAGS)

ck_copy_SOURCES  = ck_copy.c ../../mod/opencl.h
ck_copy_LDADD    =                $(OPENCL_LIBS) ../../api/c/libux.la
ck_copy_CPPFLAGS = $(AM_CPPFLAGS) $(OPENCL_CPPFLAGS)
ck_copy_LDFLAGS  =                $(OPENCL_LDFLAGS)
