/*
 * CoordinateTab.cc
 *
 *  Created on: Mar 16, 2010
 *      Author: juergens
 */

/*
 $Id$

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
#include <QFont>
#include "CoordinateTab.h"
#include <iostream>
#include <QPixmap>
#include "tsSetup.h"
#include <QMenu>
#include <QEvent>
#include <QtGui>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>



//#include "list-add.xpm"
#include <sstream>

using namespace std;

CoordinateTab::CoordinateTab(QWidget* parent)   : QWidget(parent)
{
  recordingPositions=false;
  addToRecord=false;

  activeCacheRequest=false;
  modell    = new QComboBox(this);
  runl      = new QComboBox(this);
  stylel    = new QComboBox(this);

  connect(stylel,SIGNAL(activated(const QString&)),    this, SIGNAL(changestyle(const QString&)));
  connect(modell,SIGNAL(activated(const QString&)),    this, SLOT(changeModel(const QString&)));
  connect(runl,SIGNAL(activated(const QString&)),      this, SLOT(changeRun(const QString&)));


  longitude = new CoordinateManager(this,Qt::Horizontal,"Lon");
  latitude  = new CoordinateManager(this,Qt::Vertical,  "Lat");

  longitude->setRange(-180,180);
  latitude->setRange(-90,90);

  connect(longitude,SIGNAL(coordinatesChanged()),this,SLOT(coordinatesChanged()));
  connect(latitude,SIGNAL(coordinatesChanged()),this,SLOT(coordinatesChanged()));

  QStringList head;
  head << tr("Bookmarks");

  bookmarks = new QTreeView(this);
  bookmarks->setMinimumHeight(1);
  bookmarks->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(bookmarks,SIGNAL( customContextMenuRequested ( const QPoint &)), this, SLOT(showContextMenu(const QPoint& )));



  model     = new QStandardItemModel();
  model->setHorizontalHeaderLabels ( head );
  tsSetup setup;

  variableBookmarkfile =  setup.files.wdbBookmarks;


  bookmarkTools.setMaxRecords(setup.wdb.maxRecord);
  bookmarkTools.setModel(model);
  bookmarkTools.addFolder("TRASH",true);
  bookmarkTools.read(variableBookmarkfile,false);
  bookmarkTools.read(setup.files.commonBookmarks,true);
  bookmarkTools.addFolder("RECORD",true);
  bookmarks->setModel(model);

  bookmarks->setDragDropMode(QAbstractItemView::InternalMove);
  bookmarks->setSelectionMode(QAbstractItemView::ExtendedSelection);

  connect(bookmarks, SIGNAL(activated(QModelIndex)),       this, SLOT(bookmarkClicked(QModelIndex)));
  connect(bookmarks, SIGNAL(clicked(QModelIndex)),         this, SLOT(bookmarkClicked(QModelIndex)));
  connect(bookmarks, SIGNAL(expanded(const QModelIndex &)),this, SLOT(poslistChanged(const QModelIndex &)));
  connect(bookmarks, SIGNAL(collapsed(const QModelIndex &)),this, SLOT(poslistChanged(const QModelIndex &)));




  cutAction   = new QAction(tr("C&ut..."),  this);
  copyAction  = new QAction(tr("&Copy..."),  this);
  pasteAction = new QAction(tr("&Paste..."), this);
  delAction   = new QAction(tr("&Delete..."), this);

  cutAction->setShortcut(   QKeySequence::Cut    );
  copyAction->setShortcut(  QKeySequence::Copy   );
  pasteAction->setShortcut( QKeySequence::Paste  );
  delAction->setShortcut(   QKeySequence::Delete );


  connect( cutAction,   SIGNAL( triggered() ) , this, SLOT( cut()  ) );
  connect( copyAction,  SIGNAL( triggered() ) , this, SLOT( copy() ) );
  connect( pasteAction, SIGNAL( triggered() ) , this, SLOT( paste()) );
  connect( delAction,   SIGNAL( triggered() ) , this, SLOT( remove()));


  bookmarks->addAction(cutAction);
  bookmarks->addAction(copyAction);
  bookmarks->addAction(pasteAction);
  bookmarks->addAction(delAction);



  cutAction->setEnabled(true);
  copyAction->setEnabled(true);
  pasteAction->setEnabled(true);

  // layout -------------------------------


  QVBoxLayout * vlayout     = new QVBoxLayout();  // main layout
  QHBoxLayout * latHlayout  = new QHBoxLayout();  // Hor - lat layout amd lvl layout
  QVBoxLayout * latVlayout  = new QVBoxLayout();  // Ver - lat label and slider
  QHBoxLayout * lonHlayout  = new QHBoxLayout();  // Hor - lon label and slider
  QHBoxLayout * editHlayout = new QHBoxLayout();  // Hor - lat/lon edit fields and labels

  vlayout->addWidget(stylel);
  vlayout->addWidget(modell);
  vlayout->addWidget(runl);


  latVlayout->addWidget(latitude->slider);
  latVlayout->addWidget(latitude->sliderLabel);


  latHlayout->addLayout(latVlayout);


  latHlayout->addWidget(bookmarks,3);

  lonHlayout->addWidget(longitude->sliderLabel);
  lonHlayout->addWidget(longitude->slider);
  lonHlayout->setAlignment(Qt::AlignHCenter);


  vlayout->addLayout(latHlayout);
  vlayout->addLayout(lonHlayout);

  editHlayout->addWidget(longitude->editLabel);
  editHlayout->addWidget(longitude->edit);
  editHlayout->addWidget(latitude->editLabel);
  editHlayout->addWidget(latitude->edit);

  vlayout->addLayout(editHlayout);

  setLayout(vlayout);

}


void CoordinateTab::setWdbGeometry(int minLon, int maxLon, int minLat, int maxLat)
{
  longitude->setRange( minLon, maxLon );
  latitude->setRange(  minLat, maxLat );
}

void CoordinateTab::setCoordinates(float lon, float lat, QString name)
{

  float oldlat=latitude->getValue();
  float oldlon=longitude->getValue();

  if(fabs(oldlat -lat ) < 0.00001 )
    if(fabs(oldlon -lon ) < 0.00001 )
      return;

  longitude->setValue(lon);
  latitude->setValue(lat);

  if(name.isEmpty()) {
    ostringstream ost;
    ost << bookmarkTools.createRecordName(lon,'E','W') << " "
        << bookmarkTools.createRecordName(lat,'N','S');

    name.fromLatin1(ost.str().c_str());
  } 


  bookmarkTools.addRecord(lon,lat,name.toStdString());
  emit changeCoordinates(lon,lat,name);
  addToRecord=true;
}

void CoordinateTab::setLatRange(int min, int max)
{
  latitude->setRange(min,max);
}

void CoordinateTab::setLonRange(int min, int max)
{
  longitude->setRange(min,max);
}

void CoordinateTab::coordinatesChanged()
{
  float lat=latitude->getValue();
  float lon=longitude->getValue();
  bookmarkTools.addRecord(lon,lat,"");
  emit changeCoordinates(lon,lat,"");

}

std::string CoordinateTab::coordinateString()
{
  ostringstream ost;
  ost <<  latitude->getValue() << ":" << longitude->getValue();
  return ost.str();
}


QString CoordinateTab::setStyles(const QStringList& qlist)
{

  QString cur = stylel->currentText();

  stylel->clear();
  stylel->addItems(qlist);

  if(!cur.isEmpty()) {
    int idx = stylel->findText(cur);
    if( idx >= 0)
      stylel->setCurrentIndex(idx);
  }
  return stylel->currentText();
}


void CoordinateTab::setStyle(const QString nstyle)
{
  int idx = stylel->findText(nstyle);
  if( idx >= 0)
    stylel->setCurrentIndex(idx);
}

void CoordinateTab::setModel(const QString nmodel)
{
  int idx = modell->findText(nmodel);
  if( idx >= 0) {
    modell->setCurrentIndex(idx);
    activeCacheRequest=false;
    emit changemodel(nmodel);
  }
}

void CoordinateTab::setRun(const QString nrun)
{
  QString newrun=nrun;
  int idx = runl->findText(nrun);
  if( idx >= 0) {
    runl->setCurrentIndex(idx);
  } else {
    newrun=runl->currentText();
  }
  activeCacheRequest=false;
  emit changerun(newrun);
}

void CoordinateTab::writeBookmarks()
{
  bookmarkTools.write(variableBookmarkfile);

}

void CoordinateTab::setModels(const QStringList& newmodels)
{
  QString cur = modell->currentText();

  modell->clear();
  modell->addItems(newmodels);

  if(!cur.isEmpty()) {
    int idx = modell->findText(cur);
    if( idx >= 0) {
      modell->setCurrentIndex(idx);
    }
  }
  QString cur2 = modell->currentText();

  if(cur2 != cur) {
    emit changemodel(cur2);
  }
}

void CoordinateTab::setRuns(const QStringList& newruns)
{
  QString cur = runl->currentText();

  runl->clear();
  runl->addItems(newruns);

  if(!cur.isEmpty()) {
    int idx = runl->findText(cur);
    if( idx >= 0)
      runl->setCurrentIndex(idx);
  }
  QString cur2 = runl->currentText();
  if(cur != cur2)
    emit changerun(cur2);
}
void CoordinateTab::changeModel(const QString& s)
{
  activeCacheRequest=false;
  emit changemodel(s);
}

void CoordinateTab::changeRun(const QString& s)
{
  activeCacheRequest=false;
  emit changerun(s);
}


bool CoordinateTab::findPosition(QString newpos, QModelIndex& found_idx)
{
  unsigned int num_rows = model->rowCount();

  for (unsigned int row=0; row <num_rows; row++ ) {
    QModelIndex idx = model->index(row,0);
    if(bookmarks->isExpanded(idx)) {
      QStandardItem * item = model->itemFromIndex(idx);
      if(item) {
        unsigned int num_item_rows=item->rowCount();
        for(unsigned int item_row=0;item_row < num_item_rows;item_row++) {
          QStandardItem * child = item->child(item_row,0);

          if(child) {
            if(newpos==child->text()){
              found_idx=model->indexFromItem(child);
              return true;
            }
          }
        }
      }

    }

  }
  return false;
}

void CoordinateTab::changePosition(QString newpos)
{
  QModelIndex idx;
  if(findPosition(newpos,idx)) {
    bookmarks->setCurrentIndex(idx);
    bookmarkClicked(idx);
  }

}


void CoordinateTab::bookmarkClicked(QModelIndex idx)
{
  QStandardItem* item=model->itemFromIndex(idx);
  if(!item) return;
  QVariant var =item->data();
  QString  coor=var.toString();
  QString  name=item->text();

  string s=coor.toStdString();
  vector<string> c;
  boost::split(c,s, boost::algorithm::is_any_of(":") );
  if(c.size() < 2) return;

  float lat= atof(c[0].c_str());
  float lon= atof(c[1].c_str());


  setCoordinates(lon,lat,name);
}

void CoordinateTab::addBookmarkFolder()
{
  bookmarkTools.addFolder("NEW",false);
}

void CoordinateTab::poslistChanged(const QModelIndex &)
{
  emit changePoslist();
}

std::string CoordinateTab::getExpandedDirs()
{
  ostringstream ost;
  cerr << "getExpandedDirs" << endl;
  unsigned int num_rows = model->rowCount();
  std::string delimiter="";
  for (unsigned int row=0; row <num_rows; row++ ) {
    QModelIndex idx = model->index(row,0);
    if(bookmarks->isExpanded(idx)) {
      QStandardItem * item = model->itemFromIndex(idx);
      if(item) {
        ost <<   delimiter << item->text().toStdString();
        delimiter = ",";

      }
    }
  }
  return ost.str();
}


void CoordinateTab::setExpandedDirs(std::string e)
{
  vector<string> exdirs;
  boost::split(exdirs,e, boost::algorithm::is_any_of(",") );

  for(int i=0; i< exdirs.size();i++) {
    QString dir = QString::fromStdString(exdirs[i]);
    QList<QStandardItem *> items = model->findItems(dir);
    if(!items.isEmpty()) {
      QModelIndex idx= model->indexFromItem(items.front());
      bookmarks->expand(idx);
    }
  }
}





vector<string> CoordinateTab::getPoslist()
{

  set<QString> positionfilter;
  vector<string> activePositions;
  unsigned int num_rows = model->rowCount();

  for (unsigned int row=0; row <num_rows; row++ ) {
    QModelIndex idx = model->index(row,0);
    if(bookmarks->isExpanded(idx)) {
      QStandardItem * item = model->itemFromIndex(idx);
      if(item) {
        unsigned int num_item_rows=item->rowCount();
        for(unsigned int item_row=0;item_row < num_item_rows;item_row++) {
          QStandardItem * child = item->child(item_row,0);

          if(child) {
            QVariant var =child->data();
            QString  coor=var.toString();
            QString  name=child->text();

            if( !positionfilter.count(name)) {
              positionfilter.insert(name); // avoid doublets
              ostringstream ost;
              ost << name.toStdString() << ":" << coor.toStdString();
              activePositions.push_back(ost.str());
            }
          }
        }
      }

    }
  }
  return activePositions;
}



void CoordinateTab::recordToggled(bool rec)
{
  recordingPositions=rec;


  QModelIndex recordIdx= bookmarkTools.getRecordFolderIndex();

  if(recordingPositions) {
    bookmarks->expand(recordIdx);
    return;
  }

  bookmarks->collapse(recordIdx);

  emit newPoslist();

}


void CoordinateTab::cut()
{
  QItemSelectionModel* selections = bookmarks->selectionModel ();
  QModelIndexList selectedIndexes = selections->selectedIndexes();

  bookmarkTools.copySelected(selectedIndexes);
  bookmarkTools.removeSelected(selectedIndexes);
  emit changePoslist();
}


void CoordinateTab::remove()
{
  QItemSelectionModel* selections = bookmarks->selectionModel ();
  QModelIndexList selectedIndexes = selections->selectedIndexes();

  bookmarkTools.removeSelected(selectedIndexes);
  emit changePoslist();
}


void CoordinateTab::copy()
{
  QItemSelectionModel* selections = bookmarks->selectionModel ();
  QModelIndexList selectedIndexes = selections->selectedIndexes();

  bookmarkTools.copySelected(selectedIndexes);
}


void CoordinateTab::paste()
{
  QModelIndex index = bookmarks->currentIndex();
  bookmarkTools.paste(index);
}


void CoordinateTab::showContextMenu(const QPoint& pos)
{
  cerr << __FUNCTION__ << endl;
  QPoint globalPos = mapToGlobal(pos);

  QMenu pmenu;


  pmenu.addAction(cutAction);
  pmenu.addAction(copyAction);
  pmenu.addAction(pasteAction);
  pmenu.addAction(delAction);

  pmenu.exec(globalPos);


}






