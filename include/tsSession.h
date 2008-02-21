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
// Header fil for SessionManager
#ifndef _SessionManager_h
#define _SessionManager_h

#include <puTools/miString.h>
#include <puTools/miTime.h>

#include <pets2/ptGlobals.h>
#include <tsData/ptParameterDefinition.h>
#include <pets2/ptStyle.h>

#include <tsSessionOptions.h>
#include <tsRequest.h>
#include <vector>
#include <map>

#ifdef Bool
#undef Bool
#endif 

using namespace std;

// Should have a list of required models for a station 
// for each style type
class SessionManager {

  
private:
  ParameterDefinition pdef;
  struct modeldata{       // Menynavn og id til aktuelle modeller
    miString modelname;
    Model modelid;
  };
  vector<modeldata> models;
  
  struct pardata{
    int midx;             // model index
    vector<ParId> params; // the parameters
  };
  struct styledata{
    miString stylename;         // name of style
    miString stylefile;         // name of stylefile
    ptStyle  style;             // pets-style
    bool modelchoice;           // whether there is a choice of models
    vector<int> modelidx;       // model indices for modelchoice
    vector<ParId> params;       // id without model
    vector<pardata> fullparams; // full id with model
  };
  vector<styledata> styles;
public:

  // get defined stylenames, return number of style
  int getStyleTypes(vector<miString>& stylenam);
  // get a PETS style by name
  ptStyle& getStyle(const miString);
  // get a PETS style by index
  ptStyle& getStyle(int idx);
  // make a sessionoption with given style,model and run
  bool getShowOption(SessionOptions&, int, Model, Run);
  bool getShowOption(SessionOptions&,const tsRequest *);


  // get a list of available models for a given styleindex
  int getModels(const miString& s, 
		map<miString,Model>& modid,
		vector<miString>& modname);
  // get a list of available runs for style/model
  int getRuns(const int sidx, const int midx,
	      vector<Run>& runid,
	      vector<miString>& runname);
  
  // read diagram-sessions from file
  void readSessions(const miString& fname,
		    bool verbose = false);
  // for test-purposes
  void makeTestSessions();
};

#endif









