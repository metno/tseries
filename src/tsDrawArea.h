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
#ifndef _tsDrawArea_h
#define _tsDrawArea_h


#include "tsRequest.h"
#include "tsDatafileColl.h"
#include "tsSession.h"

#include <map>
#include <vector>
#include <puTools/miTime.h>

#include <pets2/ptGlobals.h>
#include <pets2/ptDiagram.h>
#include <tsData/ptWeatherParameter.h>
#include <pets2/ptPlotElement.h>
#include <pets2/ptEditLineElement.h>
#include "tsSetup.h"


//#include <GLP.h>
#include "diPrintOptions.h"

class tsDrawArea {
private:
  tsRequest     * request;
  tsSetup         setup;
  DatafileColl  * data;
  SessionManager* session;

  ptDiagram     * diagram;
  ptDiagramData * theData;
  ptStyle         diaStyle;
  printOptions    printoptions;

  int width;
  int height;
  float glwidth;
  float glheight;
  float pixwidth;
  float pixheight;
  bool  Initialised;
  bool  hardcopy;
  bool  hardcopystarted;
  bool  oco; // original
  bool  ico; // new color

  map<miutil::miString,miutil::miTime> timemarks;
  void useTimemarks();
  bool prepareData();
  bool prepareDiagram();

  bool prepareWdbData();

  int  maxProg;
  int  minProg;

public:
  tsDrawArea( tsRequest* tsr, DatafileColl* tsd, SessionManager* ses);

  void prepare(bool readData = true);

  void setViewport(int w, int h,float,float);

  void startHardcopy(const printOptions&,
		     bool delay_creation= true);
  void endHardcopy();

  void plot();

  void setTimemark(miutil::miTime nt,miutil::miString name="");
  void clearTimemarks(miutil::miString name="");

  void setProgintervall(int mi,int ma)
  {minProg=mi; maxProg=ma;}

};

#endif







