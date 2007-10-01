# Slave Makefile for TSERIES

CCOBJ= tsSession.o tsSessionOptions.o tsDatafileColl.o tsSetup.o tsRequest.o tsDrawArea.o tsConfigure.o diPrintOptions.o 

COBJ= 

QOBJ= qtsMain.o qtsWork.o qtsSidebar.o qtsShow.o qtPrintManager.o qtHelpDialog.o qtsFilterManager.o qtsTimeControl.o

GOBJ= tseries.o

BOBJ= btseries.o diImageIO.o


#----------------------------------------------------------------------------
CSRC=  $(COBJ:.o=.c)
CCSRC= $(CCOBJ:.o=.cc)
QSRC=  $(QOBJ:.o=.cc)
GSRC=  $(GOBJ:.o=.cc)
BSRC=  $(BOBJ:.o=.cc)

MOCOBJ=$(QOBJ:.o=_moc.o)

ALLSRC=  $(CCSRC) $(CSRC) $(QSRC) $(GSRC) $(BSRC) 
ALLOBJ=  $(CCOBJ) $(COBJ) $(QOBJ) $(GOBJ) $(MOCOBJ)  
BALLOBJ= $(CCOBJ) $(COBJ) $(BOBJ)

#----------------------------------------------------------------------------

.SUFFIXES: .cc .o 

# From .cc to .o
.cc.o:
	$(CXX) $(CCFLAGS) -c $< $(INCLUDE)

# From .c to .o
.c.o:
	$(CC) -c $(CCFLAGS) $< $(INCLUDE)



all: $(BINDIR)/tseries $(BINDIR)/btseries

#----------------------------------------------------------------------------

mocs:	
	echo "#automatic generated moc targets " > $(MOCFILE)
	for ENTRY in $(QOBJ:.o=.h) ; \
	do \
	echo                                                       >> $(MOCFILE) ; \
	echo $${ENTRY%.h}_moc.cc: $(INCDIR)/$${ENTRY}              >> $(MOCFILE) ; \
	echo "	"$(MOC) $(INCDIR)/$${ENTRY} -o $${ENTRY%.h}_moc.cc >> $(MOCFILE) ; \
	done

include $(MOCFILE)

#----------------------------------------------------------------------------

depends:
	rm -f $(DEPENDSFILE)
	echo '# Automatically-generated dependencies list:' > $(DEPENDSFILE)
	$(CXX) $(CCFLAGS) $(INCLUDE) -M $(ALLSRC) >> $(DEPENDSFILE)


-include $(DEPENDSFILE)

#----------------------------------------------------------------------------

LANGUAGES= $(LANGDIR)/tseries_de.ts \
	   $(LANGDIR)/tseries_no.ts \
	   $(LANGDIR)/tseries_fr.ts \
	   $(LANGDIR)/tseries_nn.ts \
	   $(LANGDIR)/tseries_se.ts



languages:
	lupdate  -verbose $(QSRC) -ts $(LANGUAGES)
	cd $(LANGDIR) ; 
	lrelease -verbose $(LANGUAGES)


#----------------------------------------------------------------------------

$(BINDIR)/tseries: $(ALLOBJ) 
	$(CXX) -o $@ $(CCFLAGS) $(ALLOBJ)  $(LINKS)


#-----------------------------------------------------

$(BINDIR)/btseries: $(BALLOBJ) 
	$(CXX) -o $@ $(CCFLAGS) $(BALLOBJ) $(BLINKS)


#-----------------------------------------------------
VERSIONHEADER=../include/tsVersion.h
version:
	@if [ -s $(VERSIONHEADER) ]; then \
	  olddate=`grep version_string $(VERSIONHEADER) | cut -f2 -d\"`; \
	else \
	  olddate="firsttime"; \
	fi; \
	today=`date +%Y-%m-%d`; \
	if [ ! $$olddate = $$today ]; then \
	  rm -f $(VERSIONHEADER); \
	  echo '#ifndef _version_h_' > $(VERSIONHEADER); \
	  echo '#define _version_h_' >> $(VERSIONHEADER); \
	  echo 'const string version_string= "'$$today'";' >> $(VERSIONHEADER); \
	  echo '#endif' >> $(VERSIONHEADER); \
	  chmod 0664 $(VERSIONHEADER); \
	  echo "UPDATED $(VERSIONHEADER) $$today"; \
	else \
	  echo "UNCHANGED $(VERSIONHEADER) $$olddate"; \
	fi


