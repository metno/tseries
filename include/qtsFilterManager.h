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
#ifndef _qtsFilterManager_h
#define _qtsFilterManager_h



#include <qdialog.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qstringlist.h>
#include <qlayout.h>

#include <miString.h>

#include <vector>
#include <set>



using namespace std; 

class qtsFilterManager : public QDialog 
{
  Q_OBJECT
  
private:
  QVBoxLayout*   vlayout;

  QPushButton*   removeB;
  QPushButton*   copyB;
  QPushButton*   reloadB;

  QListBox    *  all;
  QListBox    *  filtered;

  QStringList original;

  QStringList createList(const set<miString>&);
private slots: 
  void reload();
  void remove();
  void copy();

public:
  qtsFilterManager(const set<miString>&,
		   const set<miString>&,
		   const set<miString>&,
		   QWidget* parent = 0);

  
  set<miString> result();



};

#endif
