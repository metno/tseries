# setup file for pu's time-series viewer

lang=no


Odata=/opdata
Mdata=$(Odata)/metcorr
Sdata=$(Odata)/sea
Wdata=$(Odata)/wave
Edata=$(Odata)/ec

#------------------

<Path>
Work=/metno/local/tseries
[PU]
Work=./
[]

Etc=$(Work)/etc
Images=$(Work)/images/
Doc=$(Work)/doc
Lang=$(Work)/lang:${QTDIR}/translations:/metno/local/translations

#------------------

<files>
Defs=$(Etc)/tsDiagrams.def
Configure=$(Home)/.tseries.conf
WeatherSymbols=$(Etc)/symbols.def
stdImage=$(Images)/ts_station.xpm
finImage=$(Images)/ts_find.xpm
iconImage=$(Work)/tseries.png
baseFilter=$(Etc)/tseries.filter


<server>

Client=Diana
Name=tseries

Command=/metno/local/bin/coserver4

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

[VV:PU:FOU:VA:VNN]
DataFile=$(Mdata)/NEW_ecmwf12.hdf
[MA:VTK]
DataFile=$(Mdata)/ecmwf12.hdf
[]
DataDescription=ECMWF12
DataType=HDF
Contents=x,x,ECMWF,12

[VV:PU:FOU:VA:VNN]
DataFile=$(Mdata)/NEW_ecmwf00.hdf
[MA:VTK]
DataFile=$(Mdata)/ecmwf00.hdf
[]
DataDescription=ECMWF00
DataType=HDF
Contents=x,x,ECMWF,00





# -------------------------------------------------

CollectionName=HIRLAM4Meteogrammer
PreferredDiagram=Meteogram
InitialOpen=0



DataFile=$(Mdata)/hirlam4km00.hdf
DataDescription=HIRLAM.4km
DataType=HDF
Contents=x,x,HIRLAM.4km,0

DataFile=$(Mdata)/hirlam4km12.hdf
DataDescription=HIRLAM.4km
DataType=HDF
Contents=x,x,HIRLAM.4km,12





# -------------------------------------------------

CollectionName=HIRLAM8Meteogrammer
PreferredDiagram=Meteogram
InitialOpen=0

DataFile=$(Mdata)/hirlam800.hdf
DataDescription=HIRLAM.8km
DataType=HDF
Contents=x,x,HIRLAM.8km,0

DataFile=$(Mdata)/hirlam806.hdf
DataDescription=HIRLAM.8km
DataType=HDF
Contents=x,x,HIRLAM.8km,6

DataFile=$(Mdata)/hirlam812.hdf
DataDescription=HIRLAM.8km
DataType=HDF
Contents=x,x,HIRLAM.8km,12

DataFile=$(Mdata)/hirlam818.hdf
DataDescription=HIRLAM.8km
DataType=HDF
Contents=x,x,HIRLAM.8km,18



# -------------------------------------------------


CollectionName=HIRLAM12Meteogrammer
PreferredDiagram=Meteogram
InitialOpen=0

DataFile=$(Mdata)/hirlam1200.hdf
DataDescription=HIRLAM.12km
DataType=HDF
Contents=x,x,HIRLAM.12km,0

DataFile=$(Mdata)/hirlam1206.hdf
DataDescription=HIRLAM.12km
DataType=HDF
Contents=x,x,HIRLAM.12km,6

DataFile=$(Mdata)/hirlam1212.hdf
DataDescription=HIRLAM.12km
DataType=HDF
Contents=x,x,HIRLAM.12km,12

DataFile=$(Mdata)/hirlam1218.hdf
DataDescription=HIRLAM.12km
DataType=HDF
Contents=x,x,HIRLAM.12km,18



# -------------------------------------------------

CollectionName=UM4kmMeteogrammer
PreferredDiagram=Meteogram
InitialOpen=0

[VV:VNN:MA:VTK]

DataFile=$(Mdata)/um4km.hdf
DataDescription=UM.4km
DataType=HDF
Contents=x,x,UM.4km,x

[VA:FOU:PU]

DataFile=$(Mdata)/um4km00.hdf
DataDescription=UM.4km
DataType=HDF
Contents=x,x,UM.4km,0

DataFile=$(Mdata)/um4km12.hdf
DataDescription=UM.4km
DataType=HDF
Contents=x,x,UM.4km,12


[VA:VNN:VV:FOU]
CollectionName=UM4km Barentshavet
PreferredDiagram=Meteogram
InitialOpen=0

DataFile=$(Mdata)/um4bx.hdf
DataDescription=UM4BX 
DataType=HDF
Contents=x,x,UM4BX,x

[]


# -------------------------------------------------

CollectionName=UM1kmMeteogrammer
PreferredDiagram=Meteogram
InitialOpen=0


DataFile=$(Mdata)/um1km00.hdf
DataDescription=UM.1km
DataType=HDF
Contents=x,x,UM.1km,0

# -------------------------------------------------

CollectionName=UM1kmExpMeteogrammer
PreferredDiagram=Meteogram
InitialOpen=0


DataFile=$(Odata)/um1exp/um1exp00.hdf
DataDescription=UM1.exp 
DataType=HDF
Contents=x,x,UM1.exp,x





# -------------------------------------------------

CollectionName=EPS-produkter
PreferredDiagram=EPS_T2M
InitialOpen=0

DataFile=$(Mdata)/eps12.hdf
DataDescription=EPS-data
DataType=HDF
Contents=x,x,EPS,12

DataFile=$(Mdata)/eps00.hdf
DataDescription=EPS-data
DataType=HDF
Contents=x,x,EPS,00


# -------------------------------------------------

[VA:VV:VNN:MA:FOU:PU]

CollectionName=LAMEPS-produkter
PreferredDiagram=LAMEPS_T2M
InitialOpen=0

DataFile=$(Mdata)/lameps18.hdf
DataDescription=LAMEPS-data
DataType=HDF
Contents=x,x,LAMEPS,18


DataFile=$(Mdata)/lameps06.hdf
DataDescription=LAMEPS-data
DataType=HDF
Contents=x,x,LAMEPS,06

[]

# -------------------------------------------------


[FOU:PU:VA:VV:VNN]

CollectionName=Nordic4Havdiagrammer
PreferredDiagram=Havdiagram
InitialOpen=1

DataFile=$(Sdata)/Nordic_4km_npl00.hdf
DataDescription=Nordic_4km
DataType=HDF
Contents=x,x,Nordic_4km,0

DataFile=$(Sdata)/Nordic_4km_npl12.hdf
DataDescription=Nordic_4km 
DataType=HDF
Contents=x,x,Nordic_4km,12
[]

# -------------------------------------------------

[FOU:PU:VA:VV:VNN]

CollectionName=Nordic4residHavdiagrammer
PreferredDiagram=Havdiagram
InitialOpen=1

DataFile=$(Sdata)/Nordic_4km_resid_npl00.hdf
DataDescription=Nordic_4km_resid
DataType=HDF
Contents=x,x,Nordic_4km_resid,0

DataFile=$(Sdata)/Nordic_4km_resid_npl12.hdf
DataDescription=Nordic_4km_resid
DataType=HDF
Contents=x,x,Nordic_4km_resid,12
[]

# -------------------------------------------------



[VA:VV:VNN:MA:FOU:PU]

CollectionName=Bølgediagrammer
PreferredDiagram=Bølgediagram
InitialOpen=1

DataFile=$(Wdata)/wam_npl00.hdf
DataDescription=WAM00
DataType=HDF
Contents=x,x,WAM,0

DataFile=$(Wdata)/wam_npl06.hdf
DataDescription=WAM06
DataType=HDF
Contents=x,x,WAM,06

DataFile=$(Wdata)/wam_npl12.hdf
DataDescription=WAM12
DataType=HDF
Contents=x,x,WAM,12

DataFile=$(Wdata)/wam_npl18.hdf
DataDescription=WAM18
DataType=HDF
Contents=x,x,WAM,18

DataFile=$(Wdata)/wam_npl_ec12.hdf
DataDescription=WAMEC
DataType=HDF
Contents=x,x,WAM_EC,12

DataFile=$(Edata)/ecwam_npl_00.hdf
DataDescription=ECMWF_WAM
DataType=HDF
Contents=x,x,ECMWF_WAM,0

DataFile=$(Edata)/ecwam_npl_12.hdf
DataDescription=ECMWF_WAM
DataType=HDF
Contents=x,x,ECMWF_WAM,12
[]

# -------------------------------------------------



[VA:VV:VNN:MA:FOU:PU]

CollectionName=Bølgediagrammer
PreferredDiagram=Bølgediagram
InitialOpen=1

DataFile=$(Wdata)/wam_10km_npl00.hdf
DataDescription=WAM_10km
DataType=HDF
Contents=x,x,WAM_10km,0

DataFile=$(Wdata)/wam_10km_npl12.hdf
DataDescription=WAM_10km
DataType=HDF
Contents=x,x,WAM_10km,12
[]

# -------------------------------------------------


[VA:VV:VNN:MA:FOU:PU]

# Subjective meteogram

CollectionName=Subjective
PreferredDiagram=S-Meteogram
InitialOpen=0

DataFile=$(Mdata)/quba_subj09.hdf
DataDescription=SUBJ
DataType=HDF
Contents=x,x,SUBJ,09

DataFile=$(Mdata)/quba_subj19.hdf
DataDescription=SUBJ
DataType=HDF
Contents=x,x,SUBJ,19

[]

# -------------------------------------------------

[VA:VV:VNN:MA:FOU:PU]

# Subjective maritime meteogram

CollectionName=Maritim
PreferredDiagram=MariProd
InitialOpen=1

DataFile=$(Mdata)/maritim_subj06.hdf
DataDescription=Maritim
DataType=HDF
Contents=x,x,SUBJ,06

[]

# -------------------------------------------------

[VA:VV:VNN:MA:FOU:PU]

CollectionName=rr24q
PreferredDiagram=Boksplott-Døgnnedbør
InitialOpen=0

DataFile=$(Odata)/statprog/rr24_nn+lqr_ec/rr24_nn+lqr_ec.hdf
DataDescription=RR24Q
DataType=HDF
Contents=x,x,rr24,x

# -------------------------------------------------

CollectionName=fx10q
PreferredDiagram=Boksplott-Maks.vind
InitialOpen=0

DataFile=$(Odata)/statprog/fx10-6h_lqr_ec/fx10-6h_lqr_ec.hdf
DataDescription=FX10Q
DataType=HDF
Contents=x,x,fx10-6h,x

# -------------------------------------------------

CollectionName=t2mq
PreferredDiagram=Boksplott-Temperatur2m
InitialOpen=0

DataFile=$(Odata)/statprog/t2m_lqr_ec/t2m_lqr_ec.hdf
DataDescription=T2mQ
DataType=HDF
Contents=x,x,t2m,x

[]
