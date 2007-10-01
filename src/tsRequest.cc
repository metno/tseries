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
#include <tsRequest.h>


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


