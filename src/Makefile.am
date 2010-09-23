# Makefile to build tseries & btseries 
# $Id$ 
#

bin_PROGRAMS= tseries.bin btseries

# sources --------------------------

cc_sources=                 \
	tsSession.cc        \
	tsSessionOptions.cc \
	tsDatafileColl.cc   \
	tsSetup.cc          \
	tsRequest.cc        \
	tsDrawArea.cc       \
	tsConfigure.cc      \
	diPrintOptions.cc   



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
	WdbCacheThread.cc  \
	CoordinateManager.cc \
	WdbBookmarkTools.cc  

# headers ( for distribution )  -----

header_sources=             \
	$(cc_sources:.cc=.h)\
	$(qt_sources:.cc=.h)\
	diImageIO.h         \
	diPrintOptions.h


# Generated Qt source code 
moc_sources = $(qt_sources:.cc=.moc.cc)
%.moc.cc: %.h
	$(MOC4) -o $@ $<


# Languages -------------------------

language_sources= \
	$(top_srcdir)/lang/tseries_de.ts \
	$(top_srcdir)/lang/tseries_fr.ts \
	$(top_srcdir)/lang/tseries_nn.ts \
	$(top_srcdir)/lang/tseries_no.ts \
	$(top_srcdir)/lang/tseries_se.ts

if !HAS_PROGRAM_SUFFIX
languagedir= $(datarootdir)/tseries/lang
nodist_language_DATA=$(language_sources:.ts=.qm)
endif

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
	trashcan.xpm


# Flags -----------------------------

common_cppflags +=              \
	$(PETS2_CPPFLAGS)       \
	$(PUMET_CPPFLAGS)       \
	$(GLTEXT_CPPFLAGS)      \
	$(PUDATATYPES_CPPFLAGS) \
	$(QUTILITIES_CPPFLAGS)  \
	${HDF4_CPPFLAGS}        \
	$(QT4_CPPFLAGS) 

common_ldflags=                \
	$(PETS2_LDFLAGS)       \
	$(PUMET_LDFLAGS)       \
	$(GLTEXT_LDFLAGS)      \
	$(PUDATATYPES_LDFLAGS) \
	$(QUTILITIES_LDFLAGS)  \
	${HDF4_LDFLAGS}        \
	$(QT4_LDFLAGS) 

common_ldadd=               \
	$(PETS2_LIBS)       \
	$(PUMET_LIBS)       \
	$(GLTEXT_LIBS)      \
	$(PUDATATYPES_LIBS) \
	$(QUTILITIES_LIBS)  \
	${HDF4_LIBS}        \
	$(QT4_LIBS) 


# building tseries ------------------

tseries_bin_SOURCES=        \
	tseries.cc          \
	$(header_sources)   \
	$(cc_sources)       \
	$(qt_sources)       \
	$(img_sources)      \
	$(language_sources)

nodist_tseries_bin_SOURCES = \
	$(moc_sources)

tseries_bin_CXXFLAGS= $(common_cppflags)
tseries_bin_LDFLAGS=  $(common_ldflags)
tseries_bin_LDADD=    $(common_ldadd)


# building btseries -----------------

btseries_SOURCES=         \
	btseries.cc       \
	diImageIO.cc      \
	$(header_sources) \
	$(cc_sources)

btseries_CXXFLAGS= $(common_cppflags)
btseries_LDFLAGS=  $(common_ldflags)
btseries_LDADD=    $(common_ldadd)


# generated file must be cleaned afterwards...

CLEANFILES = $(moc_sources) $(nodist_language_DATA)


# Preserve timestamps under install --

INSTALL = install -p
