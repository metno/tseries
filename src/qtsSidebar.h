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
#ifndef _qtsSidebar_h
#define _qtsSidebar_h

#include <QLayout>
#include <QWidget>
#include <QStringList>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QLabel>
#include <QPixmap>
#include <QProgressBar>

#include "qtsTimeControl.h"
#include "tsRequest.h"

#include "StationTab.h"
#include "FimexTab.h"

#include <vector>
#include <string>

class ClientSelection;

class qtsSidebar : public QWidget
{
  Q_OBJECT
private:

  QTabWidget  *   tabs;
  TimeControl *   timecontrol;
  ClientSelection*   pluginB;
  QPushButton*    targetB;
  QPushButton*    filterB;
  QPushButton*    observationB;
  QPushButton *   addFimexBookmarkButton;
  QPushButton *   recordFimexButton;
  QPushButton *   expandFimexButton;
  QPushButton *   collapseFimexButton;

  QLabel*         connectStatus;
  QLabel*         obsInfo;
  QLabel*         progressHeader;

  StationTab*     stationtab;
  FimexTab*       fimextab;
  QProgressBar*   progress;

  int stationIdx, fimexIdx;
  int actualIndex;
  bool fimexRexordToggled;
  bool fimexDisabled, hdfDisabled;

private Q_SLOTS:
  void tabChanged(int);
  void recordToggled(bool record);

public Q_SLOTS:
  void searchStation(const QString&);
  void newTimeRange(int,int);
  void currentStationChanged ( QListWidgetItem * current, QListWidgetItem * previous );

public:
  qtsSidebar(QString language);

  QString fillList(const std::vector<std::string>& v, const StationTab::lEntry l);
  QString fillStations(const QStringList& s) { return stationtab->fillStations(s);}

  QString current(const StationTab::lEntry l) { return stationtab->current(l);}
  QString station()               { return stationtab->station(); }
  void set(const std::string& s,const  StationTab::lEntry c) {stationtab->set(s,c);}

  ClientSelection* pluginButton() const {return pluginB;}
  QPushButton*  targetButton() const {return targetB;}
  void setStationInfo(QString s) { stationtab->setStationInfo(s); }
  void setObsInfo(QString s);

  void writeBookmarks();
  void setTab(int idx);
  int getTab() { return tabs->currentIndex(); }

  std::string getTimecontrolLog() { return timecontrol->getTimecontrolLog(); }
  void setTimeControlFromLog(std::string t) { timecontrol->setTimecontrolFromlLog(t); }
  void toggleLockHoursToModel(bool lockHoursToModel) {if(timecontrol) timecontrol->toggleLockHoursToModel(lockHoursToModel);}

  void setObservationsEnabled(bool e) {if(e) observationB->setChecked(e);}
  bool getObservationsEnabled() {return observationB->isChecked();}

  void setCoordinates(float lon, float lat);

  // Fimex

  void enableFimex(bool);

  void setFimexModels(const QStringList& newModels){ fimextab->setModels(newModels);    }
  void setFimexRuns(const QStringList& newRuns)    { fimextab->setRuns(newRuns);        }

  // visible positions (for diana)
  QList<QStringList> getFimexPositions()
    { return fimextab->getPoslist(); }

  // all positions (refresh interpolation)
  std::vector<std::string> allFimexPositions() { return fimextab->allFimexPositions();     }

  void changeFimexPosition( QString newpos) { fimextab->changePosition(newpos);}

  void setProgress(int progr, std::string text);
  void endProgress();

  std::string getFimexExpanded() const { return fimextab->getExpandedDirs(); }
  bool restoreFimexFromLog(std::string mod, std::string sty, std::string expanded);
  miCoordinates fimexCoordinates() const
    { return fimextab->coordinates(); }

Q_SIGNALS:
  void changestyle(const QString&);
  void changemodel(const QString&);
  void changerun(const QString&);
  void changestation(const QString&);
  void filterToggled(bool);
  void observationToggled(bool);
  void minmaxProg(int,int);

  void changetype(const tsRequest::Streamtype);

  // Fimex

  void changeFimexStyle(const QString& );
  void changeFimexModel(const QString& );
  void changeFimexRun(  const QString& );
  void changeFimexCoordinates(float lon, float lat,QString name);
  void changeFimexPoslist();
  void newFimexPoslist();
};

#endif
