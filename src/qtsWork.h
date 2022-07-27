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

extern const QString DATASET_FIMEX;
extern const QString TARGETS_TSERIES;
extern const QString IMG_STD_TSERIES;
extern const QString IMG_FIN_TSERIES;
extern const QString IMG_NEW_TSERIES;
extern const QString IMG_ACTIVE_TSERIES;
extern const QString IMG_ICON_TSERIES;
extern const QString NOMODEL_TSERIES;


class qtsWork: public QWidget
{
  Q_OBJECT

private:
  qtsSidebar*    sidebar;
  qtsShow*       show;
  SessionManager session;
  DatafileColl   data;
  tsSetup        setup;
  tsRequest      request;
  QString               oldModel;
  std::map<std::string,Model>    modelMap;
  std::map<std::string,Model>    fimexModelMap;
  QList<QStringList>             myStations;
  std::map<std::string, miCoordinates> myList;

  bool reading_data;
  bool activeRefresh;

  void Initialise();

  bool makeStyleList();
  void checkObsPosition(miCoordinates cor);

  QString lastList() const;

public:
  qtsWork(QWidget*, QString language);

  void collectLog();
  void restoreLog();
  void refresh(bool readData = false);
  void changeStation(const QString&);
  void updateProgress();

  miQMessage getStationList();

  miQMessage target();
  std::string  file(const std::string typ) const { return request.file(typ);}
  qtsShow*  Show() {return show;}
  qtsSidebar* sideBar() const {return sidebar;}

  void changePositions(float lon, float lat);
  void toggleLockHoursToModel(bool lockHoursToModel) { if(sidebar) sidebar->toggleLockHoursToModel(lockHoursToModel);}
  void setShowGridLines( bool s ){ if(show) show->setShowGridLines(s); }

  std::set<std::string>    getKlimaBlacklist() const { return data.getKlimaBlacklist();}
  std::vector<std::string> getAllKlimaParameters() const {return data.getAllKlimaParameters();}
  void  setKlimaBlackList(std::set<std::string>& bl);

  void setObservationStartTime(miutil::miTime st) {show->setObservationStartTime(st); refresh(true);}
  miutil::miTime getObservationStartTime() const { return show->getObservationStartTime(); }

  void updateStreams();

public Q_SLOTS:
  void setProgintervall(int mi,int ma) { show->setProgintervall(mi,ma);refresh(false);}
  void observationToggled(bool showobs) { show->setShowObservations(showobs);refresh(true);}

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
