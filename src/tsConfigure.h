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
#ifndef _tsConfigure_h
#define _tsConfigure_h

#include <puTools/miString.h>

#include <vector>
#include <map>


class tsConfigure {

public:

  bool read(const miutil::miString&);

  bool get(const miutil::miString& key, miutil::miString& cont);
  bool get(const miutil::miString& key, float& cont);
  bool get(const miutil::miString& key, bool& cont);
  bool get(const miutil::miString& key, int& cont);

  void set(miutil::miString key, const miutil::miString token);
  void set(miutil::miString key, const float token);
  void set(miutil::miString key, const bool token);
  void set(miutil::miString key, const int token);

  std::vector<miutil::miString> getCustoms();
  std::vector<miutil::miString> getList(miutil::miString);

  bool save(miutil::miString);

private:

  struct tsCustoms {
    miutil::miString name;
    std::vector<miutil::miString> list;
  };

  static std::map<miutil::miString,miutil::miString> contents;
  static std::vector<tsCustoms> custom;


  enum { PUBLIC,STYLE,LIST} sec;

  int its,itl;

  void fetchSection(miutil::miString);
  void setSimpleToken(miutil::miString);
  void stripComments(miutil::miString&);

  bool splitToken(const miutil::miString&,miutil::miString&, miutil::miString&);

  void setToken(miutil::miString);
  void setStyle(miutil::miString&, miutil::miString&);
  void setList(miutil::miString);
  void setDefaults();

};

#endif









