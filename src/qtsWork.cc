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

#include "qtsWork.h"
#include "tsConfigure.h"

#include <tsData/FimexStream.h>
#include <puTools/miStringFunctions.h>
#include <coserver/QLetterCommands.h>

#include <QApplication>
#include <QSplitter>

#include <fstream>
#include <set>
#include <string>

using namespace std;
using namespace miutil;

namespace {
bool qStr2miStr(const QString& i, std::string& o)
{
  if (i.isEmpty()) {
    o = "";
    return false;
  } else {
    o = i.toStdString();
    return true;
  }
}

//! insert first lat, then lon
QStringList& operator<<(QStringList& s, const miCoordinates& co)
{
  return s << QString::number(co.dLat()) << QString::number(co.dLon());
}
} // namespace

const QString C_DATASET = "dataset";

const QString DATASET_FIMEX=   "T-series Fimex";
const QString TARGETS_TSERIES= "TARGETS_TSERIES";
const QString IMG_STD_TSERIES= "IMG_STD_TSERIES";
const QString IMG_FIN_TSERIES= "IMG_FIN_TSERIES";
const QString IMG_NEW_TSERIES= "IMG_NEW_TSERIES";
const QString IMG_ACTIVE_TSERIES= "IMG_ACTIVE_TSERIES";
const QString IMG_ICON_TSERIES= "IMG_ICON_TSERIES";
const QString NOMODEL_TSERIES= "NONE";

qtsWork::qtsWork(QWidget* parent, QString language)
  : QWidget(parent) , activeRefresh(true)
{
  reading_data=false;

  QSplitter   * splitter = new QSplitter(this);
  QHBoxLayout * hlayout  = new QHBoxLayout(this);
  oldModel = NOMODEL_TSERIES;

  sidebar = new qtsSidebar(language);
  show    = new qtsShow(&request,&data,&session);

  connect (show,SIGNAL(dataread_started()),this,SLOT(dataread_started()));
  connect (show,SIGNAL(dataread_ended()),this,SLOT(dataread_ended()));

  connect( sidebar, SIGNAL( minmaxProg(int,int)),
      this,    SLOT(setProgintervall(int,int)));

  connect(show,SIGNAL(newTimeRange(int, int)), sidebar,SLOT(newTimeRange(int,int)));

  connect( sidebar, SIGNAL(  observationToggled(bool)), this, SLOT(observationToggled(bool)));
  connect( sidebar, SIGNAL(changeFimexPoslist()), this, SIGNAL(fimexPoslistChanged()));

  splitter->addWidget(show);
  splitter->addWidget(sidebar);
  splitter->setStretchFactor (0, 3);

  hlayout->addWidget(splitter);

  connect(sidebar,SIGNAL(changeFimexModel(const QString&)), this,SLOT(changeFimexModel(const QString& )));
  connect(sidebar,SIGNAL(changeFimexStyle(const QString&)), this,SLOT(changeFimexStyle(const QString& )));
  connect(sidebar,SIGNAL(changeFimexRun(const QString&)),   this,SLOT(changeFimexRun(  const QString& )));
  connect(sidebar,SIGNAL(changeFimexCoordinates(float, float,QString)),this,SLOT(changeFimexCoordinates(float,float,QString)));
  connect(sidebar,SIGNAL( newFimexPoslist() ),   this,SLOT( newFimexPoslist() ));

  Initialise(); // the none gui stuff...

  connect(sidebar,SIGNAL(changetype(const tsRequest::Streamtype)), this,SLOT(changeType(const tsRequest::Streamtype)));
}

void qtsWork::Initialise()
{
  session.readSessions(setup.files.defs,setup.path.styles);

  for (unsigned int i=0; i<setup.streams.size(); i++) {
    data.addDataset(setup.streams[i].collectionName);

    for (unsigned int j=0; j<setup.streams[i].data.size(); j++) {
      data.addStream(setup.streams[i].data[j].name,      // streamname
          setup.streams[i].data[j].descript,  // description
          setup.streams[i].data[j].type,      // streamtype
          i, j,                               // dataset and
          // number in dataset
          setup.streams[i].data[j].contents, // models/runs
          setup.streams[i].data[j].config);  // fimexconfigfile
    }
  }

  makeStyleList();

  int t,f;
  show->getTimeRange(t,f);
  sidebar->newTimeRange(t,f);
}

miQMessage qtsWork::getStationList()
{
  miQMessage m(qmstrings::positions);
  m.addCommon(C_DATASET, DATASET_FIMEX)
      .addCommon("image", IMG_STD_TSERIES)
      .addCommon("icon", IMG_ICON_TSERIES)
      .addCommon("annotation", DATASET_FIMEX + lastList());
  m.setData((QStringList() << "name" << "lat" << "lon"),
            sidebar->getFimexPositions());
  return m;
}

QString qtsWork::lastList() const
{
  return QString::fromStdString(request.getFimexModel());
}

bool qtsWork::makeStyleList()
{
  vector<std::string> fimexStyles;
  session.getStyleTypes(fimexStyles);

  const QString fstyle = sidebar->fillList(fimexStyles, CMFIMEXSTYLE);
  makeFimexModels(fstyle);
  request.setFimexStyle(fstyle.toStdString());
  return true;
}

///////////////////////// SLOTS

void qtsWork::changePositions(float lon, float lat)
{
  sidebar->setCoordinates(lon,lat);
}

void qtsWork::changeStation(const QString& qstr)
{
  sidebar->changeFimexPosition(qstr);
}

void  qtsWork::setKlimaBlackList(std::set<std::string>& bl)
{
  data.setKlimaBlacklist(bl);
  refresh(true);
}

void qtsWork::checkObsPosition(miCoordinates cor)
{
  pets::KlimaStation s = data.getNearestKlimaStation(cor);

  if(!s.stationid) {
    // try mora station
    pets::MoraStation s;
    s = data.getNearestMoraStation(cor);
    if(s.name.empty()) {
      sidebar->setObsInfo("");
      return;
    }
    sidebar->setObsInfo(s.description().c_str());
    return; 
  }
  sidebar->setObsInfo(s.description().c_str());
}

void qtsWork::refresh(bool readData)
{
  if (activeRefresh){
    show->refresh(readData);
  }
}

void qtsWork::restoreLog()
{
  tsConfigure c;
  std::string po;

  bool validR;
  if(!c.get("VALIDREQUEST",validR))
    return;
  if(!validR)
    return;

  c.get("REQUESTPOS",po);

  activeRefresh = false;

  changeStation(QString::fromStdString(po));

  activeRefresh = true;
  refresh(true);

  std::string observationfilter;
  c.get("OBSERVATIONFILTER",observationfilter);
  data.setObservationBlacklistFromString(observationfilter);

  std::string timecontrol;
  c.get("TIMECONTROL",timecontrol);
  sidebar->setTimeControlFromLog(timecontrol);

  bool showobs=false;
  c.get("SHOWOBSERVATIONS",showobs);
  sidebar->setObservationsEnabled(showobs);
  show->setShowObservations(showobs);


  std::string fimexmodel,fimexstyle,fimexexpand,fimexfilter;

  c.get("FIMEXMODEL",fimexmodel);
  c.get("FIMEXSTYLE",fimexstyle);
  c.get("FIMEXEXPANDEDDIRS",fimexexpand);
  c.get("FIMEXFILTER",fimexfilter);

  pets::FimexStream::setParameterFilterFromString(fimexfilter);

  if(!fimexmodel.empty())
     request.setFimexModel(fimexmodel);
  if(!fimexstyle.empty())
     request.setFimexStyle(fimexstyle);

  QString qstyle(fimexstyle.c_str());
  changeFimexStyle(qstyle);

  sidebar->restoreFimexFromLog(fimexmodel,fimexstyle,fimexexpand);
}

void qtsWork::collectLog()
{
  tsConfigure c;

  c.set("VALIDREQUEST",true);

  c.set("SHOWOBSERVATIONS",sidebar->getObservationsEnabled());
  c.set("TIMECONTROL",sidebar->getTimecontrolLog());
  c.set("OBSERVATIONFILTER",data.getObservationBlacklistAsString());


  c.set("FIMEXMODEL",request.getFimexModel());
  c.set("FIMEXSTYLE",request.getFimexStyle());
  c.set("FIMEXEXPANDEDDIRS",sidebar->getFimexExpanded());

  c.set("FIMEXFILTER",pets::FimexStream::getParameterFilterAsString());

  sidebar->writeBookmarks();
}

miQMessage qtsWork::target()
{
  miCoordinates coord;
  coord = sidebar->fimexCoordinates();

  miQMessage m(qmstrings::positions);
  m.addDataDesc(TARGETS_TSERIES+";name").addDataDesc("lat").addDataDesc("lon").addDataDesc("image");
  m.addDataValues(QStringList() << "." << coord << IMG_FIN_TSERIES);
  return m;
}

void qtsWork::updateStreams()
{
  vector<int> idx;

  if (data.check(idx)){

    int size, dset, nindset;

    std::string filename, descrip;

    for (unsigned int i=0; i<idx.size();i++){
      data.getStreamInfo(idx[i], filename, descrip, size, dset, nindset);

      cout << "Reopening stream: " << filename << " : " << descrip << endl;

      data.openStream(idx[i]);
    }

    if (idx.size())
      data.makeStationList();
  }

  if(data.updateFimexStreams(request.getFimexModel())) {
    vector<std::string> times = data.getFimexTimes(request.getFimexModel());
    sidebar->fillList(times, CMFIMEXRUN);
  }
}

////////////////////////////////////////////////////////////

/// FIMEX --------------------------------------------
//

void qtsWork::makeFimexModels(const QString& activeStyle)
{
  std::string st;
  qStr2miStr(activeStyle,st);

  vector<std::string>    modname;

  int choice =  session.getModels(st, fimexModelMap, modname);

  if (choice < 0) {
    if(modname.size() > 1)
      modname.erase(modname.begin()+1, modname.end());
  }

  sidebar->fillList(modname, CMFIMEXMODEL);
}

void qtsWork::changeFimexModel(const QString& newmodel)
{
  request.setFimexModel(newmodel.toStdString());
  vector<std::string> times = data.getFimexTimes(newmodel.toStdString());
  sidebar->fillList(times, CMFIMEXRUN);
}

void qtsWork::changeFimexRun(const QString& nrun)
{
  if(request.setFimexRun(nrun.toStdString())) {
    refresh(true);
  }
}

void qtsWork::changeFimexStyle(const QString& nsty)
{
  try {
    makeFimexModels(nsty);
  } catch (exception& e) {
    cerr << "Exception while trying to make new modellist" << e.what() << endl;
  }

  if(request.setFimexStyle(nsty.toStdString()))
    refresh(true);
}

void qtsWork::changeFimexCoordinates(float lon, float lat,QString name)
{
  if(request.setFimexLocation(lat,lon,name.toStdString())) {
    miCoordinates cor(lon,lat);
    checkObsPosition(cor);
    refresh(true);
  }
  emit fimexPositionChanged(name);
}

void qtsWork::newFimexPoslist()
{
  vector<string> newfimexposlist = sidebar->allFimexPositions();
  pets::FimexStream::setCommonPoslistFromStringlist(newfimexposlist);
}

void qtsWork::dataread_started()
{
  cerr << "DATAREAD STARTED" << endl;
  reading_data = true;
}

void qtsWork::dataread_ended()
{
  reading_data = false;
  sidebar->endProgress();
}

void qtsWork::updateProgress()
{
  if(reading_data) {
    int newprogress = pets::FimexStream::getProgress();
    if (newprogress > 0 && newprogress < 100) {
      string progressText= pets::FimexStream::getProgressMessage();

      sidebar->setProgress(newprogress,progressText);
    }
  }
}
