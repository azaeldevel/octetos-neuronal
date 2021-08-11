
AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = develop

develop_SOURCES = main.cc Perceptron.cc
develop_LDFLAGS = $(OCTETOS_CORE_CFLAGS)
develop_LDADD = $(OCTETOS_CORE_LIBS)


