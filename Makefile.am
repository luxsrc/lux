ACLOCAL_AMFLAGS = -I .build-m4

SUBDIRS = mod api tools tests

EXTRA_DIST = .autom4te.cfg .version bootstrap

nodist_include_HEADERS = lux.h
DISTCLEANFILES = lux # ugly hack so '#include <lux/interface.h>' works;
                     # see the last few lines in "configure.ac"
