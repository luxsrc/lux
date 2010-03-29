AM_CPPFLAGS = -I$(top_builddir)

check_PROGRAMS = ck_clist ck_tpool
TESTS = $(check_PROGRAMS)

ck_clist_SOURCES = ck_clist.c ../mod/clist.h
ck_clist_LDADD   = ../api/libux.la

ck_tpool_SOURCES = ck_tpool.c ../mod/tpool.h
ck_tpool_LDADD   = ../api/libux.la
