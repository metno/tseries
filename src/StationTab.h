/*
 * StationTab.h
 *
 *  Created on: Mar 15, 2010
 *      Author: juergens
 */

#ifndef StationTab_H_
#define StationTab_H_

/*
 Copyright (C) 2006 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 email: diana@met.no

 This file is part of generated by met.no

 This is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Tseries; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include <QLayout>
#include <QListWidget>
#include <QLineEdit>
#include <QWidget>
#include <QStringList>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>

#include <vector>
#include <string>


class StationTab : public QWidget {
  Q_OBJECT
public:
  enum lEntry { CMMODEL, CMSTYLE, CMRUN, CMFIMEXSTYLE, CMFIMEXMODEL, CMFIMEXRUN };

private:
  QLineEdit   * searchw;    // search widget (stations)
  QListWidget * statl;      // list of stations
  QComboBox   * modell;     // list of models
  QComboBox   * stylel;     // list of styles (meteogram etc.)
  QComboBox   * runl;       // list of runs
  QLabel      * pos_label;

public:
  StationTab(QWidget* parent);

  QString fillList(const QStringList& qlist , const StationTab::lEntry);
  QString fillStations(const QStringList&);

  QString current(const lEntry);
  QString station();
  void set(const std::string& s,const StationTab::lEntry c);
  void setStationInfo(QString s) { pos_label->setText(s); }

public slots:
  void searchStation(const QString&);
  void currentStationChanged ( QListWidgetItem * current, QListWidgetItem * previous );


signals:
  void changestyle(const QString&);
  void changemodel(const QString&);
  void changerun(const QString&);
  void changestation(const QString&);

};

#endif /* StationTab_H_ */
