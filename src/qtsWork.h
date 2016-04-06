/*
  Tseries - A Free Meteorological Timeseries Viewer

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

#include <coserver/miMessage.h>
#include <puDatatypes/miCoordinates.h>

#include <string>
#include <vector>
#include <set>


const std::string DATASET_TSERIES= "T-series ";
const std::string DATASET_FIMEX=   "T-series Fimex";
const std::string TARGETS_TSERIES= "TARGETS_TSERIES";
const std::string IMG_STD_TSERIES= "IMG_STD_TSERIES";
const std::string IMG_FIN_TSERIES= "IMG_FIN_TSERIES";
const std::string IMG_NEW_TSERIES= "IMG_NEW_TSERIES";
const std::string IMG_ACTIVE_TSERIES= "IMG_ACTIVE_TSERIES";
const std::string IMG_ICON_TSERIES= "IMG_ICON_TSERIES";
const std::string NOMODEL_TSERIES= "NONE";
const std::string TS_MINE        = " -- ";

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
  std::string               oldModel;
  std::map<std::string,Model>    modelMap;
  std::map<std::string,Model>    fimexModelMap;
  std::vector<std::string>            myStations;
  miMessage              myTarget;
  std::map<std::string,std::string> myList;
  std::set<std::string>          filter;


  bool reading_data;
  bool activeRefresh;
  bool filterOn;
  bool latlonInDecimal;
  bool has_wdb_stream;
  bool has_fimex_stream;

  void Initialise();

  void makeStationList(bool  = false);
  bool makeStyleList();
  bool makeModelList(const std::string&);
  bool makeRunList(const std::string&);
  bool makeRunList(const std::string&,const std::string&);
  void restoreModelFromLog();
  void checkPosition(std::string st);
  void checkObsPosition(miCoordinates cor);

  // WDB ------
  void makeWdbModels();

public:
  qtsWork(QWidget*, QString language);

  void collectLog();
  void restoreLog();
  void refresh(bool readData = false);
  void changeStyle(const std::string&);
  void changeModel(const std::string&);
  void changeStation(const std::string&);
  void changeRun(const std::string&);
  void updateProgress();


  std::set<std::string> Filter() const {return filter;}
  std::set<std::string> fullPosList();
  std::set<std::string> createFilter(bool orig=false);
  miMessage getFimexStationList();
  miMessage getStationList();
  std::string  model() const {return request.model();}
  std::string  lastList() { return (filterOn ? TS_MINE : "" ) + request.model();}

  miMessage target();
  std::string  file(const std::string typ) const { return request.file(typ);}
  qtsShow*  Show() {return show;}
  qtsSidebar* sideBar() const {return sidebar;}

  void changePositions(const std::string&);
  SelectionType getSelectionType() const {return selectionType;};
  void toggleLockHoursToModel(bool lockHoursToModel) { if(sidebar) sidebar->toggleLockHoursToModel(lockHoursToModel);}
  void setShowGridLines( bool s ){ if(show) show->setShowGridLines(s); }

  std::set<std::string>    getKlimaBlacklist() const { return data.getKlimaBlacklist();}
  std::vector<std::string> getAllKlimaParameters() const {return data.getAllKlimaParameters();}
  void  setKlimaBlackList(std::set<std::string>& bl);

  void setObservationStartTime(miutil::miTime st) {show->setObservationStartTime(st); refresh(true);}
  miutil::miTime getObservationStartTime() const { return show->getObservationStartTime(); }


public Q_SLOTS:
  void changeStyle(const QString&);
  void changeModel(const QString&);
  void changeStation(const QString&);
  void changeRun(const QString&);
  void updateStreams();
  void filterToggled(bool);
  void newFilter(const std::set<std::string>&);
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

Q_SIGNALS:
  void refreshStations();
  void selectionTypeChanged();
  void coordinatesChanged();
  void fimexPoslistChanged();
  void fimexPositionChanged(const QString&);

};

#endif
