/*
  Tseries - A Free Meteorological Timeseries Viewer

  $Id$


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
#include "qtsSidebar.h"

#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "tsSetup.h"
#include "ts_find.xpm"
#include "ts_filter.xpm"
#include "view-refresh.xpm"
#include "list-add.xpm"
#include "synop.xpm"

#include <iostream>

using namespace miutil;
using namespace std;

qtsSidebar::qtsSidebar()
  : QWidget()
{

  tsSetup s;

  tabs       = new QTabWidget(this);

  // The Station Tabulator
  stationtab = new StationTab(this);

  connect(stationtab,SIGNAL(changestyle( const QString&)),  this, SIGNAL(changestyle(const QString& )));
  connect(stationtab,SIGNAL(changemodel( const QString&)),  this, SIGNAL(changemodel(const QString& )));
  connect(stationtab,SIGNAL(changerun(    const QString&)), this, SIGNAL(changerun(  const QString& )));
  connect(stationtab,SIGNAL(changestation(const QString&)), this, SIGNAL(changestation(  const QString& )));


  // The WDB Tabulator

  wdbtab  = new CoordinateTab(this);


  connect(wdbtab,SIGNAL(changestyle( const QString&)), this, SIGNAL(changeWdbStyle(const QString& )));
  connect(wdbtab,SIGNAL(changemodel( const QString&)), this, SIGNAL(changeWdbModel(const QString& )));
  connect(wdbtab,SIGNAL(changerun(   const QString&)), this, SIGNAL(changeWdbRun(  const QString& )));
  connect(wdbtab,SIGNAL(changelevel( const QString&)), this, SIGNAL(changeWdbLevel(const QString& )));
  connect(wdbtab,SIGNAL(changelevel( const QString&)), this, SIGNAL(changeWdbLevel(const QString& )));
  connect(wdbtab,SIGNAL(changeCoordinates(float, float,QString)), this,
      SIGNAL(changeCoordinates(float,float,QString)));



  QString dbname= s.wdb.host.cStr();
  dbname.truncate( dbname.indexOf(".") );


  stationIdx = tabs->addTab(stationtab,tr("Stations"));
  wdbIdx     = tabs->addTab(wdbtab,dbname);

  connect(tabs,SIGNAL(currentChanged(int)), this,SLOT(tabChanged(int)));

  // Control the start and length

  timecontrol= new TimeControl(this);

  connect(timecontrol,SIGNAL(minmaxProg(int,int)),this, SIGNAL(minmaxProg(int,int)));



  obsInfo = new QLabel(this);
  obsInfo->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  obsInfo->hide();


  // connectbuttons are hosted here... but are used and
  // connected in qtsMain!

  QPixmap synop_pix(synop_xpm);
  QPixmap find_pix(ts_find_xpm);
  QPixmap filter_pix(ts_filter_xpm);
  QPixmap refresh_pix(view_refresh_xpm);
  QPixmap add_pix(list_add_xpm);

  pluginB = new ClientButton(s.server.name.cStr(),
			     s.server.command.cStr(),
			     this);
  pluginB->useLabel(true);



  observationB = new QPushButton(synop_pix,"",this);
  observationB->setMaximumWidth(synop_pix.width());
  observationB->setCheckable(true);
  observationB->setToolTip(  tr("enable/disable observations") );

  connect(observationB,SIGNAL(toggled(bool)), this, SIGNAL(observationToggled(bool)));

  targetB = new QPushButton(find_pix,"",this);
  targetB->setMaximumWidth(find_pix.width());
  targetB->setToolTip(tr("Show position (DIANA)") );


  filterB = new QPushButton(filter_pix,"",this);
  filterB->setMaximumWidth(filter_pix.width());
  filterB->setCheckable(true);
  filterB->setToolTip(  tr("Position filter") );

  connect(filterB,SIGNAL(toggled(bool)), this, SIGNAL(filterToggled(bool)));


  addBookmarkButton =  new QPushButton(add_pix, "",this);
  connect(addBookmarkButton,SIGNAL(clicked()),wdbtab, SLOT(addBookmarkFolder()));
  cacheQueryButton  =  new QPushButton(refresh_pix, "",this);
  connect(cacheQueryButton,SIGNAL(clicked()),    this, SLOT(chacheQueryActivated()));

  // LAYOUT ---------------------------

  QVBoxLayout * vlayout = new QVBoxLayout(this);


  vlayout->addWidget(tabs);

  vlayout->addWidget(timecontrol);

  vlayout->addWidget(obsInfo);

  // Buttons -------------------


  connectStatus = new QLabel(this);
  connectStatus->setMinimumSize(50,32);
  cacheQueryButton->setMinimumSize(32,32);
  busyLabel     = new QMovie(s.wdb.busyMovie.cStr());

  QHBoxLayout * blayout = new QHBoxLayout();
  blayout->addWidget(addBookmarkButton);
  blayout->addWidget(cacheQueryButton);
  blayout->addWidget(connectStatus);
  blayout->addStretch(2);
  blayout->addWidget(observationB);
  blayout->addWidget(filterB);
  blayout->addWidget(targetB);
  blayout->addWidget(pluginB);
  vlayout->addLayout(blayout);

  addBookmarkButton->hide();
  cacheQueryButton->hide();
}

void qtsSidebar::newTimeRange(int total,int fcast)
{
  timecontrol->setTimeRange(total,fcast);
}

void qtsSidebar::searchStation(const QString& s)
{
  stationtab->searchStation(s);
}

void qtsSidebar::currentStationChanged ( QListWidgetItem * current, QListWidgetItem * previous )
{
  stationtab->currentStationChanged(current,previous);
}


void qtsSidebar::setObsInfo(QString s)
{
    obsInfo->setText(s);
    if(s.isEmpty())
      obsInfo->hide();
    else
      obsInfo->show();
}


QString  qtsSidebar::fillList(const vector<miutil::miString>& v, const StationTab::lEntry l)
{
  QStringList qlist;
  for(unsigned int i=0;i<v.size();i++) {
     qlist << v[i].cStr();
  }



  if(l==StationTab::CMWDBSTYLE)
    return wdbtab->setStyles(qlist);

  return stationtab->fillList(qlist,l);

}

///  Wdb ---------------------------


void qtsSidebar::tabChanged(int idx)
{

  if(idx==wdbIdx){
    addBookmarkButton->show();
    cacheQueryButton->show();
    emit changetype(tsRequest::WDBSTREAM);
  } else if (idx==stationIdx) {
    addBookmarkButton->hide();
    cacheQueryButton->hide();
    emit changetype(tsRequest::HDFSTREAM);
  }
}



void qtsSidebar::enableWdb(bool has_wdb)
{
  wdbtab->setEnabled(has_wdb);
  int wdbtabindex =  tabs->indexOf(wdbtab);
  tabs->setTabEnabled(wdbtabindex,has_wdb);
}

void qtsSidebar::enableBusyLabel(bool enable)
{
  if(enable) {
    connectStatus->setMovie(busyLabel);
    busyLabel->start();
  } else {
    connectStatus->clear();
  }

}


bool qtsSidebar::restoreWdbFromLog(miutil::miString mod, miutil::miString sty, double lat, double lon, miutil::miString run,miutil::miString posname)
{
  if(!wdbtab->isEnabled()) {
    cout << "WDB-tab disabled log information discarded" << endl;
    return false;
  }

  wdbtab->setStyle( sty.cStr() );
  wdbtab->setModel( mod.cStr() );
  wdbtab->setRun(   run.cStr() );
  wdbtab->setCoordinates(lon,lat,posname.cStr());

}




void qtsSidebar::enableCacheButton(bool enable, bool force, unsigned long querytime)
{

  if(wdbtab->getActiveCacheRequest() && !force) return;
  bool cacheenabled = cacheQueryButton->isEnabled();

  if  (!enable) {
    if (!cacheenabled ) return;
    cacheQueryButton->setToolTip( QString("WDB Query is fast enough ( %1 ms ) query caching disabled ").arg(querytime) );
  } else {
    cacheQueryButton->setToolTip( QString("WDB Query is slow ( %1 ms )- query caching enabled").arg(querytime) );
  }

  cacheQueryButton->setEnabled(enable);
}


void qtsSidebar::chacheQueryActivated()
{
  wdbtab->setActiveCacheRequest(true);

  cacheQueryButton->setToolTip( QString("WDB caching already done for this model"));
  cacheQueryButton->setEnabled(false);

  enableBusyLabel(true);
  emit requestWdbCacheQuery();

}








