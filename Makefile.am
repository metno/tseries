AUTOMAKE_OPTIONS=foreign
SUBDIRS=src



# tell automake where to find the m4 
ACLOCAL_AMFLAGS= -I m4

tseriesbindir= $(bindir)
dist_tseriesbin_SCRIPTS= \
	 bin/tseries


include mk/debian.mk



if ENABLE_MYTEST
include beta/beta.mk
else
include share/share.mk
include etc/etc.mk
endif


