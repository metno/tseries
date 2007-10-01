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

#include <fstream>

#include <qlayout.h>
#include <qwidget.h>
#include <qlabel.h>

#include <qtsShow.h>
#include <qtsSidebar.h>

#include <tsSession.h>
#include <tsDatafileColl.h>
#include <tsSetup.h>
#include <tsRequest.h>

#include <miString.h>
#include <miMessage.h>

#include <set>

using namespace std;

const miString DATASET_TSERIES= "T-series ";
const miString TARGETS_TSERIES= "TARGETS_TSERIES";
const miString IMG_STD_TSERIES= "IMG_STD_TSERIES";
const miString IMG_FIN_TSERIES= "IMG_FIN_TSERIES";
const miString IMG_ICON_TSERIES= "IMG_ICON_TSERIES";
const miString NOMODEL_TSERIES= "NONE";
const miString TS_MINE        = " -- ";

class qtsWork: public QWidget 
{
  Q_OBJECT
public:
  qtsWork(QWidget*);

  void collectLog();
  void restoreLog();

  void changeStyle(const miString&);
  void changeModel(const miString&);
  void changeStation(const miString&);
  void changeRun(const miString&);

  set<miString> Filter() const {return filter;}
  set<miString> fullPosList();
  set<miString> createFilter(bool orig=false);

  miMessage getStationList();
  miString  model() const {return request.model();}
  miString  lastList() { return (filterOn ? TS_MINE : "" ) + request.model();}
 
  miMessage target();
  miString  file(const miString typ) const { return request.file(typ);}
  qtsShow*  Show() {return show;} 
  qtsSidebar* sideBar() const {return sidebar;}



public slots:
 
  void changeStyle(const QString&);
  void changeModel(const QString&);
  void changeStation(const QString&);
  void changeRun(const QString&);
  void updateStreams();
  void filterToggled(bool);
  void newFilter(const set<miString>&);


signals:
  void refreshStations();

private:
  qtsSidebar*    sidebar;
  qtsShow*       show;
  SessionManager session;
  DatafileColl   data;
  tsSetup        setup;
  tsRequest      request;

  miString               oldModel;
  map<miString,Model>    modelMap;
  vector<miString>       myStations;
  miMessage              myTarget;
  map<miString,miString> myList;
  set<miString>          filter;

  bool activeRefresh;
  bool filterOn;

  QHBoxLayout* hlayout;

  void Initialise();
  void refresh();
  void makeStationList(bool=false);
  bool makeStyleList();
  bool makeModelList(const miString&);
  bool makeRunList(const miString&);
  bool makeRunList(const miString&,const miString&);
  void restoreModelFromLog();
  
public slots:
  void setProgintervall(int mi,int ma) { show->setProgintervall(mi,ma);refresh();}

 
};
#endif











