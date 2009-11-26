bin_PROGRAMS= tseries.bin btseries


common_cc_sources= tsSession.cc    \
	tsSessionOptions.cc \
	tsDatafileColl.cc   \
	tsSetup.cc          \
	tsRequest.cc        \
	tsDrawArea.cc       \
	tsConfigure.cc 
	diPrintOptions.cc 


common_header_sources= diImageIO.h \
	diPrintOptions.h    \
	qtHelpDialog.h      \
	qtPrintManager.h    \
	qtsFilterManager.h  \
	qtsMain.h           \
	qtsShow.h           \
	qtsSidebar.h        \
	qtsTimeControl.h    \
	qtsWork.h           \
	tsConfigure.h       \
	tsDatafileColl.h    \
	tsDrawArea.h        \
	tsRequest.h         \
	tsSession.h         \
	tsSessionOptions.h  \
	tsSetup.h

# Qt sources
qt_sources= qtsMain.cc     \
	qtsWork.cc         \
	qtsSidebar.cc      \
	qtsShow.cc         \
	qtPrintManager.cc  \
	qtHelpDialog.cc    \
	qtsFilterManager.cc\
	qtsTimeControl.cc

# Generated Qt source code 
moc_sources = $(qt_sources:.cc=.moc.cc)
CLEANFILES += $(moc_sources)
%.moc.cc: %.h
	$(MOC4) -o $@ $<


# images for distribution 
img_sources= copy.xpm      \
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
	ts_find.xpm



btseries_SOURCES= btseries.cc diImageIO.cc \
	$(common_header_sources) \
	$(common_cc_sources)


tseries_bin_SOURCES= tseries.cc \
	$(common_header_sources) \
	$(common_cc_sources) \
	$(qt_sources)        \
	$(img_sources)



nodist_tseries_bin_SOURCES = \
	$(moc_sources)


common_cppflags=

common_ldflags=


tseries_bin_CXXFLAGS= $(common_cppflags)
btseries_CXXFLAGS=    $(common_cppflags)

tseries_bin_LDFLAGS= $(common_ldflags)
btseries_LDFLAGS=    $(common_ldflags)


INSTALL = install -p
