
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
#include <set>


class WdbBookmarkTools {
private:
  std::set<std::string> ignores;
  QStandardItemModel*   model;
  std::map<std::string,QModelIndex> folders;
  QIcon directoryIcon;
  QIcon recordIcon;
  QIcon metnoIcon;
  QIcon trashIcon;
  int record;
  int maxRecords;

public:
  WdbBookmarkTools();
  void setModel(QStandardItemModel* m) {model=m;}
  bool read(     std::string file, bool ignoreFromSave);
  void write(    std::string file );
  void addLine(  std::string line, bool ignoreFromSave,bool reverse=false);
  void addFolder(std::string name, bool ignoreFromSave);
  void addRecord(float lon,float lat);
  void cutRecord();
  void setMaxRecords(int m) { maxRecords=m;}

  QStandardItem * createFolder(std::string folder,bool ignoreFromSave);
};

#endif
