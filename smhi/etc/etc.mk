# Makefile to distribute stuff into /etc/tseries 


dist_etc_DATA = \
	smhi/etc/tseries/tseries.ctl \
	smhi/etc/tseries/tseries.ctl_SMHI \
	smhi/etc/tseries/tsDiagrams_export.def_smhi \
	smhi/etc/tseries/HirlamE11.grib \
	smhi/etc/tseries/HirlamC22.grib


dist_etc_DATA += \
	smhi/etc/tseries/symbols.def            \
	smhi/etc/tseries/tsDiagrams.batch       \
	smhi/etc/tseries/tsDiagrams.def         \
	smhi/etc/tseries/tsDiagrams_export.def  \
	smhi/etc/tseries/tsDiagramsREGIONS.def  \
	smhi/etc/tseries/tseries.ctl_batch      \
	smhi/etc/tseries/tseries.ctl_export     \
	smhi/etc/tseries/tseries.filter         


betcdir= $(sysconfdir)/btseries
dist_betc_DATA =                                     \
	smhi/etc/btseries/parameters.def             \
	smhi/etc/btseries/style.eps_rr24             \
	smhi/etc/btseries/pose.metdat                \
	smhi/etc/btseries/tsDiagrams.def             \
	smhi/etc/btseries/metgrm_ps.sh               \
	smhi/etc/btseries/symbols.def                \
	smhi/etc/btseries/style.eps_rr24t2m_noens_op \
	smhi/etc/btseries/tseries.setup              \
	smhi/etc/btseries/style.meteogram            \
	smhi/etc/btseries/style.eps_z500             \
	smhi/etc/btseries/style.eps_t2m              \
	smhi/etc/btseries/btseries.input

