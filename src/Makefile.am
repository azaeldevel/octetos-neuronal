
AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = develop bp-neurona exp-perceptron-circle exp-perceptron-line exp-network

develop_SOURCES = main.cc neuronal.cc perceptron.cc layer.cc  network.cc  network-bp.cc plot.cc
develop_LDFLAGS = $(OCTETOS_CORE_CFLAGS)
develop_LDADD = $(OCTETOS_CORE_LIBS)


bp_neurona_SOURCES = bp-neurona.cc perceptron.cc plot.cc
bp_neurona_LDFLAGS = $(OCTETOS_CORE_CFLAGS)
bp_neurona_LDADD = $(OCTETOS_CORE_LIBS)

exp_perceptron_circle_SOURCES = exp-perceptron-circle.cc plot.cc
exp_perceptron_circle_LDFLAGS = $(OCTETOS_CORE_CFLAGS)
exp_perceptron_circle_LDADD = $(OCTETOS_CORE_LIBS)

exp_perceptron_line_SOURCES = exp-perceptron-line.cc plot.cc
exp_perceptron_line_LDFLAGS = $(OCTETOS_CORE_CFLAGS)
exp_perceptron_line_LDADD = $(OCTETOS_CORE_LIBS)


exp_network_SOURCES = exp-network.cc

exp_examples_SOURCES = exp-examples.cc