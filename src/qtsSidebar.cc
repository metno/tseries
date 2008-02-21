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
#include <qtsSidebar.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <iostream>
#include <qtooltip.h>
#include <tsSetup.h>
#include "ts_find.xpm"
#include "ts_filter.xpm"

qtsSidebar::qtsSidebar(QWidget* parent) 
  : QWidget(parent)
{ 


  vlayout = new Q3VBoxLayout(this, 10, 10, "listLayout");
  Q3HBoxLayout * hlayout =  new Q3HBoxLayout(0, "hLayout");;
  Q3HBoxLayout * blayout =  new Q3HBoxLayout(0, "bLayout");;

  modell  = new QComboBox(this,"modellist");
  runl    = new QComboBox(this,"runlist");
  stylel  = new QComboBox(this,"stylelist");
  statl   = new Q3ListBox(this,"stationList");
  searchw = new QLineEdit(this,"searchEdit");
  timecontrol= new TimeControl(this);

  connect(timecontrol,SIGNAL(minmaxProg(int,int)),
	  this,       SIGNAL(minmaxProg(int,int)));
	  

  runl->setMaximumWidth(50); 

  hlayout->addWidget(modell,3);
  hlayout->addWidget(runl);


  vlayout->addWidget(stylel);
  vlayout->addLayout(hlayout);
  vlayout->addWidget(timecontrol);

  vlayout->addWidget(statl);
  vlayout->addWidget(searchw);

  // connectbuttons are hosted here... but are used and
  // connected in qtsMain!

  tsSetup s;
  QPixmap find_pix(ts_find_xpm);
  QPixmap filter_pix(ts_filter_xpm);


  pluginB = new ClientButton(s.server.name.cStr(),
			     s.server.command.cStr(),
			     this);
  pluginB->useLabel(true);
  
  targetB = new QPushButton(find_pix,"",this);
  targetB->setMaximumWidth(find_pix.width());
  QToolTip::add( targetB, tr("Show position (DIANA)") );


  filterB = new QPushButton(filter_pix,"",this);
  filterB->setMaximumWidth(filter_pix.width());
  filterB->setToggleButton(true);

  QToolTip::add( filterB, tr("Position filter") );


  blayout->addStretch(2);
  blayout->addWidget(filterB);
  blayout->addWidget(targetB);
  blayout->addWidget(pluginB);


  vlayout->addLayout(blayout);

  connect(filterB,SIGNAL(toggled(bool)),this,
	  SIGNAL(filterToggled(bool)));

  connect(searchw,SIGNAL(textChanged(const QString&)),
	  this, SLOT(searchStation(const QString&)));

  connect(stylel,SIGNAL(activated(const QString&)),
	  this,SIGNAL(changestyle(const QString&)));
  connect(modell,SIGNAL(activated(const QString&)),
	  this,SIGNAL(changemodel(const QString&)));

  connect(runl,SIGNAL(activated(const QString&)),
	  this,SIGNAL(changerun(const QString&)));
  connect(statl,SIGNAL(highlighted(const QString&)),
	  this,SIGNAL(changestation(const QString&)));
	 
 
}

void qtsSidebar::searchStation(const QString& str )
{
  Q3ListBoxItem * item = statl->findItem(str);
  
  if(item)
    statl->setCurrentItem(item);  
}

QString qtsSidebar::fillStations(const QStringList& qlist )
{
  QString tmp = statl->currentText();

  statl->clear();
  statl->insertStringList(qlist);
  
  if(!tmp.isEmpty())
    searchStation(tmp);

  return statl->currentText();
}
QString qtsSidebar::station()
{
  return statl->currentText();
} 


QString qtsSidebar::fillList(const vector<miString>& slist, const lEntry c)
{
  QStringList qlist;
  QComboBox * co =  modell;
  
  if ( c == CMRUN   ) co = runl;
  if ( c == CMSTYLE ) co = stylel;
  
  QString cur = co->currentText();
  miString tmp = (!cur.isEmpty() ? cur.latin1() : "");
  bool entryFound = false;

  for(int i=0;i<slist.size();i++) { 
    qlist << slist[i].cStr();
    if(slist[i] == tmp )
      entryFound = true;
  }
  
  co->clear();
  co->insertStringList(qlist);
  
  if(!cur.isEmpty())
    if(entryFound)
      co->setCurrentText(cur);

  return co->currentText();
}

QString qtsSidebar::current(const lEntry c)
{
 
  if(c == CMSTYLE)
    return stylel->currentText();
  if(c == CMRUN)
    return runl->currentText();
  return modell->currentText();
}


void qtsSidebar::set(const miString& s,const lEntry c)
{
  if(c == CMSTYLE)
    stylel->setCurrentText(s.cStr());
  else if(c == CMRUN)
    runl->setCurrentText(s.cStr());
  else if(c == CMMODEL)
    modell->setCurrentText(s.cStr());
}


void qtsSidebar::iterateModel(int add)
{
  int i   = modell->currentItem() + add;
  int max = modell->count() - 1 ;

  if(i>max)
    i = 0;
  else if(i<0)
    i=max;
  
  modell->setCurrentItem(i);
  emit(changemodel(modell->currentText()));
}

