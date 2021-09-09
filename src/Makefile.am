
AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = develop experement

develop_SOURCES = main.cc neuronal.cc perceptron.cc layer.cc network.cc plot.cc
develop_LDFLAGS = $(OCTETOS_CORE_CFLAGS)
develop_LDADD = $(OCTETOS_CORE_LIBS)

experement_SOURCES = experement.cc perceptron.cc plot.cc
experement_LDFLAGS = $(OCTETOS_CORE_CFLAGS)
experement_LDADD = $(OCTETOS_CORE_LIBS)