# Makefile to build tseries & btseries 

bin_PROGRAMS= tseries.bin

# sources --------------------------

cc_sources=                 \
	tsSession.cc        \
	tsSessionOptions.cc \
	tsDatafileColl.cc   \
	tsSetup.cc          \
	tsRequest.cc        \
	tsConfigure.cc      \
	diPrintOptions.cc   

common_sources = tsDrawArea.cc  PrepareDataThread.cc        
common_moc_sources = $(common_sources:.cc=.moc.cc)

# Qt sources ------------------------

qt_sources=                \
	qtsMain.cc         \
	qtsWork.cc         \
	qtsSidebar.cc      \
	qtsShow.cc         \
	qtPrintManager.cc  \
	qtsFilterManager.cc\
	qtsTimeControl.cc  \
	StationTab.cc      \
	CoordinateTab.cc   \
	FimexTab.cc        \
	WdbCacheThread.cc  \
	CoordinateManager.cc \
	WdbBookmarkTools.cc  \
	ParameterFilterDialog.cc \
	PopupCalendar.cc  \
	ClearLineEdit.cc  \
	FetchStations.cc


# headers ( for distribution )  -----

header_sources=             \
	$(cc_sources:.cc=.h)\
	$(qt_sources:.cc=.h)\
	$(common_sources:.cc=.h) \
	diImageIO.h         \
	diPrintOptions.h


# Generated Qt source code 
moc_sources = $(qt_sources:.cc=.moc.cc) $(common_moc_sources)
%.moc.cc: %.h
	$(MOC4) -o $@ $<


# Languages -------------------------

language_sources= \
	$(top_srcdir)/lang/tseries_de.ts \
	$(top_srcdir)/lang/tseries_fr.ts \
	$(top_srcdir)/lang/tseries_nn.ts \
	$(top_srcdir)/lang/tseries_no.ts \
	$(top_srcdir)/lang/tseries_se.ts

languagedir= $(datarootdir)/tseries/@PVERSION@/lang
nodist_language_DATA=$(language_sources:.ts=.qm)


common_cppflags =
if ENABLE_GRIBFILE
common_cppflags += -DGRIBSTREAM
endif

%.qm: %.ts  $(qt_sources)
	$(LUPDATE4)  $(qt_sources) -ts $< ; \
	$(LRELEASE4)  $<


# images for distribution -----------

img_sources=               \
	copy.xpm           \
	delete.xpm         \
	filter.xpm         \
	tb_close.xpm       \
	tb_left_arrow.xpm  \
	tb_no.xpm          \
	tb_refresh.xpm     \
	tb_right_arrow.xpm \
	tb_save.xpm        \
	tb_yes.xpm         \
	tseries.xpm        \
	ts_filter.xpm      \
	ts_find.xpm        \
	bookmark.xpm       \
	view-refresh.xpm   \
	list-add.xpm       \
	directory.xpm      \
	media-record.xpm   \
	metno.xpm          \
	trashcan.xpm       \
	synop.xpm          \
	expand.xpm         \
	collapse.xpm       \
	clear.xpm          \
	search_small.xpm   \
	search_folder.xpm


# Flags -----------------------------

common_cppflags +=              \
	$(PETS2_CPPFLAGS)       \
	$(PUMET_CPPFLAGS)       \
	$(PUDATATYPES_CPPFLAGS) \
	$(COSERVER_CPPFLAGS)    \
	$(QT4_CPPFLAGS)         \
	$(FIMEX_CPPFLAGS) -DFIMEX 

common_ldflags=                \
	$(PETS2_LDFLAGS)       \
	$(PUMET_LDFLAGS)       \
	$(PUDATATYPES_LDFLAGS) \
	$(COSERVER_LDFLAGS)    \
	$(QT4_LDFLAGS)         \
	$(FIMEX_LDFLAGS) -DFIMEX 

common_ldadd=               \
	$(PETS2_LIBS)       \
	$(PUMET_LIBS)       \
	$(PUDATATYPES_LIBS) \
	$(COSERVER_LIBS)    \
	$(QT4_LIBS) \
	$(FIMEX_LIBS) 


# building tseries ------------------

tseries_bin_SOURCES=        \
	tseries.cc          \
	$(header_sources)   \
	$(cc_sources)       \
	$(qt_sources)       \
	$(common_sources)   \
	$(img_sources)      \
	$(language_sources)

nodist_tseries_bin_SOURCES = \
	$(moc_sources) 

tseries_bin_CXXFLAGS= $(common_cppflags)
tseries_bin_LDFLAGS=  $(common_ldflags)
tseries_bin_LDADD=    $(common_ldadd)


# building btseries -----------------

## bin_PROGRAMS += btseries
##
## btseries_SOURCES=         \
## 	btseries.cc       \
## 	diImageIO.cc      \
## 	$(header_sources) \
## 	$(common_sources) \
## 	$(cc_sources)
##
## nodist_btseries_SOURCES = \
## 	$(common_moc_sources)
##
## btseries_CXXFLAGS= $(common_cppflags)
## btseries_LDFLAGS=  $(common_ldflags)
## btseries_LDADD=    $(common_ldadd)

# generated file must be cleaned afterwards...

CLEANFILES = $(moc_sources) $(nodist_language_DATA) $(common_moc_sources)

# Preserve timestamps under install --

INSTALL = install -p
