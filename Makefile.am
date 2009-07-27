ACLOCAL_AMFLAGS = -I .build-m4

SUBDIRS = api mod src

EXTRA_DIST = .autom4te.cfg .version DESIGN STYLE bootstrap

nodist_include_HEADERS = lux.h
DISTCLEANFILES = lux # ugly hack so '#include <lux/interface.h>' works;
                     # see the last few lines in "configure.ac"
