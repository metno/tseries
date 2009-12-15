# Makefile to build tseries & btseries 
# $Id$ 
#

SUBDIRS=src

ACLOCAL_AMFLAGS= -I m4


tseriesbindir= $(bindir)
dist_tseriesbin_SCRIPTS= bin/tseries


etcdir        = $(sysconfdir)/${APP}
dist_etc_DATA = etc/${APP}/tseries.ctl

applicationsdir        = $(datarootdir)/applications
dist_applications_DATA = share/applications/metno-${APP}.desktop


if !HAS_PROGRAM_SUFFIX

include share/share.mk
include etc/etc.mk

endif


include mk/debian.mk

