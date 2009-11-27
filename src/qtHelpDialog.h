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
#ifndef _helpdialog_h
#define _helpdialog_h


#include <qdialog.h>
#include <qfont.h>
#include <qpalette.h>
#include <Q3VBoxLayout>
#include <Q3HBoxLayout>
#include <QLabel>
#include <puTools/miString.h>
#include <vector>

using namespace std;

class Q3VBoxLayout;
class Q3HBoxLayout;
class QPushButton;
class Q3TextBrowser;
class QLabel;


class HelpDialog: public QDialog {
  Q_OBJECT
public:

  struct Info {

    struct Source {
      miutil::miString source;
      miutil::miString name;
      miutil::miString defaultlink;
    };
    miutil::miString path;
    vector<Source> src;
  };

  HelpDialog( QWidget* parent, const Info& hdi );

  void addFilePath( const miutil::miString& filepath );
  void setSource( const miutil::miString& source );

  void Print();
  miutil::miString helpPath() const {return info.path;}

public slots:
  void showdoc(const int doc, const miutil::miString tag ="");

private slots:
  void hideHelp();

private:
  void ConstructorCernel(const miutil::miString& filepath, const miutil::miString& source);

  Q3TextBrowser* tb;

  Info info;

  QPushButton* pushbackward;
  QPushButton* pushforward;
  QPushButton* closebutton;
  QLabel* plabel;

  Q3VBoxLayout* vlayout;
  Q3HBoxLayout* hlayout;

  QFont m_font;
};


#endif
