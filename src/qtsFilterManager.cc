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
#include <qtsFilterManager.h>

#include <qtooltip.h>
#include <q3accel.h>

#include <qpixmap.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include "delete.xpm"
#include "copy.xpm"
#include "tb_refresh.xpm"

using namespace std;
using namespace miutil;

qtsFilterManager::qtsFilterManager(const set<miString>& p,
				   const set<miString>& f,
				   const set<miString>& o,
				   QWidget* parent)
  : QDialog( parent, "Filter manager", true )
{

  original = createList(o);

  Q3VBoxLayout*  vlayout  = new Q3VBoxLayout(this, 10, 10, "vayout");


  Q3VBoxLayout*  bvlayout = new Q3VBoxLayout(5, "bvlayout");
  Q3HBoxLayout*  hlayout  = new Q3HBoxLayout(5, "hLayout");
  Q3HBoxLayout*  bhlayout = new Q3HBoxLayout(5, "bhLayout");



  // Vertical Button Tab

  QPixmap copy_pix(copy_xpm);
  QPixmap del_pix(delete_xpm);
  QPixmap reload_pix(tb_refresh_xpm);


  copyB    = new QPushButton(copy_pix,"",this);
  removeB  = new QPushButton(del_pix,"",this);
  reloadB  = new QPushButton(reload_pix,"",this);

  copyB->setMaximumWidth(copy_pix.width()+10);
  removeB->setMaximumWidth(del_pix.width()+10);
  reloadB->setMaximumWidth(reload_pix.width()+10);

  QToolTip::add( copyB,   tr("Copy to filter") );
  QToolTip::add( removeB, tr("Delete from filter" ) );
  QToolTip::add( reloadB, tr("Reset filter" ) );

  bvlayout->addStretch(2);
  bvlayout->addWidget(copyB);
  bvlayout->addWidget(removeB);
  bvlayout->addStretch(2);
  bvlayout->addWidget(reloadB);

  connect(reloadB,SIGNAL(clicked()),this,SLOT(reload()));
  connect(removeB,SIGNAL(clicked()),this,SLOT(remove()));
  connect(copyB,SIGNAL(clicked()),this,SLOT(copy()));




  Q3Accel *a = new Q3Accel( this );

  a->connectItem( a->insertItem(Qt::Key_Delete),this, SLOT(remove()));

  // Horizontal Button Tab

  QPushButton * okB   = new QPushButton(tr("Ok"),this, "okb");
  QPushButton * quitB = new QPushButton(tr("Cancel"),this, "quitb");


  bhlayout->addWidget(okB);
  bhlayout->addWidget(quitB);


  connect(okB,SIGNAL(clicked()),this,SLOT(accept()));
  connect(quitB,SIGNAL(clicked()),this,SLOT(reject()));


  // Horizontal Layout -> list | buttons | list


  // COMPLETE LIST

  QStringList posl = createList(p);

  all = new QListWidget(this);

  all->setMinimumWidth(250);
  all->addItems(posl);

  Q3Accel *b = new Q3Accel( all );
  a->connectItem( a->insertItem(Qt::Key_Space),this, SLOT(copy()));


  // FILTERED LIST
  QStringList filt = createList(f);

  filtered = new QListWidget(this);

  filtered->setMinimumWidth(250);
  filtered->addItems(filt);
  filtered->setSelectionMode(QAbstractItemView::MultiSelection);


  // LAYOUT MANAGING

  hlayout->addWidget(all);
  hlayout->addLayout(bvlayout);
  hlayout->addWidget(filtered);

  vlayout->addLayout(hlayout);
  vlayout->addLayout(bhlayout);

}


QStringList qtsFilterManager::createList(const set<miString>& in)
{
  QStringList slist;
  set<miString>::iterator itr=in.begin();
  for(;itr!=in.end();itr++)
    slist << itr->cStr();
  return slist;
}



set<miString> qtsFilterManager::result()
{
  set<miString> res;
  if(filtered->count()) {
    for(int i=0; i < filtered->count();i++) {
      miString a = filtered->item(i)->text().latin1();
      res.insert(a);
    }
  }
  return res;
};


void qtsFilterManager::reload()
{
  filtered->clear();
  filtered->addItems(original);
};


void qtsFilterManager::remove()
{
  int last=0;

  for(int i=0;i<filtered->count();i++) {
    QListWidgetItem * it = filtered->item(i);
    if(it->isSelected()) {
      it =  filtered->takeItem(i);
      delete it ;
      last=i;
      i--;
    }
  }

  int max = filtered->count() -1;

  if(max >= 0) {
    if(last > max  )
      last = max;
    filtered->setCurrentRow(last);
  }
}

void qtsFilterManager::copy()
{
  QString txt = all->currentItem()->text();
  if(!txt.isEmpty()) {
    QList<QListWidgetItem *> q=filtered->findItems( txt,Qt::MatchExactly);
    if(!q.isEmpty())
      return;

    filtered->addItem(txt);
  }
}







