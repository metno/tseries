# 
# $Id$
#

AC_PREREQ(2.59)
AC_INIT([tseries], [4.1.0], [diana@met.no])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S

METNO_PROGRAM_SUFFIX

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

METNO_REQUIRE_HDF4

METNO_REQUIRE_PKG([PETS2], [pets2])
METNO_REQUIRE_PKG([PUMET], [puMet])
METNO_REQUIRE_PKG([GLTEXT], [glText])
METNO_REQUIRE_PKG([PUDATATYPES], [puDatatypes])
METNO_REQUIRE_PKG([QUTILITIES], [qUtilities])

METNO_REQUIRE_QT4([ 
	Gui
	Core 
	OpenGL 
	Qt3Support 
	Network 
])


# Output

# creating bin/tseries from bin/tseries.in - substituting from configure
AC_CONFIG_FILES([bin/tseries])


AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_OUTPUT



