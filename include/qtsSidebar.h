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
#ifndef _qtsSidebar_h
#define _qtsSidebar_h

#include <qlayout.h>
#include <q3listbox.h>
#include <qlineedit.h>
#include <qwidget.h>
#include <qstringlist.h>
#include <qcombobox.h>
#include <Q3VBoxLayout>
#include <qpushbutton.h>

#include <qtsTimeControl.h>

#include <vector>
#include <puTools/miString.h>
#include <qUtilities/ClientButton.h>




using namespace std;

class qtsSidebar : public QWidget
{
  Q_OBJECT
public:
  enum lEntry { CMMODEL, CMSTYLE,CMRUN };

private:
  Q3VBoxLayout * vlayout;
  QLineEdit   * searchw;    // search widget (stations)

  Q3ListBox   * statl;      // list of stations
  QComboBox   * modell;     // list of models
  QComboBox   * stylel;     // list of styles (meteogram etc.)
  QComboBox   * runl;       // list of runs
  QLabel      * pos_label;  // position information

  TimeControl * timecontrol;
  ClientButton* pluginB;
  QPushButton*  targetB;
  QPushButton*  filterB;

public slots:
  void searchStation(const QString&);

  void iterateModel(int);

  void nextModel() {iterateModel( 1);}
  void prevModel() {iterateModel(-1);}


public:
  qtsSidebar(QWidget*);

  QString fillList(const vector<miString>&, const lEntry);
  QString fillStations(const QStringList&);

  QString current(const lEntry);
  QString station();
  void set(const miString& s,const lEntry c);

  ClientButton* pluginButton() const {return pluginB;}
  QPushButton*  targetButton() const {return targetB;}
  void setStationInfo(QString s) { pos_label->setText(s); }


signals:
  void changestyle(const QString&);
  void changemodel(const QString&);
  void changerun(const QString&);
  void changestation(const QString&);
  void filterToggled(bool);
  void minmaxProg(int,int);
};

#endif










