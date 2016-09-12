AM_CPPFLAGS = -I$(top_builddir)
AM_LDFLAGS  = -no-undefined -export-dynamic -export-symbols-regex '^lux[A-Z_]' -module

tmpdir           = /tmp/lux
tmp_LTLIBRARIES  = demo.la
DISTCLEANFILES   = demo.so

demo_la_SOURCES  = demo.c
demo_la_LIBADD   = ../../api/c/libux.la
demo_la_CPPFLAGS = $(AM_CPPFLAGS) -DLUX_MOD_NAME=demo
