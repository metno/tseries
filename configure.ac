#-----------------------------------------------------------------------------
# Pre-Conf
#-----------------------------------------------------------------------------

AC_PREREQ(2.59)

#-----------------------------------------------------------------------------
# Programs
#-----------------------------------------------------------------------------
AC_INIT(src/tseries.cc)
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE(proverdaemon, 0.1.0)
AC_CONFIG_HEADER([config.h])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL

#-----------------------------------------------------------------------------
# Checks
#-----------------------------------------------------------------------------

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h sys/time.h])
AC_CHECK_HEADERS([string vector sstream fstream])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

AX_LIB_MYSQL

METNO_REQUIRE_CXX_LIBRARY([BOOST_DATE_TIME], [boost_date_time])
METNO_REQUIRE_CXX_LIBRARY([BOOST_PROGRAM_OPTIONS], [boost_program_options])
METNO_REQUIRE_C_LIBRARY([LDAP], [ldap],[ldap.h])

METNO_REQUIRE_QT4([ QtCore ])

PKG_CHECK_MODULES([prover], [prover >= 0.1.0])

AC_OUTPUT(Makefile src/Makefile)
