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
#include <qaccel.h>

#include <qpixmap.h>
#include "delete.xpm"
#include "copy.xpm"
#include "tb_refresh.xpm"

qtsFilterManager::qtsFilterManager(const set<miString>& p,
				   const set<miString>& f,
				   const set<miString>& o,
				   QWidget* parent)
  : QDialog( parent, "Filter manager", true )
{

  original = createList(o);

  QVBoxLayout*  vlayout  = new QVBoxLayout(this, 10, 10, "vayout");
  

  QVBoxLayout*  bvlayout = new QVBoxLayout(5, "bvlayout"); 
  QHBoxLayout*  hlayout  = new QHBoxLayout(5, "hLayout");
  QHBoxLayout*  bhlayout = new QHBoxLayout(5, "bhLayout");


  
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



  
  QAccel *a = new QAccel( this );

  a->connectItem( a->insertItem(Key_Delete),this, SLOT(remove()));

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

  all = new QListBox(this,"allpos");

  all->setMinimumWidth(250);
  all->insertStringList(posl);

  QAccel *b = new QAccel( all );
  a->connectItem( a->insertItem(Key_Space),this, SLOT(copy()));


  // FILTERED LIST
  QStringList filt = createList(f);

  filtered = new QListBox(this,"filtered");

  filtered->setMinimumWidth(250);
  filtered->insertStringList(filt);
  filtered->setSelectionMode(QListBox::Multi);
  

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
      miString a = filtered->text(i).latin1();
      res.insert(a);
    }
  }
  return res;
};


void qtsFilterManager::reload()
{
  filtered->clear();
  filtered->insertStringList(original);
};


void qtsFilterManager::remove()
{
  int last=0;
  for(int i=0;i < filtered->count(); i++ ) 
    if(filtered->isSelected(i)) {
      filtered->removeItem(i);
      last=i;
      i--;
    }
  
  int max = filtered->count() -1;

  if(max >= 0) {
    if(last > max  )
      last = max;
    filtered->setSelected(last,true);
  }
}

void qtsFilterManager::copy()
{
  QString txt = all->currentText();

  if(!txt.isEmpty()) {
    if(filtered->findItem(txt,Qt::ExactMatch))
      return;

    filtered->insertItem(txt);
  }
}







