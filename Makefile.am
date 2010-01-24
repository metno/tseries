# Makefile to build tseries & btseries 
# $Id$ 
#

SUBDIRS=src

ACLOCAL_AMFLAGS= -I m4


tseriesbindir= $(bindir)
dist_tseriesbin_SCRIPTS= bin/tseries


etcdir        = $(sysconfdir)/${APP}


if ENABLE_SMHI

include smhi/etc/etc.mk

else

dist_etc_DATA = etc/${APP}/tseries.ctl

if !HAS_PROGRAM_SUFFIX

include share/share.mk
include etc/etc.mk

endif # !HAS_PROGRAM_SUFFIX

endif # ENABLE_SMHI


applicationsdir        = $(datarootdir)/applications
dist_applications_DATA = share/applications/metno-${APP}.desktop


include mk/debian.mk

