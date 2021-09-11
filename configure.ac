

AC_INIT(octetos-neuronal, 0.1.0-alpha,azael.devel@gmail.com)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CXX








LT_INIT

AC_CHECK_PROG([gnuplot],[gnuplot],[yes],[no])
AM_CONDITIONAL([FOUND_GNUPLOT], [test "x$gnuplot" = xyes])
AM_COND_IF([FOUND_GNUPLOT],,[AC_MSG_ERROR([required program 'gnuplot' not found.])])

PKG_CHECK_MODULES(OCTETOS_CORE,octetos-core)


AC_OUTPUT([
Makefile
src/Makefile

])
