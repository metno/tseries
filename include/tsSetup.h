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

#include <miString.h>

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
 *  miString x=setup.path.work
 */


class tsSetup {
private:
  static bool   Initialised;
  miString      site;
  set<miString> actualSites;
  
  bool readsetup(miString filename);

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
  bool read(const miString& filename,miString s="");
  
  // contents
   
  /// Struct containing a single PETS datastream
  struct sStruct{
    miString  name;
    miString  descript;
    miString  type;
    miString  contents;
  };

  /// Struct containing datastreams
  struct dsStruct {
    int  InitialOpen;
    miString collectionName;
    miString preferredStyle;
    vector<sStruct> data;
  };
 
  /// Struct containing filenames for tseries
  struct fistruct {
    miString defs;
    miString configure;
    miString weatherSymbols;
    miString std_image;
    miString fin_image;
    miString icon_image;
    miString filter;
    miString baseFilter;
  };

  /// Struct containing coserver-info 
  struct svstruct {
    miString name;
    miString command;
    miString client;
  };

  /// Struct containing path-names
  struct ptstruct {
    miString work;
    miString images;
    miString etc;
    miString tmp;
    miString saves;
    miString doc;
    miString home;
    vector<miString> lang;
  };

  /// Struct containing GUI-specific information 
  struct gustruct {
    float origoLon;
    float origoLat;
    miString style;
  };

  /// Struct containing information about DIANA
  struct distruct {
    miString name;
    miString command;
    miString workdir;
    miString args;
  };

  /// Struct containing documentation path etc.
  struct dostruct {
    miString mainSource;
    miString mainName;
    miString mainLink;
    miString newsSource;
    miString newsName;
    miString newsLink;
  };

  static vector<dsStruct> streams; ///< Data streams
  static fistruct files;           ///< All filenames
  static svstruct server;          ///< Server information
  static ptstruct path;            ///< General path info
  static gustruct gui;             ///< GUI info
  static distruct diana;           ///< DIANA information
  static dostruct doc;             ///< Documentation locations
  static miString lang;            ///< Languages

private:
  enum { PUBLIC, FILES, STREAMS, SERVER, GUI, PATH, DIANA, DOC} sec;
  enum warning { wKEY, wTOKEN, wSECTION, wFILE };
     
  map<miString,miString> lookup;
  miString fname;
  int line;
  int idx,ids;
  
  void fetchSite(miString);
  void fetchSection(miString);
  void setSimpleToken(miString);
  bool checkLookup(miString&);
  bool checkEnvironment(miString&);
  void stripComments(miString&);

  miString inSection();
  bool splitToken(const miString&,miString&, miString&);
  void warn(miString&,warning);
       
  void setPublic(miString&, miString&);
  void setFiles(miString&, miString&);
  void setStreams(miString&, miString&);
  void setServer(miString&, miString&);
  void setGui(miString&, miString&);
  void setPath(miString&, miString&);
  void setDiana(miString&, miString&);
  void setDoc(miString&, miString&);

  void set(miString&, const miString&);
  void set(int&,const miString&);
  void set(float&, const miString&);

};

#endif
