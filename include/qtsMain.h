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
#include <qtextstream.h>
#include <qprinter.h>
#include <q3mainwindow.h>
#include <q3popupmenu.h>
#include <qmessagebox.h>
#include <qmenubar.h>
#include <q3filedialog.h>
#include <qimage.h>
#include <q3accel.h>
#include <QTimerEvent>
#include <QCloseEvent>
#include <qapplication.h>

#include <qUtilities/ClientButton.h>
#include <qUtilities/miMessage.h>


#include <qtHelpDialog.h>
#include <qtsWork.h>
#include <qtsFilterManager.h>
#include <qtPrintManager.h>
#include <qtsTimeControl.h>

#include <tsConfigure.h>
#include <map>
#include <set>


using namespace std; 

class qtsMain : public Q3MainWindow {
  Q_OBJECT
private:
  qtsWork      * work;
  tsConfigure config;
  
  Q3PopupMenu * menu_setting;
  Q3PopupMenu * menu_file; 
  Q3PopupMenu * menu_help;
  Q3PopupMenu * menu_lang;

  map<int,miString> langID;

  // printerdefinitions
  printerManager pman;
  QPrinter   * printer;

  ClientButton* pluginB;
  QPushButton*  targetB;

  bool dianaconnected;

  HelpDialog* helpdialog;

  miMessage target;

  miString      lang;
  miString      currentModel;  
  set<miString> sendModels;

  void timerEvent(QTimerEvent*);
  int updateTimer;

  int  sOnQuit;
  int  idsnormal;
  int  idsselect;
  int  idsicon;
  int  idsposition;
  int  idtmark;

  bool tmark;
  bool snormal;
  bool sselect;
  bool sicon;
  bool sposition;

  void makeMenuBar();
  void makeFileMenu();
  void makeHelpMenu();
  void makeConnectButtons();
  void makeSettingsMenu();
  void makeAccelerators();
  

  void restoreLog();
  void sendImage(const miString, const QImage&);
  void initHelp();
  void setRemoteParameters();
  void makeEPS(const miString&);
  void setTimemark(miTime);
  void setDianaTimemark(miTime);

  void disablePoslist(miString);
  void enableCurrentPoslist();
  void sendNewPoslist();
protected:
 void closeEvent ( QCloseEvent * );

private slots:  
  void quit();
  void print();
  void raster();
  void about();
  void writeLog();
  void readLog();
  void setSaveOnQuit();
  void refreshDianaStations();
  void sendNamePolicy();

  void toggleNormalNames();
  void toggleSelectedNames();
  void toggleIcon();
  void togglePositions();
  void toggleTimemark();
  void toggleLang(int);

  void processLetter(miMessage&);
  void processConnect();
  void sendTarget();
  void clearTarget();
 
  void showHelp();
  void cleanConnection();

  void manageFilter();
  void chooseFont();
  void findLanguages();

public:
  qtsMain(miString l);

  void setLang(miString l) { lang=l; }

};

#endif





