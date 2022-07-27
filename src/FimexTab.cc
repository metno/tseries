/*
 * FimexTab.cc
 *
 *  Created on: Mar 16, 2010
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

#include "FimexTab.h"

#include "tsSetup.h"

#include <QFont>
#include <QPixmap>
#include <QMenu>
#include <QEvent>
#include <QtGui>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <iostream>
#include <sstream>

using namespace std;

FimexTab::FimexTab(QWidget* parent, QString lang)   : QWidget(parent)
{
  tsSetup setup;
  fetchstations = new FetchStations(QString::fromStdString(setup.fimex.externalPosService), lang);

  connect(fetchstations,SIGNAL(searchResult(std::vector<std::string>)),this,
      SLOT(searchResult(std::vector<std::string>)));

  recordingPositions=false;
  addToRecord=false;

  activeCacheRequest=false;
  modell    = new QComboBox(this);
  runl      = new QComboBox(this);
  stylel    = new QComboBox(this);

  connect(stylel,SIGNAL(activated(const QString&)),    this, SIGNAL(changestyle(const QString&)));
  connect(modell,SIGNAL(activated(const QString&)),    this, SLOT(changeModel(const QString&)));
  connect(runl,SIGNAL(activated(const QString&)),      this, SLOT(changeRun(const QString&)));

  QStringList head;
  head << tr("Bookmarks");

  bookmarks = new QTreeView(this);
  bookmarks->setMinimumHeight(1);
  bookmarks->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(bookmarks,SIGNAL( customContextMenuRequested ( const QPoint &)), this, SLOT(showContextMenu(const QPoint& )));

  model = new QStandardItemModel();
  model->setHorizontalHeaderLabels ( head );

  variableBookmarkfile = setup.files.fimexBookmarks;

  bookmarkTools.setModel(model);
  bookmarkTools.addFolder("TRASH",true);
  bookmarkTools.addFolder("RECORD",true);
  bookmarkTools.addFolder("SEARCH",true);

  bookmarkTools.read(variableBookmarkfile,false);
  bookmarkTools.read(setup.files.commonBookmarks,true);

  proxyModel = new FilterProxyModel(this);
  proxyModel->setSourceModel(model);
  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

  bookmarks->setModel(proxyModel);

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

  QVBoxLayout * vlayout     = new QVBoxLayout();
  vlayout->addWidget(stylel);
  vlayout->addWidget(modell);
  vlayout->addWidget(runl);
  vlayout->addWidget(bookmarks,3);

  filter = new ClearLineEdit(this);
  connect(filter,SIGNAL(textChanged(const QString&)),this, SLOT(filterBookmarks(const QString&)));
  connect(filter,SIGNAL(search(QString)),this,SLOT(search(QString)));
  filter->setPlaceholderText(tr("Set Filter"));

  vlayout->addWidget(filter);
  setLayout(vlayout);
}

void FimexTab::filterBookmarks(const QString& text)
{
  proxyModel->setFilterFixedString(text);
  selectFirstPosition();
  emit changePoslist();
}

void FimexTab::setCoordinates(float lon, float lat, QString name)
{
  cerr << "SET COORDINATES WITH VALUES: " << name.toStdString() << " " << lat << "  :  " << lon << endl;

  if(fabs(latitude -lat ) < 0.00001 )
    if(fabs(longitude -lon ) < 0.00001 )
      return;

  longitude = lon;
  latitude  = lat;

  if(name.isEmpty()) {
    ostringstream ost;
    ost << bookmarkTools.createRecordName(lon,'E','W') << " "
        << bookmarkTools.createRecordName(lat,'N','S');

    name.fromLatin1(ost.str().c_str());
  }

  if(recordingPositions) {
    if(addToRecord) {
      bookmarkTools.addRecord(lon,lat,name.toStdString());
      emit changePoslist();
    }
  } else {
    if(!name.isEmpty())
      emit changeCoordinates(lon,lat,name);
  }

  addToRecord=true;
}

void FimexTab::coordinatesChanged()
{
  bookmarkTools.addRecord(longitude,latitude,"");
  emit changeCoordinates(longitude,latitude,"");
}

miCoordinates FimexTab::coordinates() const
{
  return miCoordinates(longitude, latitude);
}

QString FimexTab::setStyles(const QStringList& qlist)
{
  QString cur = stylel->currentText();

  stylel->clear();
  stylel->addItems(qlist);

  if(!cur.isEmpty())  {
    setStyle(cur);
  }
  return stylel->currentText();
}

void FimexTab::setStyle(const QString nstyle)
{
  int idx = stylel->findText(nstyle);
  if( idx >= 0)
    stylel->setCurrentIndex(idx);
}

void FimexTab::setModel(const QString nmodel)
{
  int idx = modell->findText(nmodel);
  if( idx >= 0) {
    modell->setCurrentIndex(idx);
    activeCacheRequest=false;
    emit changemodel(nmodel);
  }
}

void FimexTab::setRun(const QString nrun)
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

void FimexTab::writeBookmarks()
{
  bookmarkTools.write(variableBookmarkfile);
}

void FimexTab::setModels(const QStringList& newmodels)
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

void FimexTab::setRuns(const QStringList& newruns)
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

void FimexTab::changeModel(const QString& s)
{
  activeCacheRequest=false;
  emit changemodel(s);
}

void FimexTab::changeRun(const QString& s)
{
  activeCacheRequest=false;
  emit changerun(s);
}

bool FimexTab::findPosition(QString newpos, QModelIndex& found_idx)
{
  unsigned int num_rows = model->rowCount();
  for (unsigned int row=0; row <num_rows; row++ ) {
    QModelIndex idx = proxyModel->index(row,0);
    if(bookmarks->isExpanded(idx)) {

      QStandardItem * item = model->itemFromIndex(proxyModel->mapToSource(idx));
      if(item) {

        unsigned int num_item_rows=item->rowCount();
        for(unsigned int item_row=0;item_row < num_item_rows;item_row++) {
          QStandardItem * child = item->child(item_row,0);

          if(child) {

            QString  name=child->text();

            if (QString::compare(name, newpos, Qt::CaseInsensitive) == 0) {
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

void FimexTab::changePosition(QString newpos)
{
  QModelIndex idx;
  if(findPosition(newpos,idx)) {
    bookmarks->setCurrentIndex(proxyModel->mapFromSource(idx));
    bookmarkClicked(proxyModel->mapFromSource(idx));
  }
}

void FimexTab::bookmarkClicked(QModelIndex idx)
{
  QStandardItem *item = model->itemFromIndex(proxyModel->mapToSource(idx));
  if(!item) {
    return;
  }

  QVariant var =item->data();
  QString  coor=var.toString();
  QString  name=item->text();

  string s=coor.toStdString();
  vector<string> c;
  boost::split(c,s, boost::algorithm::is_any_of(":") );
  if(c.size() < 2) return;

  float lat= atof(c[0].c_str());
  float lon= atof(c[1].c_str());

  addToRecord=false;

  setCoordinates(lon,lat,name);
}

void FimexTab::addBookmarkFolder()
{
  bookmarkTools.addFolder("NEW",false);
}

void FimexTab::poslistChanged(const QModelIndex &)
{
  emit changePoslist();
}

std::string FimexTab::getExpandedDirs()
{
  ostringstream ost;
  unsigned int num_rows = model->rowCount();
  std::string delimiter="";
  for (unsigned int row=0; row <num_rows; row++ ) {
    QModelIndex idx = model->index(row,0);
    if(bookmarks->isExpanded(idx)) {
      QStandardItem * item = model->itemFromIndex(proxyModel->mapToSource(idx));
      if(item) {
        ost <<   delimiter << item->text().toStdString();
        delimiter = ",";
      }
    }
  }
  return ost.str();
}

void FimexTab::setExpandedDirs(std::string e)
{
  vector<string> exdirs;
  boost::split(exdirs,e, boost::algorithm::is_any_of(",") );

  for(size_t i=0; i < exdirs.size();i++) {
    QString dir = QString::fromStdString(exdirs[i]);
    QList<QStandardItem *> items = model->findItems(dir);
    if(!items.isEmpty()) {
      QModelIndex idx= model->indexFromItem(items.front());
      bookmarks->expand(proxyModel->mapToSource(idx));
    }
  }
}

QList<QStringList> FimexTab::getPoslist()
{
  set<QString> positionfilter;
  QList<QStringList> activePositions;
  unsigned int num_rows = model->rowCount();

  const QRegExp & filterreg = proxyModel->filterRegExp();
  bool noFilterApplied      = filter->text().isEmpty();

  for (unsigned int row=0; row <num_rows; row++ ) {
    QModelIndex idx = proxyModel->index(row,0);
    if(bookmarks->isExpanded(idx)) {
      QStandardItem * item = model->itemFromIndex(proxyModel->mapToSource(idx));
      if(item) {

        unsigned int num_item_rows=item->rowCount();
        for(unsigned int item_row=0;item_row < num_item_rows;item_row++) {
          QStandardItem * child = item->child(item_row,0);

          if(child) {
            QString  name=child->text();
            if (filterreg.indexIn(name) == 0 ||noFilterApplied ) {
              if (!positionfilter.count(name)) {
                positionfilter.insert(name); // avoid doublets
                QVariant var = child->data();
                QStringList coor = var.toString().split(":");
                activePositions << (QStringList() << name << coor);
              }
            }
          }
        }
      }

    }
  }
  return activePositions;
}

void FimexTab::recordToggled(bool rec)
{
  recordingPositions=rec;

  QModelIndex recordIdx= bookmarkTools.getRecordFolderIndex();

  if(recordingPositions) {
    bookmarks->expand(proxyModel->mapFromSource(recordIdx));
    return;
  }

  emit newPoslist();
}

void FimexTab::cut()
{
  QItemSelection selections = proxyModel->mapSelectionToSource(bookmarks->selectionModel()->selection());

  QModelIndexList selectedIndexes = selections.indexes();

  bookmarkTools.copySelected(selectedIndexes);
  bookmarkTools.removeSelected(selectedIndexes);
  emit changePoslist();
}

void FimexTab::remove()
{
  QItemSelection selections = proxyModel->mapSelectionToSource(bookmarks->selectionModel()->selection());
  QModelIndexList selectedIndexes = selections.indexes();

  bookmarkTools.removeSelected(selectedIndexes);
  emit changePoslist();
}

void FimexTab::copy()
{
  QItemSelection selections = proxyModel->mapSelectionToSource(bookmarks->selectionModel()->selection());
  QModelIndexList selectedIndexes = selections.indexes();

  bookmarkTools.copySelected(selectedIndexes);
}

void FimexTab::paste()
{
  QModelIndex index = bookmarks->currentIndex();
  bookmarkTools.paste(proxyModel->mapToSource(index));
}

void FimexTab::showContextMenu(const QPoint& pos)
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

void FimexTab::expandAll()
{
  bookmarks->expandAll();
  emit changePoslist();
}

void FimexTab::collapseAll()
{
  bookmarks->collapseAll();

  emit changePoslist();
}

bool FilterProxyModel::filterAcceptsRow(int row, const QModelIndex &parent) const
{
  // accept highest level (region)
  if (not parent.isValid())
    return true;

  const QRegExp & filter = filterRegExp();
  const QString stationname = sourceModel()->data(parent.child(row, 0)).toString();
  if (filter.indexIn(stationname) == 0) {
    return true;
  }

  // do not match other columns
  return false;
}

void FimexTab::selectFirstPosition()
{
  if(filter->text().isEmpty())
   return;

  unsigned int num_rows = model->rowCount();

  const QRegExp & filterreg = proxyModel->filterRegExp();

  for (unsigned int row=0; row <num_rows; row++ ) {
    QModelIndex idx = proxyModel->index(row,0);
    if(bookmarks->isExpanded(idx)) {
      QStandardItem * item = model->itemFromIndex(proxyModel->mapToSource(idx));
      if(item) {

        unsigned int num_item_rows=item->rowCount();
        for(unsigned int item_row=0;item_row < num_item_rows;item_row++) {

          QStandardItem * child = item->child(item_row,0);

          if(child) {

            QString  name=child->text();

            if (filterreg.indexIn(name) == 0  ) {

              changePosition(name);
              return;
            }
          }
        }
      }
    }
  }
}

void FimexTab::search(QString query)
{
  fetchstations->getData(query);
}

void FimexTab::searchResult(const std::vector<std::string>& stat)
{
  for (std::vector<std::string>::const_iterator it=stat.begin(); it!=stat.end(); ++it) {
      bookmarkTools.addSearch(*it);
   }

  addToRecord=false;
  QModelIndex searchIdx= bookmarkTools.getSearchFolderIndex();

  if (!stat.empty())
    bookmarks->expand(proxyModel->mapFromSource(searchIdx));

  Q_EMIT newPoslist();
}
