AM_LDFLAGS = -export-dynamic -module

pkginclude_HEADERS = hash.h hello.h htab.h
pkglib_LTLIBRARIES = hello.la

hello_la_CPPFLAGS = -DLUX_MOD_NAME=hello
hello_la_SOURCES  = hello.h hello.c
hello_la_LIBADD   = ../api/libux.la
