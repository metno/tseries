# Makefile to distribute tseries-setup into /share/tseries 

# the desktop icon in the gnome menues ---------

applicationsdir        = $(datarootdir)/applications
dist_applications_DATA = \
	share/applications/metno-tseries.desktop


mydocdir        = $(datarootdir)/doc/tseries
dist_mydoc_DATA = \
	share/doc/ts_station.xpm      \
	share/doc/no_news.html	      \
	share/doc/nn_index.html	      \
	share/doc/copy.xpm	      \
	share/doc/tseries.png	      \
	share/doc/ts_find.xpm	      \
	share/doc/tb_refresh.xpm      \
	share/doc/connect.png	      \
	share/doc/index.html	      \
	share/doc/no_index.html	      \
	share/doc/news.html	      \
	share/doc/pil.png	      \
	share/doc/ts_filter.xpm	      \
	share/doc/delete.xpm	      \
	share/doc/tseries.xpm	      \
	share/doc/de_news.html	      \
	share/doc/de_index.html	     



# images to share/pixmaps ----------------------

pixmapsbasedir= $(datarootdir)/pixmaps
dist_pixmapsbase_DATA= \
	share/pixmaps/tseries.png

# images to share/pixmaps/tseries --------------

pixmapsdir= $(datarootdir)/pixmaps/tseries
dist_pixmaps_DATA= \
	share/pixmaps/circle_mixed.rgba         \
	share/pixmaps/cloud.rgba                \
	share/pixmaps/darkPartlyCloud.rgba      \
	share/pixmaps/dark.rgba                 \
	share/pixmaps/fog.rgba                  \
	share/pixmaps/lightCloud.rgba           \
	share/pixmaps/lightRain.rgba            \
	share/pixmaps/lightRainSun.rgba         \
	share/pixmaps/lightRainThunderSun.rgba  \
	share/pixmaps/partlyCloud.rgba          \
	share/pixmaps/rainDark.rgba             \
	share/pixmaps/rain.rgba                 \
	share/pixmaps/RainThunder.rgba          \
	share/pixmaps/sleet.rgba                \
	share/pixmaps/sleetSun.rgba             \
	share/pixmaps/snowDark.rgba             \
	share/pixmaps/snow.rgba                 \
	share/pixmaps/snowSun.rgba              \
	share/pixmaps/SnowThunder.rgba          \
	share/pixmaps/sun.rgba                  \
	share/pixmaps/ts_find.xpm               \
	share/pixmaps/ts_station.xpm            \
	share/pixmaps/weather_01.png            \
	share/pixmaps/weather_02.png            \
	share/pixmaps/weather_03.png            \
	share/pixmaps/weather_04.png            \
	share/pixmaps/weather_05.png            \
	share/pixmaps/weather_06.png            \
	share/pixmaps/weather_07.png            \
	share/pixmaps/weather_08.png            \
	share/pixmaps/weather_09.png            \
	share/pixmaps/weather_10.png            \
	share/pixmaps/weather_11.png            \
	share/pixmaps/weather_12.png            \
	share/pixmaps/weather_13.png            \
	share/pixmaps/weather_14.png            \
	share/pixmaps/weather_15.png            \
	share/pixmaps/weather_16.png            \
	share/pixmaps/weather_17.png            \
	share/pixmaps/weather_18.png            \
	share/pixmaps/weather_19.png            \
	share/pixmaps/weather_999.png

# tseries stylefiles share/tseries/style  ------

styledir        = $(datarootdir)/tseries/style
dist_style_DATA = \
	share/style/style.lameps_t2m            \
	share/style/style.marinogram		 \
	share/style/style.eps_rr24		 \
	share/style/style.t2mqgram		 \
	share/style/style.eps_rrt2m		 \
	share/style/style.rrqgram		 \
	share/style/style.fxqgram		 \
	share/style/style.lameps_ff		 \
	share/style/style.proff_combi		 \
	share/style/style.webogram		 \
	share/style/style.meteogram_batch	 \
	share/style/style.snow_meteogram	 \
	share/style/style.lameps_rr		 \
	share/style/style.eps_rr		 \
	share/style/style.eps_rr24t2m_noens_op	 \
	share/style/style.maritim		 \
	share/style/style.lameps_z500		 \
	share/style/style.meteogram		 \
	share/style/style.eps_z500		 \
	share/style/style.eps_t2m

