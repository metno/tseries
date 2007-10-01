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

#include <miString.h>

#include <vector>
#include <map>



using namespace std; 

class tsConfigure {

public:
	
  bool read(const miString&);

  bool get(const miString& key, miString& cont);
  bool get(const miString& key, float& cont);
  bool get(const miString& key, bool& cont);
  bool get(const miString& key, int& cont);

  void set(miString key, const miString token);
  void set(miString key, const float token);
  void set(miString key, const bool token);
  void set(miString key, const int token);
  
  vector<miString> getCustoms();
  vector<miString> getList(miString);
  
  bool save(miString);

private:

  struct tsCustoms {
    miString name;
    vector<miString> list;
  };

  static map<miString,miString> contents;
  static vector<tsCustoms> custom;


  enum { PUBLIC,STYLE,LIST} sec;
  
  int its,itl;
  
  void fetchSection(miString);
  void setSimpleToken(miString);
  void stripComments(miString&);

  bool splitToken(const miString&,miString&, miString&);
  
  void setToken(miString);
  void setStyle(miString&, miString&);
  void setList(miString);
  void setDefaults();

};

#endif









