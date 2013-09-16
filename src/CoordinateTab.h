/*
 * CoordinateTab.h
 *
 *  Created on: Mar 16, 2010
 *      Author: juergens
 */

#ifndef COORDINATETAB_H_
#define COORDINATETAB_H_

/*
 $Id$

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
#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QDial>
#include <QLabel>
#include <QPushButton>
#include <QTreeView>
#include <QString>
#include <QStandardItemModel>
#include "WdbBookmarkTools.h"
#include "CoordinateManager.h"
#include <puTools/miString.h>
#include <vector>


class CoordinateTab : public QWidget {
  Q_OBJECT
public:
  enum TabType { WDBTAB, FIMEXTAB };

private:
  QComboBox   * modell;     // list of models
  QComboBox   * stylel;     // list of styles (meteogram etc.)
  QComboBox   * runl;       // list of runs

  QTreeView   * bookmarks;
  QPushButton * cacheQueryButton;

  TabType tabtype;

  CoordinateManager * latitude;
  CoordinateManager * longitude;
  bool                activeCacheRequest;
  WdbBookmarkTools    bookmarkTools;
  QStandardItemModel* model;
  std::string         variableBookmarkfile;
  bool recordingPositions;
public:
  CoordinateTab(QWidget*,CoordinateTab::TabType ttype);

  QString setStyles(const QStringList& qlist);
  void setWdbGeometry(int minLon, int maxLon, int minLat, int maxLat);

  miutil::miString coordinateString();
  void setActiveCacheRequest(bool b) { activeCacheRequest=b;}
  bool getActiveCacheRequest() const { return activeCacheRequest;}
  void writeBookmarks();
  std::vector<std::string> getPoslist();
  bool findPosition(QString newpos, QModelIndex& idx);

  std::string getExpandedDirs();
  void setExpandedDirs(std::string);
  std::vector<std::string> allFimexPositions() {return  bookmarkTools.getAllBookmarks();}


private slots:

  void coordinatesChanged();
  void changeModel(const QString&);
  void changeRun(const QString&);
  void bookmarkClicked(QModelIndex idx);
  void poslistChanged(const QModelIndex &);

public slots:
  void setCoordinates(float lon, float lat, QString name="");
  void setModel(QString model);
  void setStyle(QString style);
  void setRun(QString run);


  void setLatRange(int min, int max);
  void setLonRange(int min, int max);

  void setModels(const QStringList& newmodels);
  void setRuns(const QStringList& newmodels);
  void addBookmarkFolder();
  void changePosition(QString);
  void recordToggled(bool);


  signals:
    void changestyle(const QString&);
    void changemodel(const QString&);
    void changerun(const QString&);
    void changeCoordinates(float lon, float lat,QString name);
    void changePoslist();
    void newPoslist();


};

#endif /* COORDINATETAB_H_ */
