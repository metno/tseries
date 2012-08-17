# Makefile to build tseries & btseries 
# $Id$ 
#

SUBDIRS=src

ACLOCAL_AMFLAGS= -I m4


tseriesbindir= $(bindir)
dist_tseriesbin_SCRIPTS= bin/tseries


etctseriesdir = $(sysconfdir)/tseries/$(PVERSION)


if ENABLE_SMHI

include smhi/etc/etc.mk

else

dist_etctseries_DATA = etc/tseries/tseries.ctl etc/tseries/tsDiagrams.def      

include share/share.mk
include etc/etc.mk

endif # ENABLE_SMHI


applicationsdir        = $(datarootdir)/applications
dist_applications_DATA = share/applications/metno-tseries.desktop


install-data-hook:
	cd $(DESTDIR)/$(applicationsdir) && mv metno-tseries.desktop metno-tseries-@PVERSION@.desktop
	cd $(DESTDIR)/$(pixmapsbasedir) && mv tseries.png tseries-@PVERSION@.png


