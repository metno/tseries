/*
  Tseries - A Free Meteorological Timeseries Viewer

  $Id$

  Copyright (C) 2006 met.no

  Contact information:
  Norwegian Meteorological Institute
  Box 43 Blindern
  0313 OSLO
  NORWAY
  email: diana@met.no

  This file is part of Tseries

  Tseries is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Tseries is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Tseries; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef _qtsWork_h
#define _qtsWork_h



#include <QLayout>
#include <QWidget>
#include <QLabel>
#include <QString>
#include <QHBoxLayout>

#include "qtsSidebar.h"
#include "qtsShow.h"

#include "tsSession.h"
#include "tsDatafileColl.h"
#include "tsSetup.h"
#include "tsRequest.h"


#include <puTools/miString.h>
#include <coserver/miMessage.h>
#include <puDatatypes/miCoordinates.h>
#include <puTools/miString.h>

#include <string>
#include <vector>
#include <set>


const miutil::miString DATASET_TSERIES= "T-series ";
const miutil::miString DATASET_FIMEX=   "T-series Fimex";
const miutil::miString TARGETS_TSERIES= "TARGETS_TSERIES";
const miutil::miString IMG_STD_TSERIES= "IMG_STD_TSERIES";
const miutil::miString IMG_FIN_TSERIES= "IMG_FIN_TSERIES";
const miutil::miString IMG_NEW_TSERIES= "IMG_NEW_TSERIES";
const miutil::miString IMG_ACTIVE_TSERIES= "IMG_ACTIVE_TSERIES";
const miutil::miString IMG_ICON_TSERIES= "IMG_ICON_TSERIES";
const miutil::miString NOMODEL_TSERIES= "NONE";
const miutil::miString TS_MINE        = " -- ";

class qtsWork: public QWidget
{
  Q_OBJECT

public:
  enum SelectionType{SELECT_BY_STATION,SELECT_BY_WDB, SELECT_BY_FIMEX};

private:
  qtsSidebar*    sidebar;
  qtsShow*       show;
  SessionManager session;
  DatafileColl   data;
  tsSetup        setup;
  tsRequest      request;
  SelectionType  selectionType;
  unsigned int   maxWDBreadTime;
  miutil::miString               oldModel;
  std::map<miutil::miString,Model>    modelMap;
  std::map<miutil::miString,Model>    fimexModelMap;
  std::vector<std::string>            myStations;
  miMessage              myTarget;
  std::map<miutil::miString,miutil::miString> myList;
  std::set<miutil::miString>          filter;


  bool reading_data;
  bool activeRefresh;
  bool filterOn;
  bool latlonInDecimal;
  bool has_wdb_stream;
  bool has_fimex_stream;

  void Initialise();

  void makeStationList(bool  = false);
  bool makeStyleList();
  bool makeModelList(const miutil::miString&);
  bool makeRunList(const miutil::miString&);
  bool makeRunList(const miutil::miString&,const miutil::miString&);
  void restoreModelFromLog();
  void checkPosition(miutil::miString st);
  void checkObsPosition(miCoordinates cor);

  // WDB ------
  void makeWdbModels();

public:
  qtsWork(QWidget*);

  void collectLog();
  void restoreLog();
  void refresh(bool readData = false);
  void changeStyle(const miutil::miString&);
  void changeModel(const miutil::miString&);
  void changeStation(const miutil::miString&);
  void changeRun(const miutil::miString&);
  void updateProgress();


  std::set<miutil::miString> Filter() const {return filter;}
  std::set<miutil::miString> fullPosList();
  std::set<miutil::miString> createFilter(bool orig=false);
  miMessage getFimexStationList();
  miMessage getStationList();
  miutil::miString  model() const {return request.model();}
  miutil::miString  lastList() { return (filterOn ? TS_MINE : "" ) + request.model();}

  miMessage target();
  miutil::miString  file(const miutil::miString typ) const { return request.file(typ);}
  qtsShow*  Show() {return show;}
  qtsSidebar* sideBar() const {return sidebar;}

  void changePositions(const miutil::miString&);
  SelectionType getSelectionType() const {return selectionType;};
  void toggleLockHoursToModel(bool lockHoursToModel) { if(sidebar) sidebar->toggleLockHoursToModel(lockHoursToModel);}
  void setShowGridLines( bool s ){ if(show) show->setShowGridLines(s); }

  std::set<std::string>    getKlimaBlacklist() const { return data.getKlimaBlacklist();}
  std::vector<std::string> getAllKlimaParameters() const {return data.getAllKlimaParameters();}
  void  setKlimaBlackList(std::set<std::string>& bl);

  void setObservationStartTime(miutil::miTime st) {show->setObservationStartTime(st); refresh(true);}
  miutil::miTime getObservationStartTime() const { return show->getObservationStartTime(); }



  public slots:

  void changeStyle(const QString&);
  void changeModel(const QString&);
  void changeStation(const QString&);
  void changeRun(const QString&);
  void updateStreams();
  void filterToggled(bool);
  void newFilter(const std::set<miutil::miString>&);
  void latlonInDecimalToggled(bool);
  void refreshFinished();
  //void setProgintervall(int mi,int ma) { show->setProgintervall(mi,ma);refresh(true);}
  void setProgintervall(int mi,int ma) { show->setProgintervall(mi,ma);refresh(false);}
  void observationToggled(bool showobs) { show->setShowObservations(showobs);refresh(true);}


  // WDB
  void changeWdbModel(const QString&);
  void changeType(const tsRequest::Streamtype);
  void changeWdbStyle(const QString&);
  void changeWdbRun(const QString&);
  void changeCoordinates(float lon, float lat, QString name);
  void requestWdbCacheQuery();
  void cacheRequestDone();

  // FIMEX

  void changeFimexModel(const QString&);
  void changeFimexStyle(const QString&);
  void changeFimexRun(const QString&);
  void changeFimexCoordinates(float lon, float lat, QString name);
  void makeFimexModels(const QString& activeStyle);
  void newFimexPoslist();
  void dataread_started();
  void dataread_ended();

  signals:
  void refreshStations();
  void selectionTypeChanged();
  void coordinatesChanged();
  void fimexPoslistChanged();
  void fimexPositionChanged(const QString&);

};
#endif











