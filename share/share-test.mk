# Makefile to distribute tseries-setup into /share/tseries 



# tseries stylefiles share/tseries/style  ------

styledir        = $(datarootdir)/tseries-test/style
dist_style_DATA = \
	share/style-test/style.lameps_t2m            \
	share/style-test/style.marinogram		 \
	share/style-test/style.eps_rr24		 \
	share/style-test/style.t2mqgram		 \
	share/style-test/style.eps_rrt2m		 \
	share/style-test/style.rrqgram		 \
	share/style-test/style.fxqgram		 \
	share/style-test/style.lameps_ff		 \
	share/style-test/style.proff_combi		 \
	share/style-test/style.webogram		 \
	share/style-test/style.meteogram_batch	 \
	share/style-test/style.snow_meteogram	 \
	share/style-test/style.lameps_rr		 \
	share/style-test/style.eps_rr		 \
	share/style-test/style.eps_rr24t2m_noens_op	 \
	share/style-test/style.maritim		 \
	share/style-test/style.lameps_z500		 \
	share/style-test/style.meteogram		 \
	share/style-test/style.eps_z500		 \
	share/style-test/style.eps_t2m                \
	share/style-test/style.proff_edit
