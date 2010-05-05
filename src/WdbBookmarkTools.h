
#ifndef WDBBOOKMARKTOOLS_H
#define WDBBOOKMARKTOOLS_H

/*
  $Id$

  Copyright (C) 2006 met.no

  Contact information:
  Norwegian Meteorological Institute
  Box 43 Blindern
  0313 OSLO
  NORWAY
  email: diana@met.no
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <iostream>
#include <string>
#include <map>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QString>
#include <QIcon>


class WdbBookmarkItem : public QStandardItem {
private:
  bool    isLocal;

public:
  WdbBookmarkItem() : isLocal(false) {}
  WdbBookmarkItem( const QString & text ):  QStandardItem(text) , isLocal(false) {}
  WdbBookmarkItem( const QIcon & icon, const QString & text ):  QStandardItem(icon,text), isLocal(false) {}
  std::string  toFile();
  void         setLocal()          { isLocal = true; }
  bool         writeToFile() const { return isLocal; }
};



class WdbBookmarkFiles {
private:
  QStandardItemModel* model;
  std::map<std::string,QModelIndex> folders;
  QIcon directoryIcon;
  QIcon bookmarkIcon;
  QIcon recordIcon;
public:
  WdbBookmarkFiles();
  void setModel(QStandardItemModel* m) {model=m;}
  bool read(std::string file);
  void addLine(std::string line);

};

#endif
