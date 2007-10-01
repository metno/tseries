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
// Header file for SessionOptions
#ifndef _SessionOptions_h
#define _SessionOptions_h

#include <vector>

#include <miString.h>
#include <miTime.h>
#include <miPosition.h>

#include <ptGlobals.h>
#include <ptParameterDefinition.h>
#include <ptPrimitiveType.h>

using namespace std;

class SessionOptions {
private:
  struct modeldata{
    Model model;
    miString modelname;
    vector<ParId> parameters;
  };
private:
  vector<modeldata> mdata;
  miTime start, stop;  // timeline interval to use
  miString dialogname; // dialog headline (Customer name etc)
  miPosition station;

  bool idxOk(const int idx) { return (idx>=0 && idx<mdata.size());}

public:

  void Erase()     { mdata.clear();       }
  int numModels()  { return mdata.size(); }
  Model getmodel(const int idx);
  int addModel(const Model, const miString);
 
  // return name of model
  const miString& getmodelname(const int);
  const vector<ParId>& paramVector(const int idx); 
    
  // add a new parameter to a model
  bool addParam(const ParId parid, const int idx);

  // timeinterval
  void getTinterval(miTime& sta, miTime& sto);
  void setTinterval(miTime& sta, miTime& sto);

  // return dialogname

  const miString& getdialogname() { return(dialogname); }
  const miPosition& getstation()  { return(station);    }

  void setdialogname(const miString& n){ dialogname=n;}
  void setstation(const miPosition& s) { station=s;   }

  
};

#endif



