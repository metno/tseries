# Makefile to distribute tseries-setup into /share/tseries 


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
