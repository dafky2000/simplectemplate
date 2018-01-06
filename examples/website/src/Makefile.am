bin_PROGRAMS=simplecgen
AM_CFLAGS= -std=c99 -Wall -pedantic -g -I../../../src
simplecgen_SOURCES=			simplecgen.c ../../../src/template_functions.c

PACKAGE = @PACKAGE@
VERSION = @VERSION@

sysconfdir = @sysconfdir@
DEFS = -DSYSCONFDIR=\"$(sysconfdir)\" @DEFS@
