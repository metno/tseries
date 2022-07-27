/*
  Tseries - A Free Meteorological Timeseries Viewer

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

#include <puTools/miTime.h>
#include <string>

class tsRequest {
public:
  enum Streamtype { HDFSTREAM, FIMEXSTREAM };

private:
  std::string mod_;
  int run_;
  std::string pos_;
  std::string sty_;

  std::string posname_;

  Streamtype        streamtype;

  double            fimexLat;
  double            fimexLon;
  std::string  fimexModel;
  std::string  fimexStyle;
  std::string  fimexName;
  std::string  fimexRun;

  bool setString(const std::string&, std::string&);

public:
  tsRequest() : run_(-1), streamtype(tsRequest::FIMEXSTREAM), fimexStyle("Meteogram") {}


  bool setModel(const std::string& i) {              return setString(i,mod_); }
  bool setPos(  const std::string& i) { posname_= i; return setString(i,pos_); }
  bool setPos(  const std::string& n1, const std::string& n2) { posname_= n2; return setString(n1,pos_); }
  bool setStyle(const std::string& i) { return setString(i,sty_); }
  void setType(tsRequest::Streamtype s);
  bool setRun(int);

  const std::string& model() const { return mod_; }
  const std::string& style() const { return sty_; }
  int run() const { return run_; }
  const std::string& pos() const { return pos_; }
  const std::string& posname() const { return posname_; }
  tsRequest::Streamtype type() const { return streamtype;}
  std::string file(const std::string& type) const;

  bool setFimexModel(const std::string& nmod) { return setString(nmod, fimexModel); }
  bool setFimexStyle(const std::string& nsty) { return setString(nsty, fimexStyle); }
  bool setFimexRun(const std::string& nrun) { return setString(nrun, fimexRun); }
  bool setFimexLocation(double flat, double flon, const std::string& name);

  const std::string& getFimexModel() const { return fimexModel; }
  const std::string& getFimexStyle() const { return fimexStyle; }
  const std::string& getFimexRun() const { return fimexRun; }
  bool getFimexLocation(double& lat, double& lon, std::string& name);

  friend std::ostream& operator<<(std::ostream&, const tsRequest&);
};

#endif
