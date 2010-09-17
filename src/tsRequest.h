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
#include <puTools/miTime.h>

using namespace std;

class tsRequest {
public:
  enum Streamtype {HDFSTREAM, WDBSTREAM};

private:
  miutil::miString mod_;
  int              run_;
  miutil::miString pos_;
  miutil::miString sty_;

  miutil::miString posname_;

  double            wdbLat;
  double            wdbLon;
  miutil::miTime    wdbRun;
  miutil::miString  wdbStyle;
  miutil::miString  wdbModel;
  unsigned long     wdbReadTime;
  Streamtype        streamtype;
  miutil::miString  wdbstationname;


  bool setString(const miutil::miString&, miutil::miString&);

public:
  tsRequest() : run_(-1), wdbReadTime(0), streamtype(tsRequest::HDFSTREAM), wdbStyle("Meteogram") {}

  bool setModel(const miutil::miString& i) {              return setString(i,mod_); }
  bool setPos(  const miutil::miString& i) { posname_= i; return setString(i,pos_); }
  bool setPos(  const miutil::miString& n1, const miutil::miString& n2) { posname_= n2; return setString(n1,pos_); }
  bool setStyle(const miutil::miString& i) { return setString(i,sty_); }
  void setType(tsRequest::Streamtype s);
  bool setRun(int);

  bool setWdbPos(double lon,double lat);
  bool setWdbRun(miutil::miTime nrun);
  bool setWdbModel(miutil::miString nmod) { return setString(nmod,wdbModel);  }
  bool setWdbStyle(miutil::miString nsty) { return setString(nsty,wdbStyle);  }
  void setWdbReadTime(unsigned long t)    { wdbReadTime =t; }
  void setWdbStationName(miutil::miString nname) { wdbstationname=nname;}


  bool restoreWdbFromLog(miutil::miString mod, miutil::miString sty, double lat, double lon, miutil::miTime run, miutil::miString posname="");

  double           getWdbLat()      const { return wdbLat;      }
  double           getWdbLon()      const { return wdbLon;      }
  miutil::miTime   getWdbRun()      const { return wdbRun;      }
  miutil::miString getWdbModel()    const { return wdbModel;    }
  miutil::miString getWdbStyle()    const { return wdbStyle;    }
  unsigned int     getWdbReadTime() const { return wdbReadTime; }
  miutil::miString getWdbStationName()const { return wdbstationname;}

  miutil::miString model()     const { return mod_;}
  miutil::miString style()     const { return sty_;}
  int              run()       const { return run_;}
  miutil::miString pos()       const { return pos_;}
  miutil::miString posname()   const { return posname_;}
  tsRequest::Streamtype type() const { return streamtype;}
  miutil::miString file(const miutil::miString type) const;


  friend ostream& operator<<(ostream&, const tsRequest&);

};

#endif
