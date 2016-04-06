/*
 * StationTab.cc
 *
 *  Created on: Mar 15, 2010
 *      Author: juergens
 */

/*
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

#include "StationTab.h"

using namespace std;

StationTab::StationTab(QWidget* parent) : QWidget(parent)
{
  QVBoxLayout * vlayout = new QVBoxLayout(this);

  modell  = new QComboBox(this);
  runl    = new QComboBox(this);
  stylel  = new QComboBox(this);
  statl   = new QListWidget(this);
  searchw = new QLineEdit(this);

  statl->setMinimumHeight(1);


  // showing the positions

  pos_label = new QLabel("<b>Position</b>");
  pos_label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  QFont thisfont=this->font();
  thisfont.setPointSize(8);
  pos_label->setFont(thisfont);


  vlayout->addWidget(stylel);
  vlayout->addWidget(modell);
  vlayout->addWidget(runl);
  vlayout->addWidget(statl);
  vlayout->addWidget(searchw);
  vlayout->addWidget(pos_label);

  connect(searchw,SIGNAL(textChanged(const QString&)),this, SLOT(searchStation(const QString&)));
  connect(stylel,SIGNAL(activated(const QString&)),   this, SIGNAL(changestyle(const QString&)));
  connect(modell,SIGNAL(activated(const QString&)),   this, SIGNAL(changemodel(const QString&)));
  connect(runl,SIGNAL(activated(const QString&)),     this, SIGNAL(changerun(const QString&)));
  connect(statl,SIGNAL(currentItemChanged (    QListWidgetItem *, QListWidgetItem *)),
          this, SLOT  (currentStationChanged(  QListWidgetItem *, QListWidgetItem *)));
}


void StationTab::currentStationChanged ( QListWidgetItem * current, QListWidgetItem * previous )
{
  if(current) {
    QString st = current->text();
    Q_EMIT changestation(st);
  }
}

void StationTab::searchStation(const QString& str )
{
  QList<QListWidgetItem *>  items = statl->findItems (str, Qt::MatchStartsWith );
  if(!items.empty())
    statl->setCurrentItem(items.first());
}


QString StationTab::fillStations(const QStringList& qlist )
{
  if(qlist.isEmpty()) return "";
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

QString StationTab::station()
{
  QListWidgetItem* item=statl->currentItem();
   if(item)
     return item->text();
}


QString StationTab::fillList(const QStringList& qlist, const StationTab::lEntry c)
{
  QComboBox * co =  modell;

  if (c == CMRUN)
    co = runl;
  if (c == CMSTYLE)
    co = stylel;

  QString cur = co->currentText();

  co->clear();
  co->addItems(qlist);

  if(!cur.isEmpty()) {
    int idx = co->findText(cur);
    if( idx >= 0)
      co->setCurrentIndex(idx);
  }

  return co->currentText();
}

QString StationTab::current(const StationTab::lEntry c)
{

  if(c == CMSTYLE)
    return stylel->currentText();
  if(c == CMRUN)
    return runl->currentText();
  return modell->currentText();
}


void StationTab::set(const std::string& cur,const StationTab::lEntry c)
{
  int idx;
  if(c == CMSTYLE) {
    idx = stylel->findText(cur.c_str());
    if( idx >= 0)
      stylel->setCurrentIndex(idx);
  } else if(c == CMRUN) {
    idx = runl->findText(cur.c_str());
    if( idx >= 0)
      runl->setCurrentIndex(idx);
  } else if(c == CMMODEL) {
    idx = modell->findText(cur.c_str());
    if( idx >= 0)
      modell->setCurrentIndex(idx);
  }
}
