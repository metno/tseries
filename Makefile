#--------------------------------
# Toplevel Makefile for TIMESERIES
#--------------------------------

# includefile contains LOCALDIR-definition etc.
include ../conf/${OSTYPE}.mk


SRCDIR=src
LIBDIR=lib$(PLT)
OBJDIR=obj$(PLT)
BINDIR=bin$(PLT)
LANGDIR=lang
INCDIR=../include
LOCALINC=$(LOCALDIR)/include

DEPENDSFILE=$(OBJDIR)/make.depends
MOCFILE=$(OBJDIR)/make.moc
DEFINES=-DQT3_SUPPORT 
INCLUDE= -I$(INCDIR) -I$(LOCALINC) \
	-I$(HDF4DIR)/include \
	 $(QTINCLUDE) \
	 $(XINCLUDE)

LINKS = \
	`pkg-config --libs putools`    \
	`pkg-config --libs puctools`   \
	`pkg-config --libs tsdata`     \
	`pkg-config --libs pets2`      \
	`pkg-config --libs pudatatypes`\
	`pkg-config --libs parameter`  \
	`pkg-config --libs pumet`      \
	`pkg-config --libs qutilities` \
	`pkg-config --libs glp`        \
	`pkg-config --libs gltext`     \
	-L$(QTDIR)/lib $(QT_LIBS) \
	$(GLLIBDIR) -lGL -lGLU \
	-L$(HDF4DIR)/lib -ldf -ljpeg -lz -lm $(XLIBDIR) -lXext -lXmu -lXt -lX11  $(XTRALIBS)

BLINKS = \
	-L$(LOCALDIR)/$(LIBDIR) -lpets2 \
	-ltsData -lpuMet -lglText -lglp -lpuTools \
	-lpuDatatypes -lparameter -lpuCtools \
	$(GLLIBDIR) -lGL -lGLU -lpthread -lpng -ljpeg \
	-L$(HDF4DIR)/lib -ldf -lz -lm \
	$(XLIBDIR) -lXext -lXmu -lXt -lX11 -lSM -lICE -lXxf86vm -lm  $(XTRALIBS)

OPTIONS="CXX=${CXX}" "CCFLAGS=${CXXFLAGS} ${DEFINES}" "CC=${CC}" "CFLAGS=${CFLAGS} ${DEFINES}" "LDFLAGS=${CXXLDFLAGS}" "AR=${AR}" "ARFLAGS=${ARFLAGS}" "INCLUDE=${INCLUDE}" "LIBDIR=${LIBDIR}" "DEPENDSFILE=../${DEPENDSFILE}" "BINDIR=../${BINDIR}" "LOCALDIR=${LOCALDIR}" "INCDIR=${INCDIR}" "LINKS=${LINKS}" "BLINKS=${BLINKS}" "MOC=${MOC}" "MOCFILE=../${MOCFILE}" "LANGDIR=../${LANGDIR}" 

all:  directories mocs mark tseries

mocs:
	cd $(OBJDIR); make $(OPTIONS) mocs

depends:
	if [ ! -f $(DEPENDSFILE) ] ; \
	then touch $(DEPENDSFILE) ; fi
	cd $(SRCDIR); make $(OPTIONS) depends

languages:
	if [ ! -d $(LANGDIR) ] ; then mkdir $(LANGDIR) ; fi
	cd $(SRCDIR); make $(OPTIONS) languages 


directories:
	if [ ! -d $(OBJDIR) ] ; then mkdir $(OBJDIR) ; fi
	if [ ! -d $(BINDIR) ] ; then mkdir $(BINDIR) ; fi
	touch $(MOCFILE)
	cd $(OBJDIR);  ln -sf ../$(SRCDIR)/* . ; make $(OPTIONS) version
	if [ ! -f $(DEPENDSFILE) ] ; \
	then touch $(DEPENDSFILE) ; make depends ; fi

tseries:
	cd $(OBJDIR); make $(OPTIONS) all


pretty:
	find . \( -name 'core' -o -name '*~' \) -exec rm -f {} \;

clean:
	@make pretty
	cd $(OBJDIR); rm -f *.o ; rm -f *_moc.cc 

veryclean:
	@make pretty
	rm -rf $(OBJDIR)
	rm -f $(DEPENDSFILE)


mark:	
	@echo "[1;34m = = = = = = = = = = = = = = = = = = = = = = [0m "

# install
COPYFILES=bin/tseries tseries.png tseries.sh tseries.ctl tseries.ctl_batch tseries.ctl_export bin/btseries README tseriesTEST.sh tseriesTEST.ctl
COPYDIRS=doc etc images style batch lang
COPYTREES=
BINLINKS= tseries.sh bin/tseries bin/btseries
DESTNAME=tseries
TESTDESTNAME=tseriesTEST
TESTBINLINKS=tseriesTEST.sh 

include ../conf/install.mk
