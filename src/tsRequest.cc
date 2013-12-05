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
#include "tsRequest.h"

using namespace std;
using namespace miutil;
#include <boost/algorithm/string.hpp>
#include <sstream>

bool tsRequest::setString(const miString& i, miString& o)
{
  if(o == i) return false;
  o = i;
  return true;

}


bool tsRequest::setRun(int i)
{
  if(run_ == i) return false;
  run_ = i;
  return true;
}

ostream& operator<<(ostream& out, const tsRequest& rhs){

  out <<           rhs.sty_
      << " : " <<  rhs.pos_
      << " : " <<  rhs.mod_
      << " "   <<  rhs.run_;
 return out;
};

miString tsRequest::file(const miString type) const
{
  miString rst = mod_ + "_" + pos_;

  rst.replace(".","");

  rst.replace(" ","_");
  rst.replace("�","OE");
  rst.replace("�","AA");
  rst.replace("�","AE");
  rst.replace("�","UE");
  rst.replace("�","OE");
  rst.replace("�","AE");

  rst.replace("�","oe");
  rst.replace("�","aa");
  rst.replace("�","ae");
  rst.replace("�","ue");
  rst.replace("�","oe");
  rst.replace("�","ae");

  rst = rst.downcase();

  rst +="."+type;
  return rst;
}

/// WDB
bool tsRequest::setWdbPos(double lon,double lat)
{
    if( (fabs(lon-wdbLon) < 0.00001 ) && (fabs(lat-wdbLat) < 0.00001 )) return false;
    wdbLat=lat;
    wdbLon=lon;
    return true;
}

bool tsRequest::setWdbRun(miutil::miTime nrun)
{
  if(nrun == wdbRun) return false;
  wdbRun = nrun;
  return true;
}


void tsRequest::setType(tsRequest::Streamtype s)
{
  streamtype=s;
}



bool tsRequest::restoreWdbFromLog(miutil::miString mod, miutil::miString sty, double lat, double lon, miutil::miTime run,miString posname)
{
  setWdbPos(lon,lat);
  setWdbStationName(posname);
  setWdbRun(run);
  setWdbModel(mod);
  setWdbStyle(sty);
}



bool  tsRequest::setFimexLocation(double flat,double flon, std::string name)
{
  if(name == fimexName)
    return false;

  fimexLat = flat;
  fimexLon = flon;
  fimexName= name;
  return true;
}

bool tsRequest::getFimexLocation(double& lat, double& lon, miutil::miString& name)
{
  lat = fimexLat;
  lon = fimexLon;
  name = fimexName;

  if(lat==0 && lon==0 && name =="")
    return false;
  return true;
}


std::string  tsRequest::getFimexInfo()
{
  ostringstream ost;
  ost << "fimexLat:   " <<  fimexLat     << endl
      << "fimexLat:   " <<  fimexLat     << endl
      << "fimexLon:   " <<  fimexLon     << endl
      << "fimexModel: " <<  fimexModel   << endl
      << "fimexStyle: " <<  fimexStyle   << endl
      << "fimexName:  " <<  fimexName    << endl
      << "fimexRun:   " <<  fimexRun     << endl;
  return ost.str();
}





