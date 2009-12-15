# Makefile to build tseries & btseries 
# $Id$ 
#

SUBDIRS=src



# tell automake where to find the m4 
ACLOCAL_AMFLAGS= -I m4

tseriesbindir= $(bindir)
dist_tseriesbin_SCRIPTS= \
	 bin/tseries


if HAS_PROGRAM_SUFFIX

include beta/beta.mk

else

include share/share.mk
include etc/etc.mk

endif


include mk/debian.mk

