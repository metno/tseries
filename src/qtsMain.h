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
#ifndef _qtsMain_h
#define _qtsMain_h

#include <QTextStream>
#include <QPrinter>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QImage>
#include <QTimerEvent>
#include <QCloseEvent>
#include <QApplication>
#include <QActionGroup>
#include <QShortcut>

#include <qUtilities/ClientButton.h>
#include <qUtilities/miMessage.h>
#include <qUtilities/qtHelpDialog.h>

#include "qtsWork.h"
#include "qtsFilterManager.h"
#include "qtPrintManager.h"
#include "qtsTimeControl.h"

#include "tsConfigure.h"

#include <map>
#include <set>


class qtsMain : public QMainWindow {
  Q_OBJECT
private:
  qtsWork   * work;
  tsConfigure config;

  // Menues
  QMenu   * menu_help;
  QAction * showHelpAct;
  QAction * aboutAct;

  QMenu   * menu_file;
  QAction * printAct;
  QAction * rasterAct;
  QAction * filterAct;
  QAction * quitAct;

  QMenu   * menu_setting;
  QAction * readLogAct;
  QAction * writeLogAct;
  QAction * sOnQuitAct;
  QAction * normalAct;
  QAction * selectAct;
  QAction * iconAct;
  QAction * positionAct;
  QAction * tmarkAct;
  QAction * latlonAct;
  QAction * fontAct;
  QAction * lockHoursToModelAct;
  QAction * filterParametersAct;
  QAction * observationStartAct;
  QAction * showGridLinesAct;

  QMenu        * menu_lang;
  QActionGroup * languageGroup;

  QShortcut * nextModelSc;
  QShortcut * prevModelSc;


  // languages
  std::map<int,miutil::miString> langID;

  // printerdefinitions
  printerManager pman;
  QPrinter   * printer;

  ClientButton* pluginB;
  QPushButton*  targetB;

  bool dianaconnected;

  HelpDialog* helpdialog;

  miMessage target;

  miutil::miString      lang;
  miutil::miString      currentModel;
  miutil::miTime        currentTime;
  std::set<miutil::miString> sendModels;


  void timerEvent(QTimerEvent*);
  int updateTimer;

  bool tmark;
  bool snormal;
  bool sselect;
  bool sicon;
  bool sposition;
  bool latlond;
  bool lockHoursToModel;
  bool showGridLines;

  void makeMenuBar();
  void makeFileMenu();
  void makeHelpMenu();
  void makeConnectButtons();
  void makeSettingsMenu();

  void restoreLog();
  void sendImage(const miutil::miString, const QImage&);
  void initHelp();
  void setRemoteParameters();
  void makeEPS(const miutil::miString&);
  void setTimemark(miutil::miTime);
  void setDianaTimemark(miutil::miTime);

  void disablePoslist(miutil::miString);
  void enableCurrentPoslist();
  void sendNewPoslist();

  std::string lastFimexPosition;


protected:
 void closeEvent ( QCloseEvent * );

private slots:
  void quit();
  void print();
  void raster();
  void about();
  void writeLog();
  void readLog();
  void setSaveOnQuit(bool);
  void refreshDianaStations();
  void sendNamePolicy();

  void toggleNormalNames(bool);
  void toggleSelectedNames(bool);
  void toggleIcon(bool);
  void togglePositions(bool);
  void toggleTimemark(bool);
  void toggleLang(QAction*);
  void toggleLatLon(bool);
  void toggleLockHoursToModel(bool);
  void toggleShowGridlines(bool);

  void processLetter(const miMessage&);
  void processConnect();
  void sendTarget();
  void clearTarget();
  void clearFimexList();
  void showHelp();
  void cleanConnection();

  void manageFilter();
  void manageParameterFilter();
  void changeObservationStart();
  void chooseFont();
  void findLanguages();

  void selectionTypeChanged();
  void coordinatesChanged();
  void fimexPoslistChanged();
  void fimexPositionChanged(const QString&);
public:
  qtsMain(miutil::miString l);

  void setLang(miutil::miString l) { lang=l; }

};

#endif





