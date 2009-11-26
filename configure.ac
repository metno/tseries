#-----------------------------------------------------------------------------
# Pre-Conf
#-----------------------------------------------------------------------------

AC_PREREQ(2.59)

#-----------------------------------------------------------------------------
# Programs
#-----------------------------------------------------------------------------
AC_INIT(src/tseries.cc)
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE(tseries, 3.1.0)
AC_CONFIG_HEADER([config.h])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S

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
	QtGui
	QtCore 
	QtOpenGL 
	Qt3Support 
	QtNetwork 
])


# Output

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_OUTPUT



