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
Command=/metno/local/bin/coserver

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
Contents=x,x,HIRLAM.4km,x

DataFile=$(Mdata)/hirlam4km12.hdf
DataDescription=HIRLAM.4km
DataType=HDF
Contents=x,x,HIRLAM.4km,x

# -------------------------------------------------

CollectionName=HIRLAM10Meteogrammer
PreferredDiagram=Meteogram
InitialOpen=0

DataFile=$(Mdata)/hirlam10.hdf
DataDescription=HIRLAM.10km
DataType=HDF
Contents=x,x,HIRLAM.10km,x

# -------------------------------------------------

CollectionName=HIRLAM20Meteogrammer
PreferredDiagram=Meteogram
InitialOpen=0

[VTK]

DataFile=$(Mdata)/hirlam20.hdf
DataDescription=HIRLAM.20km
DataType=HDF
Contents=x,x,HIRLAM.20km,x

[VA:VV:VNN:MA:FOU:PU]

DataFile=$(Mdata)/hirlam2000.hdf
DataDescription=HIRLAM.20km
DataType=HDF
Contents=x,x,HIRLAM.20km,x

DataFile=$(Mdata)/hirlam2006.hdf
DataDescription=HIRLAM.20km
DataType=HDF
Contents=x,x,HIRLAM.20km,x

DataFile=$(Mdata)/hirlam2012.hdf
DataDescription=HIRLAM.20km
DataType=HDF
Contents=x,x,HIRLAM.20km,x

DataFile=$(Mdata)/hirlam2018.hdf
DataDescription=HIRLAM.20km
DataType=HDF
Contents=x,x,HIRLAM.20km,x
[]

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
Contents=x,x,UM.4km,x

DataFile=$(Mdata)/um4km12.hdf
DataDescription=UM.4km
DataType=HDF
Contents=x,x,UM.4km,x

[]



# -------------------------------------------------

CollectionName=UM1kmMeteogrammer
PreferredDiagram=Meteogram
InitialOpen=0


DataFile=$(Mdata)/um1km00.hdf
DataDescription=UM.1km
DataType=HDF
Contents=x,x,UM.1km,x



# -------------------------------------------------

[VA:FOU:PU]
CollectionName=MM5.1kmMeteogrammer
PreferredDiagram=Meteogram
InitialOpen=0

DataFile=$(Odata)/byluft/operational/run/res/OSLO/plot/byluft_pose.hdf
DataDescription=MM5.1km
DataType=HDF
Contents=x,x,MM5.1km,x

DataFile=$(Odata)/byluft/operational/run/res/DRAMMEN/plot/byluft_pose.hdf
DataDescription=MM5.1km
DataType=HDF
Contents=x,x,MM5.1km,x

DataFile=$(Odata)/byluft/operational/run/res/BERGEN/plot/byluft_pose.hdf
DataDescription=MM5.1km
DataType=HDF
Contents=x,x,MM5.1km,x

DataFile=$(Odata)/byluft/operational/run/res/STAVANGER/plot/byluft_pose.hdf
DataDescription=MM5.1km
DataType=HDF
Contents=x,x,MM5.1km,x

DataFile=$(Odata)/byluft/operational/run/res/TRONDHEIM/plot/byluft_pose.hdf
DataDescription=MM5.1km
DataType=HDF
Contents=x,x,MM5.1km,x

DataFile=$(Odata)/byluft/operational/run/res/GRENLAND/plot/byluft_pose.hdf
DataDescription=MM5.1km
DataType=HDF
Contents=x,x,MM5.1km,x
[]

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

[]

# -------------------------------------------------

[VA:VV:VNN:MA:FOU:PU]

CollectionName=Havdiagrammer
PreferredDiagram=Havdiagram
InitialOpen=1

DataFile=$(Sdata)/ecom3d_20km_npl00.hdf
DataDescription=ECOM3D_20km 00 UTC
DataType=HDF
Contents=x,x,ECOM3D_20km,0

DataFile=$(Sdata)/ecom3d_20km_npl12.hdf
DataDescription=ECOM3D_20km 12 UTC
DataType=HDF
Contents=x,x,ECOM3D_20km,12

[]


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

[VA:VV:VNN:MA:FOU:PU]

CollectionName=GamleHavdiagrammer
PreferredDiagram=Havdiagram
InitialOpen=1

DataFile=$(Sdata)/ecom3d.hdf
DataDescription=ECOM3D
DataType=HDF
Contents=x,x,ECOM3D,x
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
