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
#include <qtsSidebar.h>

#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "tsSetup.h"
#include "ts_find.xpm"
#include "ts_filter.xpm"

#include <iostream>

using namespace miutil;

qtsSidebar::qtsSidebar(QWidget* parent)
  : QWidget(parent)
{


  vlayout = new QVBoxLayout(this);
  QHBoxLayout * hlayout =  new QHBoxLayout();
  QHBoxLayout * blayout =  new QHBoxLayout();

  modell  = new QComboBox(this);
  runl    = new QComboBox(this);
  stylel  = new QComboBox(this);
  statl   = new QListWidget(this);
  searchw = new QLineEdit(this);
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
  targetB->setToolTip(tr("Show position (DIANA)") );


  filterB = new QPushButton(filter_pix,"",this);
  filterB->setMaximumWidth(filter_pix.width());
  filterB->setCheckable(true);
  filterB->setToolTip(  tr("Position filter") );

  pos_label = new QLabel("<b>Position</b>");
  pos_label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  QFont thisfont=this->font();
  thisfont.setPointSize(8);
  pos_label->setFont(thisfont);


  vlayout->addWidget(pos_label);

  blayout->addStretch(2);
  blayout->addWidget(filterB);
  blayout->addWidget(targetB);
  blayout->addWidget(pluginB);


  vlayout->addLayout(blayout);

  connect(filterB,SIGNAL(toggled(bool)),              this, SIGNAL(filterToggled(bool)));
  connect(searchw,SIGNAL(textChanged(const QString&)),this, SLOT(searchStation(const QString&)));
  connect(stylel,SIGNAL(activated(const QString&)),   this, SIGNAL(changestyle(const QString&)));
  connect(modell,SIGNAL(activated(const QString&)),   this, SIGNAL(changemodel(const QString&)));
  connect(runl,SIGNAL(activated(const QString&)),     this, SIGNAL(changerun(const QString&)));


  connect(statl,SIGNAL(currentItemChanged (    QListWidgetItem *, QListWidgetItem *)),
	  this, SLOT  (currentStationChanged(  QListWidgetItem *, QListWidgetItem *)));


}




void qtsSidebar::currentStationChanged ( QListWidgetItem * current, QListWidgetItem * previous )
{
  if(current) {
    QString st = current->text();
    emit changestation(st);

  }

}




void qtsSidebar::searchStation(const QString& str )
{
  QList<QListWidgetItem *>  items = statl->findItems (str, Qt::MatchStartsWith );
  if(!items.empty())
    statl->setCurrentItem(items.first());
}

QString qtsSidebar::fillStations(const QStringList& qlist )
{
  QString tmp;
  if(statl->count()) {
    if(statl->currentItem())
      tmp = statl->currentItem()->text();
  }
  statl->clear();
  statl->addItems(qlist);
  if(!tmp.isEmpty())
    searchStation(tmp);
  QListWidgetItem* item=statl->currentItem();
  if(item)
    return item->text();
  return "";
}

QString qtsSidebar::station()
{
  return statl->currentItem()->text();
}


QString qtsSidebar::fillList(const vector<miString>& slist, const lEntry c)
{
  QStringList qlist;
  QComboBox * co =  modell;

  if ( c == CMRUN   ) co = runl;
  if ( c == CMSTYLE ) co = stylel;

  QString cur = co->currentText();
  miString tmp = (!cur.isEmpty() ? cur.toStdString() : "");

  for(unsigned int i=0;i<slist.size();i++)
    qlist << slist[i].cStr();

  co->clear();
  co->addItems(qlist);

  if(!cur.isEmpty()) {
    int idx = co->findText(cur);
    if( idx >= 0)
      co->setCurrentIndex(idx);
  }

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


void qtsSidebar::set(const miString& cur,const lEntry c)
{
  int idx;
  if(c == CMSTYLE) {
    idx = stylel->findText(cur.cStr());
    if( idx >= 0)
      stylel->setCurrentIndex(idx);
  } else if(c == CMRUN) {
    idx = runl->findText(cur.cStr());
    if( idx >= 0)
      runl->setCurrentIndex(idx);
  } else if(c == CMMODEL) {
    idx = modell->findText(cur.cStr());
    if( idx >= 0)
      modell->setCurrentIndex(idx);
  }
}


void qtsSidebar::iterateModel(int add)
{
  int i   = modell->currentIndex() + add;
  int max = modell->count() - 1 ;

  if(i>max)
    i = 0;
  else if(i<0)
    i=max;

  modell->setCurrentIndex(i);
  emit(changemodel(modell->currentText()));
}

