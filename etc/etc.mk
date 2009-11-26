etcdir = $(sysconfdir)/tseries
dist_etc_DATA = \
	tseries/symbols.def            \
	tseries/tsDiagrams.batch       \
	tseries/tsDiagrams.def         \
	tseries/tsDiagrams_export.def  \
	tseries/tsDiagramsREGIONS.def  \
	tseries/tseries.ctl            \
	tseries/tseries.ctl_batch      \
	tseries/tseries.ctl_export     \
	tseries/tseries.filter         


betcdir= $(sysconfdir)/btseries
dist_betc_DATA =                              \
	btseries/parameters.def               \
	btseries/style.eps_rr24		      \
	btseries/pose.metdat		      \
	btseries/tsDiagrams.def		      \
	btseries/metgrm_ps.sh		      \
	btseries/symbols.def		      \
	btseries/style.eps_rr24t2m_noens_op   \
	btseries/tseries.setup		      \
	btseries/style.meteogram              \
	btseries/style.eps_z500		      \
	btseries/style.eps_t2m		      \
	btseries/btseries.input
