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
LANGDEST=share/lang
INCDIR=../include
LOCALINC=$(LOCALDIR)/include
VERSION=4.0.2

LOCALINC=$(LOCALDIR)/include

DEPENDSFILE=$(OBJDIR)/make.depends
MOCFILE=$(OBJDIR)/make.moc
DEFINES=-DQT3_SUPPORT 
INCLUDE= -I$(INCDIR) -I$(LOCALINC) \
	-I$(HDF4DIR)/include \
	 $(QTINCLUDE) \
	 $(XINCLUDE)

ifdef STATIC
LFLAG=--static
else
LFLAG=--libs

endif

LINKS = \
	`pkg-config $(LFLAG) putools`    \
	`pkg-config $(LFLAG) puctools`   \
	`pkg-config $(LFLAG) tsdata`     \
	`pkg-config $(LFLAG) pets2`      \
	`pkg-config $(LFLAG) pudatatypes`\
	`pkg-config $(LFLAG) parameter`  \
	`pkg-config $(LFLAG) pumet`      \
	`pkg-config $(LFLAG) qutilities` \
	`pkg-config $(LFLAG) glp`        \
	`pkg-config $(LFLAG) gltext`     \
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

OPTIONS="CXX=${CXX}" "CCFLAGS=${CXXFLAGS} ${DEFINES}" "CC=${CC}" "CFLAGS=${CFLAGS} ${DEFINES}" "LDFLAGS=${CXXLDFLAGS}" "AR=${AR}" "ARFLAGS=${ARFLAGS}" "INCLUDE=${INCLUDE}" "LIBDIR=${LIBDIR}" "DEPENDSFILE=../${DEPENDSFILE}" "BINDIR=../${BINDIR}" "LOCALDIR=${LOCALDIR}" "INCDIR=${INCDIR}" "LINKS=${LINKS}" "BLINKS=${BLINKS}" "MOC=${MOC}" "MOCFILE=../${MOCFILE}" "LANGDIR=../${LANGDIR}"  "LANGDEST=../${LANGDEST}" "VERSION=\"${VERSION}\""

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
COPYDIRS=bin
COPYTREES=share etc
DESTNAME=tseries-${VERSION}

DEBIANNAME=tseries_${VERSION}_i386
COPYDEBIAN= share etc bin

include ../conf/install.mk
