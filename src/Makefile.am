
AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = develop bp-neurona exp-perceptron

develop_SOURCES = main.cc neuronal.cc perceptron.cc layer.cc network.cc plot.cc
develop_LDFLAGS = $(OCTETOS_CORE_CFLAGS)
develop_LDADD = $(OCTETOS_CORE_LIBS)


bp_neurona_SOURCES = bp-neurona.cc perceptron.cc plot.cc
bp_neurona_LDFLAGS = $(OCTETOS_CORE_CFLAGS)
bp_neurona_LDADD = $(OCTETOS_CORE_LIBS)

exp_perceptron_SOURCES = exp-perceptron.cc plot.cc
exp_perceptron_LDFLAGS = $(OCTETOS_CORE_CFLAGS)
exp_perceptron_LDADD = $(OCTETOS_CORE_LIBS)