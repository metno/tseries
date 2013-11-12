# Makefile to build tseries & btseries 
# $Id$ 
#

SUBDIRS=src

ACLOCAL_AMFLAGS= -I m4


tseriesbindir= $(bindir)
dist_tseriesbin_SCRIPTS= bin/tseries



if ENABLE_SMHI

# include smhi/etc/etc.mk
# all setup is now seperated into https://svn.met.no/tseries/config
# consider to establish an extra branch for SMHI specific stuff there
#

else

include share/share.mk

endif # ENABLE_SMHI


applicationsdir        = $(datarootdir)/applications
dist_applications_DATA = share/applications/metno-tseries.desktop


install-data-hook:
	cd $(DESTDIR)/$(applicationsdir) && mv metno-tseries.desktop metno-tseries-@PVERSION@.desktop
	cd $(DESTDIR)/$(pixmapsbasedir) && mv tseries.png tseries-@PVERSION@.png


