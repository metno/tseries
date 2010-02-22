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
#include <QHBoxLayout>

#include "qtsSidebar.h"
#include "qtsShow.h"

#include "tsSession.h"
#include "tsDatafileColl.h"
#include "tsSetup.h"
#include "tsRequest.h"

#include <puTools/miString.h>
#include <qUtilities/miMessage.h>

#include <set>

using namespace std;

const miutil::miString DATASET_TSERIES= "T-series ";
const miutil::miString TARGETS_TSERIES= "TARGETS_TSERIES";
const miutil::miString IMG_STD_TSERIES= "IMG_STD_TSERIES";
const miutil::miString IMG_FIN_TSERIES= "IMG_FIN_TSERIES";
const miutil::miString IMG_ICON_TSERIES= "IMG_ICON_TSERIES";
const miutil::miString NOMODEL_TSERIES= "NONE";
const miutil::miString TS_MINE        = " -- ";

class qtsWork: public QWidget
{
  Q_OBJECT
public:
  qtsWork(QWidget*);

  void collectLog();
  void restoreLog();

  void changeStyle(const miutil::miString&);
  void changeModel(const miutil::miString&);
  void changeStation(const miutil::miString&);
  void changeRun(const miutil::miString&);

  set<miutil::miString> Filter() const {return filter;}
  set<miutil::miString> fullPosList();
  set<miutil::miString> createFilter(bool orig=false);

  miMessage getStationList();
  miutil::miString  model() const {return request.model();}
  miutil::miString  lastList() { return (filterOn ? TS_MINE : "" ) + request.model();}

  miMessage target();
  miutil::miString  file(const miString typ) const { return request.file(typ);}
  qtsShow*  Show() {return show;}
  qtsSidebar* sideBar() const {return sidebar;}



public slots:

  void changeStyle(const QString&);
  void changeModel(const QString&);
  void changeStation(const QString&);
  void changeRun(const QString&);
  void updateStreams();
  void filterToggled(bool);
  void newFilter(const set<miutil::miString>&);
  void latlonInDecimalToggled(bool);

signals:
  void refreshStations();

private:
  qtsSidebar*    sidebar;
  qtsShow*       show;
  SessionManager session;
  DatafileColl   data;
  tsSetup        setup;
  tsRequest      request;

  miutil::miString               oldModel;
  map<miutil::miString,Model>    modelMap;
  vector<miutil::miString>       myStations;
  miMessage              myTarget;
  map<miutil::miString,miString> myList;
  set<miutil::miString>          filter;

  bool activeRefresh;
  bool filterOn;
  bool latlonInDecimal;


  QHBoxLayout* hlayout;

  void Initialise();
  void refresh(bool readData = false);
  void makeStationList(bool=false);
  bool makeStyleList();
  bool makeModelList(const miutil::miString&);
  bool makeRunList(const miutil::miString&);
  bool makeRunList(const miutil::miString&,const miString&);
  void restoreModelFromLog();
  void checkPosition(miutil::miString st);

public slots:
  void setProgintervall(int mi,int ma) { show->setProgintervall(mi,ma);refresh();}


};
#endif











