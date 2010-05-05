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
#ifndef _tsSetup_h
#define _tsSetup_h

#include <puTools/miString.h>
#include <puMet/symbolMaker.h>

#include <vector>
#include <map>
#include <set>


using namespace std;

/// T-series setup read and storage
/** All Tseries setup is read in this class
 *  usually represented in the tseries.ctl file.
 *  The class has only one public function ( read() ) with the filename
 *  as input. All Parameters er stored in static structs.
 *  After generating a tsSetup with read() one can
 *  tsSetup setup
 *  miutil::miString x=setup.path.work
 */


class tsSetup {
private:
  static bool           Initialised;
  miutil::miString      site;
  set<miutil::miString> actualSites;

  bool readsetup(miutil::miString filename);

public:
  tsSetup();

  /// Function to create a setup object
  /** Takes in a filename and optional a switch.
   *  switches er defined in the setup-file as\n
   *  [MYSWITCH]\n
   *  and end with\n
   *  [ANOTHERSWITCH] or \n[]\n
   *  everything inside the switch is invisible to
   *  anybody except those who are using the specific switch\n
   *  Switches are free choosable strings and can be combined.
   *  A section [ME:YOU] is valid for ME and YOU but noone else.
   *  By this, several people with different needs are able to
   *  share one setup-file without getting severe conflicts.
  */
  bool read(const miutil::miString& filename, miutil::miString s="");

  // override with section:key=token
  void overrideToken(miutil::miString token);

  // contents

  /// Struct containing a single PETS datastream
  struct sStruct{
    miutil::miString  name;
    miutil::miString  descript;
    miutil::miString  type;
    miutil::miString  contents;
  };

  /// Struct containing datastreams
  struct dsStruct {
    int  InitialOpen;
    miutil::miString collectionName;
    miutil::miString preferredStyle;
    vector<sStruct> data;
  };

  struct wdbstruct {
    miutil::miString    host;
    miutil::miString    user;
    miutil::miString    busyMovie;
    unsigned long       readtime;            /// time to enable cache button (ms)
    vector<std::string> vectorFunctions;     ///< run vector transformations on these
    map<std::string,std::string> parameters; ///< translate parameters from wdb
  };


  /// Struct containing filenames for tseries
  struct fistruct {
    miutil::miString defs;
    miutil::miString configure;
    miutil::miString weatherSymbols;
    miutil::miString std_image;
    miutil::miString fin_image;
    miutil::miString icon_image;
    miutil::miString filter;
    miutil::miString baseFilter;
    miutil::miString bookmarks;
  };

  /// Struct containing coserver-info
  struct svstruct {
    miutil::miString name;
    miutil::miString command;
    miutil::miString client;
  };

  /// Struct containing path-names
  struct ptstruct {
    miutil::miString work;
    miutil::miString images;
    miutil::miString etc;
    miutil::miString tmp;
    miutil::miString saves;
    miutil::miString doc;
    miutil::miString home;
    vector<miutil::miString> lang;
  };

  /// Struct containing GUI-specific information
  struct gustruct {
    float origoLon;
    float origoLat;
    miutil::miString style;
  };

  /// Struct containing information about DIANA
  struct distruct {
    miutil::miString name;
    miutil::miString command;
    miutil::miString workdir;
    miutil::miString args;
  };

  /// Struct containing documentation path etc.
  struct dostruct {
    miutil::miString mainSource;
    miutil::miString mainName;
    miutil::miString mainLink;
    miutil::miString newsSource;
    miutil::miString newsName;
    miutil::miString newsLink;
  };

  static vector<dsStruct> streams; ///< Data streams

  static wdbstruct wdb;            ///< wdb information
  static fistruct files;           ///< All filenames
  static svstruct server;          ///< Server information
  static ptstruct path;            ///< General path info
  static gustruct gui;             ///< GUI info
  static distruct diana;           ///< DIANA information
  static dostruct doc;             ///< Documentation locations
  static miutil::miString lang;            ///< Languages

  static symbolMaker wsymbols;

private:
  enum { PUBLIC, FILES, STREAMS, SERVER, GUI, PATH, DIANA, DOC,WDB,WDBPARAMETER,WDBVECTORFUNCTIONS} sec;
  enum warning { wKEY, wTOKEN, wSECTION, wFILE };

  map<miutil::miString,miutil::miString> lookup;
  miutil::miString fname;
  int line;
  int idx,ids;

  void fetchSite(miutil::miString);
  void fetchSection(miutil::miString);
  void setSimpleToken(miutil::miString);
  bool checkLookup(miutil::miString&);
  bool checkEnvironment(miutil::miString&);
  void stripComments(miutil::miString&);

  miutil::miString inSection();
  bool splitToken(const miutil::miString&,miutil::miString&, miutil::miString&);
  void warn(miutil::miString&,warning);

  void setPublic(miutil::miString&, miutil::miString&);
  void setFiles(miutil::miString&, miutil::miString&);
  void setStreams(miutil::miString&, miutil::miString&);
  void setServer(miutil::miString&, miutil::miString&);
  void setGui(miutil::miString&, miutil::miString&);
  void setPath(miutil::miString&, miutil::miString&);
  void setDiana(miutil::miString&, miutil::miString&);
  void setDoc(miutil::miString&, miutil::miString&);
  void setWdb(miutil::miString&, miutil::miString&);
  void setWdbParameter(miutil::miString&, miutil::miString&);


  void setup(miutil::miString&, const miutil::miString&);
  void setup(int&,const miutil::miString&);
  void setup(float&, const miutil::miString&);

};

#endif
