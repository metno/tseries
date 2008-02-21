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
#include <qtsWork.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <tsConfigure.h>
#include <qUtilities/QLetterCommands.h>
#include <qapplication.h>

bool qStr2miStr(const QString& i, miString& o)
{
  o="";
  
  if(i.isEmpty())
    return false;
  o=i.latin1();
  return true;
}

qtsWork::qtsWork(QWidget* parent)
  : QWidget(parent) , activeRefresh(true)
{
  filterOn = false;
  
  QGLFormat fmt;
  fmt.setOverlay(false);
  fmt.setDoubleBuffer(true);
  fmt.setDirectRendering(false);

  hlayout  = new Q3HBoxLayout(this, 1, 1, "worklayout");
  oldModel = NOMODEL_TSERIES;

  sidebar = new qtsSidebar(this);
  show    = new qtsShow(this,fmt,&request,&data,&session);

  sidebar->setMinimumWidth(170);
  sidebar->setMaximumWidth(255);
 
  
  connect( sidebar, SIGNAL( minmaxProg(int,int)),
	   this,    SLOT(setProgintervall(int,int)));


  hlayout->addWidget(show,3);
  hlayout->addWidget(sidebar,1);
 
  hlayout->activate();

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
}


void qtsWork::Initialise()
{
  session.readSessions(setup.files.defs);
  
  data.setVerbose(false);

  for (int i=0; i<setup.streams.size(); i++) {
    data.addDataset(setup.streams[i].collectionName);

    for (int j=0; j<setup.streams[i].data.size(); j++) {
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
  //cerr << "qtsWork::makeStationList" << endl;

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

//   cerr << "model:" << request.model() << " myList.size:" << myList.size() << endl;

  for (;itr!=myList.end();itr++) {
    pos = itr->first;
    
    if(filterOn)
      if(!filter.empty())
	if(!filter.count(pos))
	  continue;
    
    slist << pos.cStr();
    myStations.push_back( pos  + ":" + itr->second );  
  }
  
  sidebar->fillStations(slist);
  
  emit(refreshStations()); 
}


bool qtsWork::makeStyleList()
{
  vector<miString> tmp;

  session.getStyleTypes(tmp);

  QString cstyle = sidebar->fillList(tmp,qtsSidebar::CMSTYLE);
  
  miString st;
  qStr2miStr(cstyle,st);
  bool changed = request.setStyle(st);
  
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
  
  QString qtmp = sidebar->fillList(modname,qtsSidebar::CMMODEL);
  
  miString tmp;
  qStr2miStr(qtmp,tmp);
  
  tmp = modelMap[tmp];
  
  QApplication::setOverrideCursor( Qt::waitCursor );
  data.openStreams(tmp);
  QApplication::restoreOverrideCursor();

  changed = request.setModel(tmp);
  return (makeRunList(tmp) || changed);
}

bool qtsWork::makeRunList(const miString& st)
{
  vector <miString> runList;
  runList = data.findRuns(st);

  QString qtmp = sidebar->fillList(runList,qtsSidebar::CMRUN);
  miString tmp;
  qStr2miStr(qtmp,tmp);
  return request.setRun(atoi(tmp.cStr()));
}

bool qtsWork::makeRunList(const miString& st,const miString& ru)
{
  vector <miString> runList;
  runList = data.findRuns(st);

  sidebar->fillList(runList,qtsSidebar::CMRUN);
  if(runList.size()) {
    for(int i=0;i<runList.size();i++)
      if(ru == runList[i] ) {
	sidebar->set(ru,qtsSidebar::CMRUN);
	return request.setRun(atoi(ru.cStr()));
      }

    
    sidebar->set(runList[0],qtsSidebar::CMRUN);
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

void qtsWork::changeStation(const QString& qstr)
{
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
    refresh();
}

void qtsWork::changeModel(const miString& st)
{
  miString tmp = modelMap[st];
  QApplication::setOverrideCursor( Qt::waitCursor );
  data.openStreams(tmp);
  QApplication::restoreOverrideCursor();
  bool changed = request.setModel(tmp);
  if(makeRunList(tmp) || changed)
    refresh();
}

void qtsWork::changeStation(const miString& st)
{
  if(request.setPos(st))
    refresh();
  else {
    miString ST = st.upcase();
    if(request.setPos(ST))
      refresh();
  }
}

void qtsWork::changeRun(const miString& st)
{
  if(request.setRun(atoi(st.cStr())))
    refresh();
}

void qtsWork::refresh()
{
  //cerr << "qtsWork::refresh, request=" << request << endl;
  QApplication::setOverrideCursor( Qt::waitCursor );
  if (activeRefresh){
    show->refresh();
  }
  // check if any streams recently opened
  if ( data.has_opened_streams() ){
    //cerr << "qtsWork::refresh - remaking station list" << endl;
    data.makeStationList();
    makeStationList();
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

 
  sidebar->set(st,qtsSidebar::CMSTYLE);
  changeStyle(st);
  
  map<miString,miString>::iterator itr = modelMap.begin();
  for(;itr!=modelMap.end();itr++)
    if(itr->second == mo) {
      sidebar->set(itr->first,qtsSidebar::CMMODEL);
      break;
    }
  
  request.setModel(mo);
  QApplication::setOverrideCursor( Qt::waitCursor );
  data.openStreams(mo);
  QApplication::restoreOverrideCursor();

  makeRunList(mo,miString(ru));

  sidebar->searchStation(po.cStr());
  changeStation(po);

  activeRefresh = true;
  refresh();
}


void qtsWork::collectLog()
{
  tsConfigure c;
  
  c.set("REQUESTMODEL",request.model());
  c.set("REQUESTPOS",request.pos());
  c.set("REQUESTRUN",request.run());
  c.set("REQUESTSTYLE",request.style());
  c.set("VALIDREQUEST",true);
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
  miString t,po;

  if(qStr2miStr(sidebar->station(),po) ) {
    changeStation(po);
  }
   
  if(myTarget.data.empty())
    myTarget.data.push_back(miString());
 
  t =  ".:" + myList[po] + ":" + IMG_FIN_TSERIES; 
  
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
  
    for (int i=0; i<idx.size();i++){
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



