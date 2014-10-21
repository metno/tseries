# 
# $Id$
#

AC_PREREQ(2.59)
AC_INIT([tseries], [6.5.3], [diana@met.no])

AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])

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

METNO_REQUIRE_PKG([PETS2], [pets2])
METNO_REQUIRE_PKG([PUMET], [puMet])
METNO_REQUIRE_PKG([GLTEXT], [glText])
METNO_REQUIRE_PKG([PUDATATYPES], [puDatatypes])
METNO_REQUIRE_PKG([XEXT],[xext])
METNO_REQUIRE_PKG([GLU],[glu])
METNO_REQUIRE_PKG([X11],[x11])
METNO_REQUIRE_PKG([FIMEX],[fimex])
METNO_REQUIRE_PKG([COSERVER],[coserver])

METNO_ENABLE([gribfile],[GRIBFILE], [enable Gribfile support],[no])
METNO_ENABLE_CONDITIONAL([GRIBFILE], [GRIBFILE])

METNO_ENABLE([smhi],[smhi],[use smhi setup files],[no])
METNO_ENABLE_CONDITIONAL([SMHI],[smhi])


METNO_REQUIRE_QT4([
	Gui
	Core
	OpenGL
	Network
])


# Output

# creating bin/tseries from bin/tseries.in - substituting from configure

#AC_SUBST([prefix],[$prefix])

#METNO_PROGRAM_SUFFIX


AC_CONFIG_FILES([
	bin/tseries
	Makefile
	src/Makefile
	share/applications/metno-tseries.desktop
])

AC_OUTPUT



