AC_PREREQ(2.59)
AC_INIT([tseries], [6.9.3], [diana@met.no])

AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.11 foreign parallel-tests])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S

METNO_PVERSION

METNO_PROG_PKG_CONFIG

#-----------------------------------------------------------------------------
# Checks
#-----------------------------------------------------------------------------

#
# C++
#
AC_LANG_PUSH([C++])

AC_CXX_HAVE_STD
AC_CXX_HAVE_STL

AC_LANG_POP([C++])

#
METNO_WITH_CONDITIONAL([FIMEX], [fimex])

METNO_REQUIRE_BOOST
METNO_REQUIRE_BOOST_FEATURE([date_time])

METNO_REQUIRE_PKG([PETS2], [pets2], [6.0.0])
METNO_REQUIRE_PKG([PUMET], [puMet], [5.1.0])
METNO_REQUIRE_PKG([PUDATATYPES], [puDatatypes])
METNO_REQUIRE_PKG([FIMEX],[fimex])
METNO_REQUIRE_PKG([COSERVER],[coserver],[2.0.5])


METNO_ENABLE([gribfile],[GRIBFILE], [enable Gribfile support],[no])
METNO_ENABLE_CONDITIONAL([GRIBFILE], [GRIBFILE])

METNO_ENABLE([smhi],[smhi],[use smhi setup files],[no])
METNO_ENABLE_CONDITIONAL([SMHI],[smhi])


METNO_REQUIRE_QT4([
	Gui
	Core
	Network
	Xml
])

AC_CONFIG_FILES([
	bin/tseries
	Makefile
	src/Makefile
	share/applications/metno-tseries.desktop
])
AC_OUTPUT
