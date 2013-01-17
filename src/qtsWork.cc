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
#include "qtsWork.h"
#include "tsConfigure.h"

#include <qUtilities/QLetterCommands.h>
#include <QApplication>

#include <fstream>
#include <set>
#include <string>
#include "WdbCacheThread.h"
#include <QSplitter>

using namespace std;
using namespace miutil;

bool qStr2miStr(const QString& i, miString& o)
{
  o="";

  if(i.isEmpty())
    return false;
  o=i.toStdString();
  return true;
}

qtsWork::qtsWork(QWidget* parent)
: QWidget(parent) , activeRefresh(true)
{
  selectionType   = SELECT_BY_STATION;
  filterOn        = false;
  latlonInDecimal = false;
  QGLFormat fmt;
  fmt.setOverlay(false);
  fmt.setDoubleBuffer(true);
  fmt.setDirectRendering(false);



  QSplitter   * splitter = new QSplitter(this);
  QHBoxLayout * hlayout  = new QHBoxLayout(this);
  oldModel = NOMODEL_TSERIES;

  sidebar = new qtsSidebar();
  show    = new qtsShow(fmt,&request,&data,&session);


  connect (show,SIGNAL(refreshFinished()),this,SLOT(refreshFinished()));


  //sidebar->setMinimumWidth(170);
  //  sidebar->setMaximumWidth(255);


  connect( sidebar, SIGNAL( minmaxProg(int,int)),
      this,    SLOT(setProgintervall(int,int)));

  connect(show,SIGNAL(newTimeRange(int, int)), sidebar,SLOT(newTimeRange(int,int)));

  connect( sidebar, SIGNAL(  observationToggled(bool)), this, SLOT(observationToggled(bool)));

  splitter->addWidget(show);

  splitter->addWidget(sidebar);

  splitter->setStretchFactor (0,3);
  splitter->setStretchFactor (1,1);


  hlayout->addWidget(splitter);



  connect(sidebar,SIGNAL(changestyle(const QString&)),
      this,SLOT(changeStyle(const QString&)));
  connect(sidebar,SIGNAL(changemodel(const QString&)),
      this,SLOT(changeModel(const QString&)));
  connect(sidebar,SIGNAL(changerun(const QString&)),
      this,SLOT(changeRun(const QString&)));
  connect(sidebar,SIGNAL(changestation(const QString&)),
      this,SLOT(changeStation(const QString&)));
  connect(sidebar,SIGNAL(filterToggled(bool)),
      this,SLOT(filterToggled(bool)));



  Initialise(); // the none gui stuff...



  // WDB signals

  connect(sidebar,SIGNAL(changeWdbModel(const QString&)),  this,SLOT(changeWdbModel(const QString&)));


  connect(sidebar,SIGNAL(changetype(const tsRequest::Streamtype)), this,SLOT(changeType(const tsRequest::Streamtype)));
  connect(sidebar,SIGNAL(changeWdbStyle(const QString&)), this,SLOT(changeWdbStyle(const QString&)));
  connect(sidebar,SIGNAL(changeWdbRun(const QString&)),   this,SLOT(changeWdbRun(const QString&)));
  connect(sidebar,SIGNAL(changeCoordinates(float, float,QString)),this,SLOT(changeCoordinates(float,float,QString)));
  connect(sidebar,SIGNAL(requestWdbCacheQuery()),this,SLOT(requestWdbCacheQuery()));


  has_wdb_stream = data.has_wdb_stream();
  sidebar->enableWdb(has_wdb_stream);
  makeWdbModels();

}


void qtsWork::Initialise()
{
  session.readSessions(setup.files.defs);

  maxWDBreadTime = setup.wdb.readtime;

  data.setVerbose(false);

  for (unsigned int i=0; i<setup.streams.size(); i++) {
    data.addDataset(setup.streams[i].collectionName);

    for (unsigned int j=0; j<setup.streams[i].data.size(); j++) {
      data.addStream(setup.streams[i].data[j].name,      // streamname
          setup.streams[i].data[j].descript,  // description
          setup.streams[i].data[j].type,      // streamtype
          i, j,                               // dataset and
          // number in dataset
          setup.streams[i].data[j].contents); // models/runs
    }
  }

  //data.openStreams();
  //getStationList();

  myTarget.command       = qmstrings::positions;
  myTarget.description   = TARGETS_TSERIES+";name:lat:lon:image";

  makeStyleList();

  int t,f;
  show->getTimeRange(t,f);
  sidebar->newTimeRange(t,f);

  filter = createFilter();
}

miMessage qtsWork::getStationList()
{
  miString f = ( filterOn ? TS_MINE : "");
  miString iconname  = "tseries.png";
  miString annotation = DATASET_TSERIES +f+ request.model() ;
  miMessage m;
  m.command     = qmstrings::positions;
  m.commondesc  = "dataset:image:icon:annotation:normal:selected";
  m.common      =  DATASET_TSERIES +f+ request.model() + ":"
      + IMG_STD_TSERIES + ":" + IMG_ICON_TSERIES +":"+annotation + ":";
  m.description = "name:lat:lon";
  m.data = myStations;
  return m;
}

set<miString> qtsWork::fullPosList()
{
  set<miString> slist;
  ExtStation **es = new ExtStation*;

  int posc = 0;
  while (data.getPosition(-1,posc,es))
    slist.insert( (*es)->station.Name() );
  delete es;

  return slist;
}

void qtsWork::makeStationList(bool forced)
{
  if(!request.model().exists())
    restoreModelFromLog();


  if(!forced)
    if( oldModel == request.model())
      return;

  oldModel = request.model();

  QStringList slist;
  myStations.clear();

  myList = data.getPositions(request.model());
  map<miString,miString>::iterator itr = myList.begin();
  miString pos;
  for (;itr!=myList.end();itr++) {
    pos = itr->first;

    if(filterOn)
      if(!filter.empty())
        if(!filter.count(pos))
          continue;

    slist << pos.cStr();
    myStations.push_back(string(pos  + ":" + itr->second));
  }

  sidebar->fillStations(slist);

  emit(refreshStations());
}


bool qtsWork::makeStyleList()
{
  vector<miString> tmp;
  vector<miString> wdbtmp;

  session.getStyleTypes(tmp);
  session.getWdbStyles(wdbtmp);

  QString cstyle = sidebar->fillList(tmp,StationTab::CMSTYLE);
  QString wstyle = sidebar->fillList(wdbtmp,StationTab::CMWDBSTYLE);

  miString st;
  qStr2miStr(cstyle,st);
  bool changed = request.setStyle(st);

  qStr2miStr(wstyle,st);
  request.setWdbStyle(st);

  return (  makeModelList(st) || changed );

}

bool qtsWork::makeModelList(const miString& st)
{
  vector<miString>    modname;
  bool changed = false;

  int choice =  session.getModels(st, modelMap, modname);

  if(choice < 0 )
    if(modname.size() > 1 )
      modname.erase(modname.begin()+1,modname.end());

  QString qtmp = sidebar->fillList(modname,StationTab::CMMODEL);

  miString tmp;
  qStr2miStr(qtmp,tmp);

  tmp = modelMap[tmp];

  QApplication::setOverrideCursor( Qt::WaitCursor );
  data.openStreams(tmp);
  QApplication::restoreOverrideCursor();

  changed = request.setModel(tmp);
  return (makeRunList(tmp) || changed);
}

bool qtsWork::makeRunList(const miString& st)
{
  vector <miString> runList;
  runList = data.findRuns(st);

  QString qtmp = sidebar->fillList(runList,StationTab::CMRUN);
  miString tmp;
  qStr2miStr(qtmp,tmp);
  return request.setRun(atoi(tmp.cStr()));
}

bool qtsWork::makeRunList(const miString& st,const miString& ru)
{
  vector <miString> runList;
  runList = data.findRuns(st);

  sidebar->fillList(runList,StationTab::CMRUN);
  if(runList.size()) {
    for(unsigned int i=0;i<runList.size();i++)
      if(ru == runList[i] ) {
        sidebar->set(ru,StationTab::CMRUN);
        return request.setRun(atoi(ru.cStr()));
      }


    sidebar->set(runList[0],StationTab::CMRUN);
    return request.setRun(atoi(runList[0].cStr()));
  }

  return false;
}


///////////////////////// SLOTS

void qtsWork::changeStyle(const QString& qstr)
{
  miString st;
  if(qStr2miStr(qstr,st))
    changeStyle(st);

  makeStationList();
}

void qtsWork::changeModel(const QString& qstr)
{
  miString st;
  if(qStr2miStr(qstr,st))
    changeModel(st);
  makeStationList();
}

void qtsWork::changePositions(const miString& pos)
{
  if(selectionType != SELECT_BY_COORDINATES) return;

  vector<miString> vcoor = pos.split(":");
  if(vcoor.size() < 2) return;
  float lat = atof(vcoor[0].cStr());
  float lon = atof(vcoor[1].cStr());

  sidebar->setCoordinates(lon,lat);
}

void qtsWork::changeStation(const QString& qstr)
{
  if(selectionType!=SELECT_BY_STATION) return;
  miString st;
  if(qStr2miStr(qstr,st))
    changeStation(st);
}

void qtsWork::changeRun(const QString& qstr)
{
  miString st;
  if(qStr2miStr(qstr,st))
    changeRun(st);
}


////// miString

void qtsWork::changeStyle(const miString& st)
{
  bool changed = request.setStyle(st);

  if(makeModelList(st) || changed)
    refresh(true);
}

void qtsWork::changeModel(const miString& st)
{
  miString tmp = modelMap[st];
  QApplication::setOverrideCursor( Qt::WaitCursor );
  data.openStreams(tmp);
  QApplication::restoreOverrideCursor();
  bool changed = request.setModel(tmp);

  //if( makeRunList(tmp) || changed) {
  makeRunList(tmp);
  refresh(true);
  //}
}

void qtsWork::changeStation(const miString& st)
{
  if(selectionType!=SELECT_BY_STATION) return;

  miPosition p=data.getPositionInfo(st);
  miCoordinates cor=p.Coordinates();
  checkObsPosition(cor);

  if(!request.setPos(st)) {
    miString ST = st.upcase();

    if(!request.setPos(ST))
      return;
  }
  refresh(true);
}

void qtsWork::checkPosition(miString name)
{
  if(name.empty()) return;
  miPosition p=data.getPositionInfo(name);
  if(p.Name()!=name ) return;

  miCoordinates cor=p.Coordinates();
  checkObsPosition(cor);
  ostringstream ost;

  if(latlonInDecimal) {
    ost <<  "<b>Lat:</b> " << cor.dLat()<< " <b>Lon:</b> " << cor.dLon();
  } else {
    ost <<  "<b>Lat:</b> " << cor.sLat()<< " <b>Lon:</b> " << cor.sLon();
  }

//  ost << " <b>Hoh:</b> " << p.height();


  sidebar->setStationInfo(ost.str().c_str());



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
    sidebar->setObsInfo("");
    return;
  }
  sidebar->setObsInfo(s.description().c_str());
}




void qtsWork::changeRun(const miString& st)
{
  if(request.setRun(atoi(st.cStr())))
    refresh(true);
}

void qtsWork::refresh(bool readData)
{
  QApplication::setOverrideCursor( Qt::WaitCursor );
  if (activeRefresh){
    show->refresh(readData);
  }


  if(request.type() == tsRequest::HDFSTREAM) {

    // check if any streams recently opened

    if (data.has_opened_streams() && readData){
      data.makeStationList();
      makeStationList();
    }

    checkPosition(request.posname());
  }

  QApplication::restoreOverrideCursor();
}


void qtsWork::restoreLog()
{
  tsConfigure c;
  miString mo,po,st;
  int ru;

  bool validR;
  if(!c.get("VALIDREQUEST",validR))
    return;
  if(!validR)
    return;



  c.get("REQUESTMODEL",mo);
  c.get("REQUESTPOS",po);
  c.get("REQUESTRUN",ru);
  c.get("REQUESTSTYLE",st);


  activeRefresh = false;


  sidebar->set(st,StationTab::CMSTYLE);
  changeStyle(st);

  map<miString,miString>::iterator itr = modelMap.begin();
  for(;itr!=modelMap.end();itr++)
    if(itr->second == mo) {
      sidebar->set(itr->first,StationTab::CMMODEL);
      break;
    }

  request.setModel(mo);
  QApplication::setOverrideCursor( Qt::WaitCursor );
  data.openStreams(mo);
  QApplication::restoreOverrideCursor();

  makeRunList(mo,miString(ru));

  changeStation(po);

  activeRefresh = true;
  refresh(true);



  float lat,lon;
  miString run;
  miString posname;
  miString observationfilter;

  c.get("OBSERVATIONFILTER",observationfilter);
  data.setObservationBlacklistFromString(observationfilter);

  c.get("WDBMODEL",mo);
  c.get("WDBSTYLE",st);
  c.get("WDBLAT",lat);
  c.get("WDBLON",lon);
  c.get("WDBRUN",run);
  c.get("WDBPOSNAME",posname);

  miString timecontrol;
  c.get("TIMECONTROL",timecontrol);

  sidebar->setTimeControlFromLog(timecontrol);

  bool showobs=false;
  c.get("SHOWOBSERVATIONS",showobs);
  sidebar->setObservationsEnabled(showobs);
  show->setShowObservations(showobs);


  request.restoreWdbFromLog(mo,st,lat,lon,miTime(run),posname);
  sidebar->restoreWdbFromLog(mo,st,lat,lon,run,posname);

}


void qtsWork::collectLog()
{
  tsConfigure c;



  c.set("REQUESTMODEL",request.model());
  c.set("REQUESTPOS",request.pos());
  c.set("REQUESTRUN",request.run());
  c.set("REQUESTSTYLE",request.style());
  c.set("VALIDREQUEST",true);

  request.setType(tsRequest::WDBSTREAM);
  c.set("WDBMODEL",request.getWdbModel());
  c.set("WDBSTYLE",request.getWdbStyle());
  c.set("WDBLAT",float(request.getWdbLat()) );
  c.set("WDBLON",float(request.getWdbLon()) );
  c.set("WDBRUN",request.getWdbRun().isoTime());
  c.set("WDBPOSNAME",request.getWdbStationName());

  c.set("SHOWOBSERVATIONS",sidebar->getObservationsEnabled());
  c.set("TIMECONTROL",sidebar->getTimecontrolLog());
  c.set("OBSERVATIONFILTER",data.getObservationBlacklistAsString());


  sidebar->writeBookmarks();




}


void qtsWork::restoreModelFromLog()
{
  tsConfigure c;
  miString mo;
  c.get("REQUESTMODEL",mo);
  request.setModel(mo);
}



miMessage qtsWork::target()
{
  miString t,po,co;

  if(myTarget.data.empty())
    myTarget.data.push_back(miString());

  if(selectionType==SELECT_BY_STATION) {
    po=request.posname();
    changeStation(po);
    co=myList[po];
  } else
    co = sidebar->coordinateString();

  t =  ".:" + co + ":" + IMG_FIN_TSERIES;

  if(t != myTarget.data[0] )
    myTarget.data[0] = t;

  return myTarget;

}

void qtsWork::updateStreams()
{
  vector<int> idx;

  if (data.check(idx)){

    int size, dset, nindset;

    miString filename, descrip;

    for (unsigned int i=0; i<idx.size();i++){
      data.getStreamInfo(idx[i], filename, descrip, size, dset, nindset);

      cout << "Reopening stream: " << filename << " : " << descrip << endl;

      data.openStream(idx[i]);
    }

    if (idx.size())
      data.makeStationList();

  }
}

void qtsWork::filterToggled(bool f)
{
  filterOn = f;
  makeStationList(true);
}

void qtsWork::latlonInDecimalToggled(bool f)
{
  latlonInDecimal = f;
  checkPosition(request.posname());
}



set<miString> qtsWork::createFilter(bool orig)
{
  tsSetup s;
  set<miString> fl;

  miString fname =  s.files.baseFilter;


  if(!orig) {
    fname = s.files.filter;
    ifstream tst(fname.cStr());

    if(!tst)
      fname = s.files.baseFilter;

    tst.close();
  }


  ifstream in(fname.cStr());

  if(!in) {
    cerr << "NO filter " << endl;
    return fl;
  }




  miString token;
  while(in) {
    getline(in,token);

    token.trim();
    if(token.substr(0,1) == "#")
      continue;
    if(token.exists())
      fl.insert(token);
  }
  in.close();
  return fl;
}


void qtsWork::newFilter(const set<miString>& f)
{
  filter = f;
  if(filterOn)
    makeStationList(true);

  tsSetup s;

  ofstream of( s.files.filter.cStr());

  if(!of) {
    cerr << "could not write filter to file " <<  s.files.filter << endl;
    return;
  }

  of << "# automatic generated file. Do not edit!" << endl;

  set<miString>::iterator itr=filter.begin();

  for(;itr!=filter.end();itr++)
    of << *itr << endl;

  of.close();
}


/// WDB --------------------------------------------
//
void qtsWork::makeWdbModels()
{
  if(!has_wdb_stream) return;

  QStringList newmodels;
  set<string> providers = data.getWdbProviders();
  set<string>::iterator itr = providers.begin();
  for(;itr!=providers.end();itr++)
    newmodels << itr->c_str();
  sidebar->setWdbModels(newmodels);
}



void qtsWork::changeWdbModel(const QString& newmodel)
{
  if(!has_wdb_stream) return;

  set<miTime> wdbtimes = data.getWdbReferenceTimes(newmodel.toStdString());
  QStringList newruns;

  set<miTime>::reverse_iterator itr = wdbtimes.rbegin();
  for(;itr!=wdbtimes.rend();itr++)
    newruns << itr->isoTime().cStr();
  sidebar->setWdbRuns(newruns);


  pets::WdbStream::BoundaryBox bb = data.getWdbGeometry();
  sidebar->setWdbGeometry(bb.minLon,bb.maxLon,bb.minLat,bb.maxLat);
  if(request.setWdbModel(newmodel.toStdString()))
    refresh(true);
}

void qtsWork::changeWdbRun(const QString& nrun)
{
  if(!has_wdb_stream) return;

  miTime newrun= miTime(nrun.toStdString());

  if(request.setWdbRun(newrun))
    refresh(true);
}


void qtsWork::changeWdbStyle(const QString& nsty)
{
  if(!has_wdb_stream) return;

  if(request.setWdbStyle(nsty.toStdString()))
    refresh(true);
}

void qtsWork::changeType(const tsRequest::Streamtype s)
{
  if(s==tsRequest::WDBSTREAM)
    selectionType=SELECT_BY_COORDINATES;
  else
    selectionType=SELECT_BY_STATION;

  request.setType(s);
  refresh(true);

  emit selectionTypeChanged();

}

void qtsWork::changeCoordinates(float lon, float lat,QString name)
{

  if(!has_wdb_stream) return;

  request.setWdbStationName(name.toStdString());
  if(request.setWdbPos(lon,lat)) {
    miCoordinates cor(lon,lat);
    checkObsPosition(cor);
    refresh(true);
  }
  emit coordinatesChanged();

}

void qtsWork::refreshFinished()
{
  if(selectionType==SELECT_BY_COORDINATES ) {
    bool enableCacheButton =  (request.getWdbReadTime() > maxWDBreadTime );
    sidebar->enableCacheButton(enableCacheButton,false,request.getWdbReadTime());
  }

}
void qtsWork::requestWdbCacheQuery()
{
  if(!has_wdb_stream) return;

  vector<string> parameters=data.getWdbParameterNames();
  string  model  = request.getWdbModel();
  string  run    = request.getWdbRun().isoTime();
  string  height = "NULL";
  tsSetup setup;
  string  host   = setup.wdb.host;
  string  usr    = setup.wdb.user;

  WdbCacheThread *cachethread = new WdbCacheThread(host,usr);
  cachethread->setParameters(model,run,height,parameters);
  connect(cachethread,SIGNAL(finished()),this,SLOT(cacheRequestDone()));
  cachethread->start();
}

void   qtsWork::cacheRequestDone()
{
  sidebar->enableBusyLabel(false);
}


