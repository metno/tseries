# Makefile to build tseries & btseries 
# $Id$ 
#

SUBDIRS=src

ACLOCAL_AMFLAGS= -I m4


tseriesbindir= $(bindir)
dist_tseriesbin_SCRIPTS= bin/tseries


etctseriesdir = $(sysconfdir)/${APP}


if ENABLE_SMHI

include smhi/etc/etc.mk

else

#dist_etctseries_DATA = etc/${APP}/tseries.ctl etc/${APP}/tsDiagrams.def      
dist_etctseries_DATA = etc/tseries-test/tseries.ctl etc/tseries-test/tsDiagrams.def      

if !HAS_PROGRAM_SUFFIX

include share/share.mk
include etc/etc.mk

else

include share/share-test.mk

endif # !HAS_PROGRAM_SUFFIX

endif # ENABLE_SMHI


applicationsdir        = $(datarootdir)/applications
dist_applications_DATA = share/applications/metno-${APP}.desktop


-include mk/debian.mk

