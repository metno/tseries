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
#ifndef _tsRequest_h
#define _tsRequest_h

#include <puTools/miString.h>


using namespace std; 

class tsRequest {
private:
  miString mod_;
  int      run_;
  miString pos_;
  miString sty_;

  miString posname_;

  bool setString(const miString&, miString&);
 
public:
  tsRequest() : run_(-1) {}

  bool setModel(const miString& i) { return setString(i,mod_); } 
  bool setPos(  const miString& i) 
  { posname_= i; return setString(i,pos_); } 
  bool setPos(const miString& n1,const miString& n2)
  { posname_= n2; return setString(n1,pos_); }
  bool setStyle(const miString& i) { return setString(i,sty_); } 
  bool setRun(int);


  miString model()  const { return mod_;}
  int      run()    const { return run_;}
  miString pos()    const { return pos_;}
  miString style()  const { return sty_;}
  miString file(const miString type) const;
  miString posname()const { return posname_;}

  friend ostream& operator<<(ostream&, const tsRequest&);

};

#endif
