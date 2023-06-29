PROJECT(tseries)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(MetnoUtils)
INCLUDE(MetnoQtTranslation)
INCLUDE(MetnoQtUtils)
INCLUDE(MetnoVersions)
METNO_PVERSION(7.2.2)

FIND_PACKAGE(Boost COMPONENTS date_time REQUIRED)

METNO_FIND_QT(Gui Core Network Xml PrintSupport)

SET(PKGS
  metlibs-pets2${METNO_QT_SUFFIX}>=8.0.0
  metlibs-putools>=8.0.0
  metlibs-pumet>=7.0.0
  tsData>=7.0.0
  metlibs-pudatatypes>=6.0.0
  metlibs-coserver${METNO_QT_SUFFIX}>=3.0.0
  metlibs-milogger>=6.0.0
)

SET(ENABLE_FIMEX ON CACHE BOOL "use fimex to read NetCDF and other files")
IF(ENABLE_FIMEX)
LIST(APPEND PKGS
  fimex>=1.7.0
)
ENDIF() # ENABLE_FIMEX
PKG_CHECK_MODULES(PC REQUIRED ${PKGS})

ADD_SUBDIRECTORY(src)


SET(MAN_TSERIES "tseries.bin-${PVERSION}.1")
CONFIGURE_FILE(tseries.1.in "${MAN_TSERIES}")
INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/${MAN_TSERIES}"
  DESTINATION ${CMAKE_INSTALL_MANDIR}
)
