# Makefile to distribute tseries-setup into /share/tseries 


mydocdir        = $(datarootdir)/doc/tseries-@PVERSION@
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
	share/doc/de_index.html	      \
	share/doc/view-refresh.xpm    \
	share/doc/busy.gif            \
	share/doc/list-add.xpm


# images to share/pixmaps ----------------------

pixmapsbasedir= $(datarootdir)/pixmaps
dist_pixmapsbase_DATA= \
	share/pixmaps/tseries.png

# images to share/pixmaps/tseries --------------

pixmapsdir= $(datarootdir)/pixmaps/tseries/@PVERSION@
dist_pixmaps_DATA= \
	share/pixmaps/tseries.png               \
	share/pixmaps/ts_find.xpm               \
	share/pixmaps/ts_station.xpm            \
	share/pixmaps/ts_active.xpm             \
	share/pixmaps/ts_new_station.xpm        \
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
	share/pixmaps/weather_999.png           \
	share/pixmaps/busy.gif
