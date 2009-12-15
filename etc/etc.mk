# Makefile to distribute stuff into /etc/tseries 

dist_etc_DATA += \
	etc/tseries/symbols.def            \
	etc/tseries/tsDiagrams.batch       \
	etc/tseries/tsDiagrams.def         \
	etc/tseries/tsDiagrams_export.def  \
	etc/tseries/tsDiagramsREGIONS.def  \
	etc/tseries/tseries.ctl            \
	etc/tseries/tseries.ctl_batch      \
	etc/tseries/tseries.ctl_export     \
	etc/tseries/tseries.filter         


betcdir= $(sysconfdir)/btseries
dist_betc_DATA =                                \
	etc/btseries/parameters.def             \
	etc/btseries/style.eps_rr24             \
	etc/btseries/pose.metdat                \
	etc/btseries/tsDiagrams.def             \
	etc/btseries/metgrm_ps.sh               \
	etc/btseries/symbols.def                \
	etc/btseries/style.eps_rr24t2m_noens_op \
	etc/btseries/tseries.setup              \
	etc/btseries/style.meteogram            \
	etc/btseries/style.eps_z500             \
	etc/btseries/style.eps_t2m              \
	etc/btseries/btseries.input

