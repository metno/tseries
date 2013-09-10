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
#include "qtsMain.h"

#include <QTimerEvent>
#include <QPixmap>
#include <QCloseEvent>
#include <QFileInfo>
#include <QFontDialog>
#include <QPrintDialog>
#include <QStringList>

#include "config.h"
#include "ParameterFilterDialog.h"
#include "PopupCalendar.h"
#include <qUtilities/QLetterCommands.h>
#include <puTools/ttycols.h>
#include <puTools/miDate.h>

#include <iomanip>
#include <iostream>
#include <map>

#include "tseries.xpm"

using namespace std;
using namespace miutil;

const miString thisTM = "MARKEDTIME";
const miString dianaTM = "DIANATIME";

qtsMain::qtsMain(miString l) :
    lang(l), QMainWindow()

{
// Added to avoid unnessecary updates when connected to diana
  // and diana is in automatic update mode
  currentTime = miTime::nowTime();

  latlond = false;
  makeMenuBar();
  dianaconnected = false;

  work = new qtsWork(this);
  setCentralWidget(work);

  makeConnectButtons();

  work->latlonInDecimalToggled(latlond);

  toggleLockHoursToModel(lockHoursToModel);
  toggleShowGridlines(showGridLines);

  printer = new QPrinter(QPrinter::HighResolution);

  setWindowIcon(QPixmap(tseries_xpm));

  initHelp();

  restoreLog();
  setRemoteParameters();
  setTimemark(currentTime);

  connect(work, SIGNAL(selectionTypeChanged()), this,
      SLOT(selectionTypeChanged()));
  connect(work, SIGNAL(coordinatesChanged()), this, SLOT(coordinatesChanged()));

  // milliseconds
  int updatetimeout = (1000 * 60) * 2;

  updateTimer = startTimer(updatetimeout);

}

void qtsMain::makeMenuBar()
{
  makeFileMenu();
  makeSettingsMenu();
  makeHelpMenu();
}

void qtsMain::makeHelpMenu()
{
  menu_help = menuBar()->addMenu(tr("Help"));

  showHelpAct = new QAction(tr("Manual"), this);
  showHelpAct->setShortcut(tr("F1"));
  showHelpAct->setStatusTip(tr("Show manual"));
  connect(showHelpAct, SIGNAL(triggered()), this, SLOT( showHelp() ));
  menu_help->addAction(showHelpAct);

  aboutAct = new QAction(tr("About"), this);
  connect(aboutAct, SIGNAL(triggered()), this, SLOT( about() ));
  menu_help->addAction(aboutAct);

}

void qtsMain::makeFileMenu()
{
  menu_file = menuBar()->addMenu(tr("File"));

  printAct = new QAction(tr("Print"), this);
  printAct->setShortcut(tr("Ctrl+P"));
  printAct->setStatusTip(tr("Print diagram"));
  connect(printAct, SIGNAL(triggered()), this, SLOT( print() ));
  menu_file->addAction(printAct);

  rasterAct = new QAction(tr("Save Image"), this);
  connect(rasterAct, SIGNAL(triggered()), this, SLOT( raster() ));
  menu_file->addAction(rasterAct);

  filterAct = new QAction(tr("Change filter"), this);
  connect(filterAct, SIGNAL(triggered()), this, SLOT( manageFilter() ));
  menu_file->addAction(filterAct);

  filterParametersAct = new QAction(tr("Change Observation filter"), this);
  connect(filterParametersAct, SIGNAL(triggered()), this,
      SLOT( manageParameterFilter() ));
  menu_file->addAction(filterParametersAct);

  observationStartAct = new QAction(tr("Change Observation start date"), this);
  connect(observationStartAct, SIGNAL(triggered()), this,
      SLOT( changeObservationStart() ));
   menu_file->addAction(observationStartAct);


  // -------------------

  menu_file->addSeparator();

  quitAct = new QAction(tr("quit"), this);
  quitAct->setShortcut(tr("Ctrl+Q"));
  quitAct->setStatusTip(tr("Quit program"));
  connect(quitAct, SIGNAL(triggered()), this, SLOT( quit() ));
  menu_file->addAction(quitAct);

}

void qtsMain::makeSettingsMenu()
{
  menu_setting = menuBar()->addMenu(tr("Preferences"));

  // -------------------

  readLogAct = new QAction(tr("Reset Preferences"), this);
  connect(readLogAct, SIGNAL(triggered()), this, SLOT( readLog() ));
  menu_setting->addAction(readLogAct);

  writeLogAct = new QAction(tr("Save Preferences"), this);
  connect(writeLogAct, SIGNAL(triggered()), this, SLOT( writeLog() ));
  menu_setting->addAction(writeLogAct);

  menu_setting->addSeparator();

  // --------------------

  sOnQuitAct = new QAction(tr("Save at exit"), this);
  sOnQuitAct->setCheckable(true);

  bool isOn;
  config.get("SAVEONQUIT", isOn);
  sOnQuitAct->setChecked(isOn);
  connect(sOnQuitAct, SIGNAL(toggled(bool)), this, SLOT( setSaveOnQuit(bool) ));
  menu_setting->addAction(sOnQuitAct);
  menu_setting->addSeparator();

  // ---------------------

  config.get("SHOWNORMAL", snormal);
  normalAct = new QAction(tr("Show positions (DIANA)"), this);
  normalAct->setCheckable(true);
  normalAct->setChecked(snormal);
  connect(normalAct, SIGNAL(toggled(bool)), this,
      SLOT(toggleNormalNames(bool)));
  menu_setting->addAction(normalAct);

  config.get("SHOWSELECT", sselect);
  selectAct = new QAction(tr("Show active position (DIANA)"), this);
  selectAct->setCheckable(true);
  selectAct->setChecked(sselect);
  connect(selectAct, SIGNAL(toggled(bool)), this,
      SLOT(toggleSelectedNames(bool)));
  menu_setting->addAction(selectAct);

  config.get("SHOWICON", sicon);
  iconAct = new QAction(tr("Show icons (DIANA)"), this);
  iconAct->setCheckable(true);
  iconAct->setChecked(sicon);
  connect(iconAct, SIGNAL(toggled(bool)), this, SLOT(toggleIcon(bool)));
  menu_setting->addAction(iconAct);

  sposition = true; //no logging
  positionAct = new QAction(tr("Send positions (DIANA)"), this);
  positionAct->setCheckable(true);
  positionAct->setChecked(sposition);
  connect(positionAct, SIGNAL(toggled(bool)), this,
      SLOT(togglePositions(bool)));
  menu_setting->addAction(positionAct);

  menu_setting->addSeparator();

  // ------------------------

  config.get("TIMEMARK", tmark);
  tmarkAct = new QAction(tr("Show timemark"), this);
  tmarkAct->setCheckable(true);
  tmarkAct->setChecked(tmark);
  connect(tmarkAct, SIGNAL(toggled(bool)), this, SLOT(toggleTimemark(bool)));
  menu_setting->addAction(tmarkAct);

  config.get("SHOWGRIDLINES", showGridLines);
  showGridLinesAct = new QAction(tr("Show Gridlines "), this);
  showGridLinesAct->setCheckable(true);
  showGridLinesAct->setChecked(showGridLines);
  connect(showGridLinesAct, SIGNAL(toggled(bool)), this,
      SLOT(toggleShowGridlines(bool)));
  menu_setting->addAction(showGridLinesAct);

  config.get("LATLONDEC", latlond);
  latlonAct = new QAction(tr("Lat/Lon in decimal"), this);
  latlonAct->setCheckable(true);
  latlonAct->setChecked(latlond);
  connect(latlonAct, SIGNAL(toggled(bool)), this, SLOT(toggleLatLon(bool)));
  menu_setting->addAction(latlonAct);

  menu_setting->addSeparator();





  config.get("LOCKHOURSTOMODEL", lockHoursToModel);
  lockHoursToModelAct = new QAction(tr("Lock Hours to Model "), this);
  lockHoursToModelAct->setCheckable(true);
  lockHoursToModelAct->setChecked(lockHoursToModel);
  connect(lockHoursToModelAct, SIGNAL(toggled(bool)), this,
      SLOT(toggleLockHoursToModel(bool)));
  menu_setting->addAction(lockHoursToModelAct);


  // ------------------------

  fontAct = new QAction(tr("Font"), this);
  connect(fontAct, SIGNAL(triggered()), this, SLOT(chooseFont()));
  menu_setting->addAction(fontAct);
  menu_setting->addSeparator();

  // ------------------------

  menu_lang = menu_setting->addMenu(tr("Languages"));
  findLanguages();

}

void qtsMain::makeConnectButtons()
{

  connect(work, SIGNAL(refreshStations()), this, SLOT(refreshDianaStations()));

  pluginB = work->sideBar()->pluginButton();
  targetB = work->sideBar()->targetButton();

  connect(targetB, SIGNAL(pressed()), this, SLOT(sendTarget()));
  connect(targetB, SIGNAL(released()), this, SLOT(clearTarget()));

  connect(pluginB, SIGNAL(receivedMessage(const miMessage&)),
      SLOT(processLetter(const miMessage&)));
  connect(pluginB, SIGNAL(addressListChanged()), SLOT(processConnect()));
  connect(pluginB, SIGNAL(connectionClosed()), SLOT(cleanConnection()));
}

void qtsMain::closeEvent(QCloseEvent * e)
{
  quit();
}

void qtsMain::quit()
{
  bool soq;
  config.get("SAVEONQUIT", soq);

  if (soq)
    writeLog();
  qApp->quit();
}

void qtsMain::raster()
{
  miString format = "PNG";
  miString fname = "./" + work->file("png");

  QString fpath = fname.c_str();
  QString fcaption = "save file dialog";
  QString fpattern = "Pictures (*.png *.xpm *.bmp *.eps);;All (*.*)";

  QString s = QFileDialog::getSaveFileName(this, fcaption, fpath, fpattern);

  if (s.isNull())
    return;

  QFileInfo finfo(s);
  fpath = finfo.absolutePath();

  fname = s.toStdString();

  int quality = -1;

  cerr << "Saving: " << fname << endl;

  if (fname.contains(".xpm") || fname.contains(".XPM"))
    format = "XPM";
  else if (fname.contains(".bmp") || fname.contains(".BMP"))
    format = "BMP";
  else if (fname.contains(".eps") || fname.contains(".epsf")) {
    makeEPS(fname);
    return;
  }

  QImage img = work->Show()->grabFrameBuffer(true);
  img.save(fname.c_str(), format.c_str(), quality);
}

void qtsMain::print()
{
  miString command;

#ifdef linux
  command = "lpr -h -r -{hash}{numcopies} -P {printer} {filename}";
#else
  command= "lp -c -n{numcopies} -d {printer} {filename}";
#endif

  printOptions priop;

  miString fname = work->file("ps"); //"tseries_temp.ps"

  QString ofn = printer->outputFileName();

  if (ofn.isNull()) {
    QFileInfo p(fname.c_str());
    printer->setOutputFileName(p.absoluteFilePath());
  } else {
    QFileInfo p(ofn);
    printer->setOutputFileName(p.path() + "/" + fname.c_str());
  }

  printer->setOutputFormat(QPrinter::NativeFormat);

  QPrintDialog *dialog = new QPrintDialog(printer, this);
  dialog->setWindowTitle(tr("Print Diagram"));

  if (dialog->exec() != QDialog::Accepted)
    return;

  if (!printer->outputFileName().isEmpty())
    priop.fname = printer->outputFileName().toStdString();
  else if (command.substr(0, 4) == "lpr ")
    priop.fname = miTime::nowTime().format("TS%d%H%M%S.ps");
  else
    priop.fname = fname;

  // fill printOption from qprinter-selections
  fillPrintOption(printer, priop);

  // set printername
  if (printer->outputFileName().isEmpty())
    priop.printer = printer->printerName().toStdString();

  // start the postscript production
  QApplication::setOverrideCursor(Qt::WaitCursor);
  work->Show()->hardcopy(priop);

  // if output to printer: call appropriate command
  if (printer->outputFileName().isEmpty()) {
    priop.numcopies = printer->numCopies();

    // expand command-variables
    pman.expandCommand(command, priop);

    cerr << "PRINT: " << command << endl;

    int sys = system(command.c_str());

  }
  QApplication::restoreOverrideCursor();

  // reset number of copies (saves a lot of paper)
  printer->setNumCopies(1);

}

void qtsMain::makeEPS(const miString& filename)
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  printOptions priop;
  priop.fname = filename;
  priop.colop = d_print::incolour;
  priop.orientation = d_print::ori_automatic;
  priop.pagesize = d_print::A4;
  priop.numcopies = 1;
  priop.usecustomsize = false;
  priop.fittopage = false;
  priop.drawbackground = true;
  priop.doEPS = true;

  work->Show()->hardcopy(priop);

  QApplication::restoreOverrideCursor();
}

void qtsMain::about()
{

  QMessageBox::about(
      this,
      tr("About T-series"),
      tr("T-series: Times series viewer\nVersion: %1\n\nmet.no 2002").arg(
          VERSION));
}

void qtsMain::writeLog()
{
  tsSetup setup;

  config.set("SIZEX", this->width());
  config.set("SIZEY", this->height());
  config.set("POSX", this->x());
  config.set("POSY", this->y());

  config.set("SHOWNORMAL", snormal);
  config.set("SHOWSELECT", sselect);
  config.set("SHOWICON", sicon);
  config.set("TIMEMARK", tmark);
  config.set("LATLONDEC", latlond);
  config.set("FONT", miString(qApp->font().toString().toStdString()));
  config.set("LOCKHOURSTOMODEL", lockHoursToModel);
  config.set("SHOWGRIDLINES", showGridLines);

  if (lang.exists())
    config.set("LANG", lang);

  work->collectLog();

  config.save(setup.files.configure);
}

void qtsMain::restoreLog()
{
  int sx, sy, px, py;
  miString f;

  if (config.get("SIZEY", sy) && config.get("SIZEX", sx))
    this->resize(sx, sy);

  if (config.get("POSX", px) && config.get("POSY", py))
    this->move(px, py);

  if (config.get("FONT", f)) {
    QFont font;
    if (font.fromString(f.c_str()))
      qApp->setFont(font);
  }

  work->restoreLog();
}

void qtsMain::readLog()
{
  tsSetup setup;
  config.read(setup.files.configure);
  restoreLog();
}

void qtsMain::setSaveOnQuit(bool)
{
  bool soq;
  config.get("SAVEONQUIT", soq);
  config.set("SAVEONQUIT", !soq);
}

void qtsMain::toggleNormalNames(bool isOn)
{
  snormal = isOn;
  sendNamePolicy();
}

void qtsMain::toggleSelectedNames(bool isOn)
{
  sselect = isOn;
  sendNamePolicy();
}

void qtsMain::toggleIcon(bool isOn)
{
  sicon = isOn;
  sendNamePolicy();
}

void qtsMain::togglePositions(bool isOn)
{

  sposition = isOn;
  if (!dianaconnected)
    return;

  if (sposition)
    refreshDianaStations();
  miMessage m;
  if (sposition)
    m.command = qmstrings::showpositions;
  else
    m.command = qmstrings::hidepositions;
  m.description = DATASET_TSERIES + work->lastList();
  pluginB->sendMessage(m);
}

void qtsMain::toggleTimemark(bool isOn)
{
  tmark = isOn;
  setTimemark(miTime::nowTime());
}

void qtsMain::toggleLatLon(bool isOn)
{
  latlond = isOn;
  if (work)
    work->latlonInDecimalToggled(latlond);
}

void qtsMain::toggleShowGridlines(bool isOn)
{
  showGridLines = isOn;
  if (work)
    work->setShowGridLines( showGridLines );
}


void qtsMain::toggleLockHoursToModel(bool isOn)
{
  lockHoursToModel = isOn;
  if (work)
    work->toggleLockHoursToModel(lockHoursToModel);
}

void qtsMain::setTimemark(miTime mark)
{
  if (work) {
    if (tmark)
      work->Show()->setTimemark(mark, thisTM);
    else
      work->Show()->clearTimemarks(thisTM);
  }
}

// DIANA

void qtsMain::setDianaTimemark(miTime mark)
{
  if (work) {
    if (dianaconnected)
      work->Show()->setTimemark(mark, dianaTM);
    else
      work->Show()->clearTimemarks(dianaTM);
  }
}

// send one image to diana (with name)
void qtsMain::sendImage(const miString name, const QImage& image)
{
  if (!dianaconnected)
    return;
  if (image.isNull())
    return;

  QByteArray *a = new QByteArray();
  QDataStream s(a, QIODevice::WriteOnly);
  s << image;

  miMessage m;
  m.command = qmstrings::addimage;
  m.description = "name:image";

  ostringstream ost;
  ost << name << ":";

  int n = a->count();
  for (int i = 0; i < n; i++) {
    ost << setw(7) << int((*a).data()[i]);
  }
  miString txt = ost.str();
  m.data.push_back(txt);

  pluginB->sendMessage(m);
}

void qtsMain::refreshDianaStations()
{
  if (!dianaconnected || !sposition)
    return;

  if (currentModel == work->lastList())
    return;

  miString prevModel = currentModel;
  currentModel = work->lastList();

  if (!sendModels.count(currentModel))
    sendNewPoslist();

  sendNamePolicy();
  enableCurrentPoslist();
  disablePoslist(prevModel);

}

void qtsMain::disablePoslist(miString prev)
{
  if (prev == NOMODEL_TSERIES)
    return;
  if (!dianaconnected)
    return;

  miMessage m;
  m.command = qmstrings::hidepositions;
  m.description = DATASET_TSERIES + prev;
  pluginB->sendMessage(m);
}

void qtsMain::enableCurrentPoslist()
{
  if (!dianaconnected)
    return;
  miMessage m;
  m.command = qmstrings::showpositions;
  m.description = DATASET_TSERIES + currentModel;
  pluginB->sendMessage(m);
}

void qtsMain::sendNewPoslist()
{
  cerr << "sendNewPosList" << endl;
  sendModels.insert(currentModel);
  if (!dianaconnected)
    return;
  miMessage m = work->getStationList();

  m.common += (snormal ? "true" : "false");
  m.common += (sselect ? ":true" : ":false");
  pluginB->sendMessage(m);
}

void qtsMain::sendTarget()
{
  if (!dianaconnected)
    return;

  miMessage m, m2;

  m = work->target();
  pluginB->sendMessage(m);

  m2.command = qmstrings::showpositions;
  m2.description = TARGETS_TSERIES;
  pluginB->sendMessage(m2);
}

void qtsMain::clearTarget()
{
  if (!dianaconnected)
    return;

  miMessage m;
  m.command = qmstrings::hidepositions;
  m.description = TARGETS_TSERIES;
  pluginB->sendMessage(m);
}

// called when client-list changes

void qtsMain::processConnect()
{
  tsSetup s;

  if (pluginB->clientTypeExist(s.server.client)) {
    dianaconnected = true;

    cout << ttc::color(ttc::Blue) << "< CONNECTING TO: " << s.server.client
        << " > " << ttc::reset << endl;

    QImage sImage(s.files.std_image.c_str());
    QImage fImage(s.files.fin_image.c_str());
    QImage iImage(s.files.icon_image.c_str());

    sendImage(IMG_STD_TSERIES, sImage);
    sendImage(IMG_FIN_TSERIES, fImage);
    sendImage(IMG_ICON_TSERIES, iImage);

    sendNamePolicy();
    selectionTypeChanged();
    refreshDianaStations();

  } else
    dianaconnected = false;

  setRemoteParameters();

}

void qtsMain::setRemoteParameters()
{
  targetB->setEnabled(dianaconnected);
  normalAct->setEnabled(dianaconnected);
  selectAct->setEnabled(dianaconnected);
  iconAct->setEnabled(dianaconnected);

  if (!dianaconnected) {
    currentModel = NOMODEL_TSERIES;
    sendModels.clear();
  }
}

void qtsMain::sendNamePolicy()
{
  if (!dianaconnected)
    return;

  miMessage m;
  m.command = qmstrings::showpositionname;
  m.description = "normal:selected:icon";

  m.data.push_back(snormal ? "true" : "false");
  m.data[0] += (sselect ? ":true" : ":false");
  m.data[0] += (sicon ? ":true" : ":false");
  pluginB->sendMessage(m);

}

// processes incoming miMessages

void qtsMain::processLetter(const miMessage& letter)
{
//#ifdef DEBUG
  cerr <<"Command: "<<letter.command<<"  ";
  cerr <<endl;
  cerr <<" Description: "<<letter.description<<"  ";
  cerr <<endl;
  cerr <<" commonDesc: "<<letter.commondesc<<"  ";
  cerr <<endl;
  cerr <<" Common: "<<letter.common<<"  ";
  cerr <<endl;
  for (int i=0;i<letter.data.size();i++)
  if (letter.data[i].length()<80)
  cerr <<" data["<<i<<"]:"<<letter.data[i]<<endl;;
  cerr <<" From: "<<letter.from<<endl;
  cerr <<"To: "<<letter.to<<endl;

//#endif
  if (letter.command == qmstrings::removeclient) {
    tsSetup s;
    if (letter.common.find(s.server.client.c_str()) != string::npos)
      cleanConnection();
  }

  if (letter.command == qmstrings::positions && letter.common == "diana")
    if (letter.data.size())
      work->changePositions(letter.data[0]);

  if (letter.command == qmstrings::selectposition) {
    if (letter.data.size())
      work->changeStation(letter.data[0]);
  } else if (letter.command == qmstrings::timechanged) {
    // Added to avoid invalid updates from diana when diana
    // is in automatic update mode
    miTime timeFromDiana = miTime(letter.common);
    if (timeFromDiana != currentTime) {
      currentTime = timeFromDiana;
      setDianaTimemark(currentTime);
    }
  }

}

void qtsMain::showHelp()
{
  helpdialog->showdoc(0, "");
}

void qtsMain::initHelp()
{
  tsSetup s;

  miString helpfile = s.path.doc + "/" + lang + "_" + s.doc.mainSource;
  miString newsfile = s.path.doc + "/" + lang + "_" + s.doc.newsSource;

  ifstream testhelpfile(helpfile.c_str());
  ifstream testnewsfile(newsfile.c_str());

  if (testhelpfile) {
    helpfile = lang + "_" + s.doc.mainSource;
    testhelpfile.close();
  } else
    helpfile = s.doc.mainSource;

  if (testnewsfile) {
    newsfile = lang + "_" + s.doc.newsSource;
    testnewsfile.close();
  } else
    newsfile = s.doc.newsSource;

  HelpDialog::Info info;
  HelpDialog::Info::Source helpsource;
  info.path = s.path.doc;
  helpsource.source = helpfile;
  helpsource.name = tr("Manual").toStdString();
  helpsource.defaultlink = s.doc.mainLink;
  info.src.push_back(helpsource);
  helpsource.source = newsfile;
  helpsource.name = tr("News").toStdString();
  helpsource.defaultlink = s.doc.newsLink;
  info.src.push_back(helpsource);

  helpdialog = new HelpDialog(this, info);
  helpdialog->hide();

}

void qtsMain::timerEvent(QTimerEvent* e)
{

  if (e->timerId() == updateTimer)
    if (work) {
      work->updateStreams();
      setTimemark(miTime::nowTime());
    }

}

void qtsMain::cleanConnection()
{
  dianaconnected = false;
  cout << ttc::color(ttc::Red) << "< DISCONNECTING >" << ttc::reset << endl;

  setRemoteParameters();
  setDianaTimemark(miTime::nowTime());
}




void qtsMain::changeObservationStart()
{
  miTime start = work->getObservationStartTime();
  int year = start.year();
  int month=start.month();
  int day = start.day();
  QDate qstart(year,month,day);

  PopupCalendar * calendar = new PopupCalendar(this,qstart);

  if (calendar->exec()) {
     qstart=calendar->result();
     qstart.getDate(&year,&month,&day);
     start.setTime(year,month,day,0,0,0);
     work->setObservationStartTime(start);
   }

}


void qtsMain::manageParameterFilter()
{
  set<string>    blacklist= work->getKlimaBlacklist();
  vector<string> allklimaParameters = work->getAllKlimaParameters();

  ParameterFilterDialog * filterdialog = new ParameterFilterDialog(blacklist,allklimaParameters, this);

  if (filterdialog->exec()) {
    blacklist=filterdialog->result();
    work->setKlimaBlackList(blacklist);
  }
}

void qtsMain::manageFilter()
{

  set<miString> p = work->fullPosList();
  set<miString> f = work->Filter();
  set<miString> o = work->createFilter(true);

  qtsFilterManager * fm = new qtsFilterManager(p, f, o, this);

  if (fm->exec()) {
    // clean out the old filters from diana
    if (dianaconnected) {
      set<miString>::iterator itr = sendModels.begin();
      for (; itr != sendModels.end(); itr++)
        if (itr->contains(TS_MINE)) {
          sendModels.erase(*itr);
          itr--;
        }
      if (currentModel.contains(TS_MINE))
        currentModel = NOMODEL_TSERIES;
    }
    // new filter
    work->newFilter(fm->result());

  }

}

void qtsMain::chooseFont()
{
  bool ok;
  QFont font = QFontDialog::getFont(&ok, qApp->font(), this);
  if (ok) {
    // font is set to the font the user selected
    qApp->setFont(font);
  } else {
    // the user cancelled the dialog; font is set to the initial
    // value: do nothing
  }
}

void qtsMain::findLanguages()
{
  QDir d;

  tsSetup setup;
  miString dlang = (setup.path.lang.empty() ? "./" : setup.path.lang[0]);

  d.setPath(dlang.c_str());
  QStringList f = d.entryList(QStringList("tseries_??.qm"));

  languageGroup = new QActionGroup(this);
  connect(languageGroup, SIGNAL( triggered(QAction*) ), this,
      SLOT( toggleLang(QAction*) ));

  for (QStringList::Iterator it = f.begin(); it != f.end(); ++it) {
    QString s = *it;
    s.replace("tseries_", "");
    s.replace(".qm", "");

    QAction * action = new QAction(s, languageGroup);
    action->setCheckable(true);
    action->setChecked(s.toStdString() == lang);
    languageGroup->addAction(action);
    menu_lang->addAction(action);
  }

  QAction * action = new QAction("en", languageGroup);
  action->setCheckable(true);
  action->setChecked(!lang.exists() || lang == "en");
  languageGroup->addAction(action);
  menu_lang->addAction(action);
}

void qtsMain::toggleLang(QAction* action)
{
  lang = action->text().toStdString();

  QMessageBox::information(
      this,
      tr("Language Changed"),
      tr("tseries must be restarted to reset the language to: [%1] ").arg(
          lang.c_str()));

}

void qtsMain::selectionTypeChanged()
{
  if (work->getSelectionType() == qtsWork::SELECT_BY_COORDINATES) {
    togglePositions(false);
    sendTarget();
  } else {
    togglePositions(true);
    clearTarget();
  }
}

void qtsMain::coordinatesChanged()
{
  sendTarget();
}

