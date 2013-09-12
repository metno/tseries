# setup file for pu's time-series viewer

lang=no
Prefix= /disk1/local/tseries/fimex
prefix= /disk1/local/tseries/fimex
Sysconfdir= ${prefix}/etc


Odata=/opdata_local

[VA:FOU:1:2:OFFLINE]
Odata=/opdata
[]

Mdata=$(Odata)/metcorr
Sdata=$(Odata)/roms
Wdata=$(Odata)/wave
Edata=$(Odata)/ec
Gdata=/opdata/glameps
Adata=$(Odata)/arome_norway25
Rdata=$(Odata)/metcorr

[OFFLINE]
Adata=/disk1/opdata_offline/
[]

[VV:VNN:VTK:VNN1:VNN2:VV1:VV2]
Rdata=/opdata/metcorr
[]


day=
[1:VNN1:VV1]
day=-1
[2:VNN2:VV2]
day=-2
[]



#------------------

<Path>

Etc= $(Sysconfdir)/tseries/5.3
Images=$(Prefix)/share/pixmaps/tseries/5.3/
Doc=$(Prefix)/share/doc/tseries-5.3
Lang=$(Prefix)/share/tseries/5.3/lang

#------------------

<files>

Defs=$(Etc)/tsDiagrams.def
Configure=$(Home)/.tseries/tseries.conf
WeatherSymbols=$(Etc)/symbols.def
stdImage=$(Images)/ts_station.xpm
finImage=$(Images)/ts_find.xpm
newImage=$(Images)/ts_new_station.xpm
iconImage=$(Prefix)/share/pixmaps/tseries.png
baseFilter=$(Etc)/tseries.filter
wdbBookmarks=$(Home)/.tseries/bookmarks.wdb
fimexBookmarks=$(Home)/.tseries/bookmarks.fimex
commonBookmarks=$(Etc)/tseries-bookmarks.common

[VNN]
commonBookmarks=$(Etc)/tseries-bookmarks.VNN

[]


<fimex>
FimexStreamTypes= netcdf


<server>

Client=Diana
Name=tseries

Command=$(Prefix)/bin/coserver4

<doc>
mainsource=index.html
mainname=Brukerveiledning
mainlink=START
newssource=news.html
newsname=Nyheter
newslink=


# -------------------------------------------------
#
# -------------------------------------------------

<streams>

# -------------------------------------------------

CollectionName=ECMeteogrammer
PreferredDiagram=Meteogram
InitialOpen=0


DataFile=$(Mdata)/ecatmo12.hdf
DataDescription=ECMWF12
DataType=HDF
Contents=x,x,ECMWF,12

DataFile=$(Mdata)/ecatmo00.hdf
DataDescription=ECMWF00
DataType=HDF
Contents=x,x,ECMWF,00




# -------------------------------------------------
# Proff data streams - for analysis


# raw...

CollectionName=Proff-data
PreferredDiagram=Meteogram
InitialOpen=0

DataFile=/opdata/proff/proff_all.hdf
DataDescription=PROFF
DataType=HDF
Contents=x,x,PROFF.APPROVED,x

[VNN]

CollectionName=Proff-data
PreferredDiagram=Meteogram
InitialOpen=0

DataFile=/opdata_routine/proff/proff_all.hdf
DataDescription=PROFF
DataType=HDF
Contents=x,x,PROFF.APPROVED,x


[]

# -------------------------------------------------


CollectionName=AROME-NorwayMeteogrammer
PreferredDiagram=Meteogram
InitialOpen=0

DataFile=$(Rdata)/AROME_Norway25_00.hdf$(day)
DataDescription=AROME-Norway
DataType=HDF
Contents=x,x,AROME-Norway,0

DataFile=$(Rdata)/AROME_Norway25_06.hdf$(day)
DataDescription=AROME-Norway
DataType=HDF
Contents=x,x,AROME-Norway,6

DataFile=$(Rdata)/AROME_Norway25_12.hdf$(day)
DataDescription=AROME-Norway
DataType=HDF
Contents=x,x,AROME-Norway,12


DataFile=$(Rdata)/AROME_Norway25_18.hdf$(day)
DataDescription=AROME-Norway
DataType=HDF
Contents=x,x,AROME-Norway,18


# ------------------------------------------

CollectionName=AROME-ArcticMeteogrammer
PreferredDiagram=Meteogram
InitialOpe=0

DataFile=$(Rdata)/harmonie25_svalbard_00.hdf$(day)
DataDescription=HARMONIE_2.5km
DataType=HDF
Contents=x,x,HARMONIE_2.5km,0

DataFile=$(Rdata)/harmonie25_svalbard_12.hdf$(day)
DataDescription=HARMONIE_2.5km
DataType=HDF
Contents=x,x,HARMONIE_2.5km,12


# -------------------------------------------------

CollectionName=HIRLAM8Meteogrammer
PreferredDiagram=Meteogram
InitialOpen=0

DataFile=$(Mdata)/hirlam800.hdf$(day)
DataDescription=HIRLAM.8km
DataType=HDF
Contents=x,x,HIRLAM.8km,0

DataFile=$(Mdata)/hirlam806.hdf$(day)
DataDescription=HIRLAM.8km
DataType=HDF
Contents=x,x,HIRLAM.8km,6

DataFile=$(Mdata)/hirlam812.hdf$(day)
DataDescription=HIRLAM.8km
DataType=HDF
Contents=x,x,HIRLAM.8km,12

DataFile=$(Mdata)/hirlam818.hdf$(day)
DataDescription=HIRLAM.8km
DataType=HDF
Contents=x,x,HIRLAM.8km,18



# -------------------------------------------------


CollectionName=HIRLAM12Meteogrammer
PreferredDiagram=Meteogram
InitialOpen=0

DataFile=$(Mdata)/hirlam1200.hdf$(day)
DataDescription=HIRLAM.12km
DataType=HDF
Contents=x,x,HIRLAM.12km,0

DataFile=$(Mdata)/hirlam1206.hdf$(day)
DataDescription=HIRLAM.12km
DataType=HDF
Contents=x,x,HIRLAM.12km,6

DataFile=$(Mdata)/hirlam1212.hdf$(day)
DataDescription=HIRLAM.12km
DataType=HDF
Contents=x,x,HIRLAM.12km,12

DataFile=$(Mdata)/hirlam1218.hdf$(day)
DataDescription=HIRLAM.12km
DataType=HDF
Contents=x,x,HIRLAM.12km,18



# -------------------------------------------------

CollectionName=UM4kmMeteogrammer
PreferredDiagram=Meteogram
InitialOpen=0

[MA:VTK]

DataFile=$(Mdata)/um4km.hdf$(day)
DataDescription=UM.4km
DataType=HDF
Contents=x,x,UM.4km,x

[VV:VNN:VA:FOU:PU]

DataFile=$(Mdata)/um4km00.hdf$(day)
DataDescription=UM.4km
DataType=HDF
Contents=x,x,UM.4km,0

DataFile=$(Mdata)/um4km06.hdf$(day)
DataDescription=UM.4km
DataType=HDF
Contents=x,x,UM.4km,6


DataFile=$(Mdata)/um4km12.hdf$(day)
DataDescription=UM.4km
DataType=HDF
Contents=x,x,UM.4km,12

DataFile=$(Mdata)/um4km18.hdf$(day)
DataDescription=UM.4km
DataType=HDF
Contents=x,x,UM.4km,18


[]
# -------------------------------------------------
# HARMONIE 1 KM

CollectionName=Harmonie1_Trondheim
PreferredDiagram=Meteogram
InitialOpen=0


DataFile=$(Mdata)/har1_trondheim.hdf
DataDescription=HARMONIE.1km.Trondheim
DataType=HDF
Contents=x,x,HARMONIE.1km.Trondheim,x

# --

CollectionName=Harmonie1_East
PreferredDiagram=Meteogram
InitialOpen=0

DataFile=$(Mdata)/har1_east.hdf
DataDescription=HARMONIE.1km.Oslofjord
DataType=HDF
Contents=x,x,HARMONIE.1km.Oslofjord,x

# --

CollectionName=Harmonie1_West
PreferredDiagram=Meteogram
InitialOpen=0

DataFile=$(Mdata)/har1_west.hdf
DataDescription=HARMONIE.1km.Vestlandet
DataType=HDF
Contents=x,x,HARMONIE.1km.Vestlandet,x





# -------------------------------------------------

CollectionName=EPS-produkter
PreferredDiagram=EPS_T2M
InitialOpen=0

DataFile=$(Mdata)/eps12.hdf$(day)
DataDescription=EPS-data
DataType=HDF
Contents=x,x,EPS,12

DataFile=$(Mdata)/eps00.hdf$(day)
DataDescription=EPS-data
DataType=HDF
Contents=x,x,EPS,00


# -------------------------------------------------

# 06

CollectionName=GLAMEPS-produkter
PreferredDiagram=GLAMEPS_TT
InitialOpen=0

DataFile=$(Gdata)/aladeps06.hdf
DataDescription=ALADEPS
DataType=HDF
Contents=x,x,ALADEPS,6

DataFile=$(Gdata)/hireps_s06.hdf
DataDescription=HIREPS_S
DataType=HDF
Contents=x,x,HIREPS_S,6


DataFile=$(Gdata)/hireps_k06.hdf
DataDescription=HIREPS_K
DataType=HDF
Contents=x,x,HIREPS_K,6


DataFile=$(Gdata)/eceps06.hdf
DataDescription=ECEPS
DataType=HDF
Contents=x,x,ECEPS,6

# 18 -

DataFile=$(Gdata)/aladeps18.hdf
DataDescription=ALADEPS
DataType=HDF
Contents=x,x,ALADEPS,18

DataFile=$(Gdata)/hireps_s18.hdf
DataDescription=HIREPS_S
DataType=HDF
Contents=x,x,HIREPS_S,18


DataFile=$(Gdata)/hireps_k18.hdf
DataDescription=HIREPS_K
DataType=HDF
Contents=x,x,HIREPS_K,18


DataFile=$(Gdata)/eceps18.hdf
DataDescription=ECEPS
DataType=HDF
Contents=x,x,ECEPS,18














# -------------------------------------------------


[FOU:PU:VA:VV:VNN:OFFLINE]

CollectionName=Nordic4Havdiagrammer
PreferredDiagram=Havdiagram
InitialOpen=1

DataFile=$(Sdata)/Nordic_4km_quba00.hdf
DataDescription=Nordic_4km
DataType=HDF
Contents=x,x,Nordic_4km,0

DataFile=$(Sdata)/Nordic_4km_quba12.hdf
DataDescription=Nordic_4km 
DataType=HDF
Contents=x,x,Nordic_4km,12
[]

# -------------------------------------------------

[FOU:PU:VA:VV:VNN:OFFLINE]

CollectionName=Nordic4residHavdiagrammer
PreferredDiagram=Havdiagram
InitialOpen=1

DataFile=$(Sdata)/Nordic_4km_resid_quba00.hdf
DataDescription=Nordic_4km_resid
DataType=HDF
Contents=x,x,Nordic_4km_resid,0

DataFile=$(Sdata)/Nordic_4km_resid_quba12.hdf
DataDescription=Nordic_4km_resid
DataType=HDF
Contents=x,x,Nordic_4km_resid,12
[]

# -------------------------------------------------

# Bølgediagrammer

#-------------------
# WAM 50

CollectionName=Bølgediagrammer
PreferredDiagram=Bølgediagram
InitialOpen=1

DataFile=$(Wdata)/wam_quba00.hdf
DataDescription=WAM00
DataType=HDF
Contents=x,x,WAM,0

DataFile=$(Wdata)/wam_quba06.hdf
DataDescription=WAM06
DataType=HDF
Contents=x,x,WAM,06

DataFile=$(Wdata)/wam_quba12.hdf
DataDescription=WAM12
DataType=HDF
Contents=x,x,WAM,12

DataFile=$(Wdata)/wam_quba18.hdf
DataDescription=WAM18
DataType=HDF
Contents=x,x,WAM,18


# -----------------
# WAM 10

CollectionName=Bølgediagrammer
PreferredDiagram=Bølgediagram
InitialOpen=1

DataFile=$(Wdata)/wam_10km_quba00.hdf
DataDescription=WAM_10km
DataType=HDF
Contents=x,x,WAM_10km,0

DataFile=$(Wdata)/wam_10km_quba06.hdf
DataDescription=WAM_10km
DataType=HDF
Contents=x,x,WAM_10km,0


DataFile=$(Wdata)/wam_10km_quba12.hdf
DataDescription=WAM_10km
DataType=HDF
Contents=x,x,WAM_10km,12

DataFile=$(Wdata)/wam_10km_quba18.hdf
DataDescription=WAM_10km
DataType=HDF
Contents=x,x,WAM_10km,0


#-------------------
# WAM 4

CollectionName=Bølgediagrammer
PreferredDiagram=Bølgediagram
InitialOpen=1

DataFile=$(Wdata)/wam_4km_quba00.hdf
DataDescription=WAM_4km
DataType=HDF
Contents=x,x,WAM_4km,0

DataFile=$(Wdata)/wam_4km_quba12.hdf
DataDescription=WAM_4km
DataType=HDF
Contents=x,x,WAM_4km,12



DataFile=$(Edata)/ecwam_quba_00.hdf
DataDescription=ECMWF_WAM
DataType=HDF
Contents=x,x,ECMWF_WAM,0

DataFile=$(Edata)/ecwam_quba_12.hdf
DataDescription=ECMWF_WAM
DataType=HDF
Contents=x,x,ECMWF_WAM,12





# -------------------------------------------------



CollectionName=FimexData
PreferredDiagram=Meteogram
InitialOpen=0




DataFile=$(Adata)/AROME_Norway25_00_DEF.nc
DataDescription=AROME
DataType=netcdf
Contents=x,x,AROME,x
 
DataFile=$(Adata)/AROME_Norway25_06_DEF.nc
DataDescription=AROME
DataType=netcdf
Contents=x,x,AROME,x
 
DataFile=$(Adata)/AROME_Norway25_12_DEF.nc
DataDescription=AROME
DataType=netcdf
Contents=x,x,AROME,x

DataFile=$(Adata)/AROME_Norway25_18_DEF.nc
DataDescription=AROME
DataType=netcdf
Contents=x,x,AROME,x
 


CollectionName=Langtid
PreferredDiagram=Meteogram
InitialOpen=0


DataFile=/opdata/nwparc/eps25/eps25_lqqt_probandltf_2013082612Z.nc
DataDescription=LANGTID
DataType=netcdf
Contents=x,x,LANGTID,00



# -------------------------------------------------


<wdb>
host=proffdb.met.no
[TEST]
host=proffdb-test.met.no
[]

user=proffread
busymovie=$(Images)/busy.gif
readtime=1500
maxRecord=20

<wdbParameter>

MSLP,0,x,x,APP=  air pressure at sea level               | 0   height above ground             :/100
RR,0,x,x,APP=    lwe precipitation rate                  | 0   height above ground
TT,0,x,x,APP=    kalman air temperature                  | 2   height above ground             :-273.15
UU,0,x,x,APP=    x wind                                  | 10  height above ground             :*1.94384
VV,0,x,x,APP=    y wind                                  | 10  height above ground             :*1.94384
CC,0,x,x,APP=    cloud area fraction                     | 1000 atmosphere sigma coordinate
CL,0,x,x,APP=    cloud area fraction in atmosphere layer | 850 atmosphere sigma coordinate
CM,0,x,x,APP=    cloud area fraction in atmosphere layer | 500 atmosphere sigma coordinate
CH,0,x,x,APP=    cloud area fraction in atmosphere layer | 300 atmosphere sigma coordinate


MSLP,0,x,x,RAW=  air pressure at sea level (RAW)                | 0   height above ground             :/100
RR,0,x,x,RAW=    lwe precipitation rate (RAW)                   | 0   height above ground
TT,0,x,x,RAW=    kalman air temperature  (RAW)                  | 2   height above ground             :-273.15
UU,0,x,x,RAW=    x wind (RAW)                                   | 10  height above ground             :*1.94384
VV,0,x,x,RAW=    y wind (RAW)                                   | 10  height above ground             :*1.94384
CC,0,x,x,RAW=    cloud area fraction (RAW)                      | 1000 atmosphere sigma coordinate
CL,0,x,x,RAW=    cloud area fraction in atmosphere layer (RAW)  | 850 atmosphere sigma coordinate
CM,0,x,x,RAW=    cloud area fraction in atmosphere layer (RAW)  | 500 atmosphere sigma coordinate
CH,0,x,x,RAW=    cloud area fraction in atmosphere layer (RAW)  | 300 atmosphere sigma coordinate




<WDBVECTORFUNCTIONS>
UU,0,x,x,APP:VV,0,x,x,APP
UU,0,x,x,RAW:VV,0,x,x,RAW



<klima>
url=http://klapp.oslo.dnmi.no/metnopub/production/metno
maxDistance=50
maxObservationLength=300

<klimaparameter>

TT_O,0,x,OBS,x=TA 
MSLP_O,0,x,OBS,x=PR 
QNH_O,0,x,OBS,x=PH
DD,0,x,OBS,x=DD 
FF,0,x,OBS,x=FF:*1.94384
RR_O,0,x,OBS,x=RR_1
CC,0,x,OBS,x=NN:*12.5
WVFD,0,x,OBS,x=COMPUTE

<klimanormals>

TAM,0,x,OBS,x=TAM
TANM,0,x,OBS,x=TANM
TAXM,0,x,OBS,x=TAXM


<fimexParameters>
# PETSPAR   = TYPE | fimexname                | Unit | extradim1 start1 len1 | extradim2 ...

MSLP,x,x,x = netcdf | air_pressure_at_sea_level         | hPa     
TT,x,x,x   = netcdf | air_temperature_2m                | Celsius
UU,x,x,x   = netcdf | x_wind_10m                        | kt
VV,x,x,x   = netcdf | y_wind_10m                        | kt
FG,x,x,x   = netcdf | fog_area_fraction                 | 0.01
CL,x,x,x   = netcdf | low_type_cloud_area_fraction      | 0.01
CM,x,x,x   = netcdf | medium_type_cloud_area_fraction   | 0.01
CH,x,x,x   = netcdf | high_type_cloud_area_fraction     | 0.01
RRAC,x,x,x = netcdf | precipitation_amount_acc          | kg/m^2


