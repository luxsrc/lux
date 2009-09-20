ACLOCAL_AMFLAGS = -I .build-m4

SUBDIRS = api mod src xtr

EXTRA_DIST = .autom4te.cfg .version DESIGN NOTES STYLE bootstrap

nodist_include_HEADERS = lux.h
DISTCLEANFILES = lux # ugly hack so '#include <lux/interface.h>' works;
                     # see the last few lines in "configure.ac"
