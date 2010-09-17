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
#include "tsSetup.h"
#include <iostream>
#include <fstream>
#include <puMet/symbolMaker.h>

using namespace std;
using namespace miutil;

vector<tsSetup::dsStruct>  tsSetup::streams;

tsSetup::wdbstruct         tsSetup::wdb;
tsSetup::fistruct          tsSetup::files;
tsSetup::svstruct          tsSetup::server;
tsSetup::ptstruct          tsSetup::path;
tsSetup::gustruct          tsSetup::gui;
tsSetup::distruct          tsSetup::diana;
tsSetup::dostruct          tsSetup::doc;
bool                       tsSetup::Initialised = false;
miString                   tsSetup::lang;

symbolMaker tsSetup::wsymbols;

tsSetup::tsSetup() : sec(PUBLIC) , line(0)
{
  if(!Initialised) {
    // giving standard values...
    idx=0;
    ids=0;
    wdb.readtime=2000;
    wdb.maxRecord=20;
  }
}

// General:

miString tsSetup::inSection()
{
  switch(sec) {
  case PUBLIC:
    return  ", in <public>";
  case FILES:
    return  ", in <files>";
  case STREAMS:
   return  ", in <streams>";
  case SERVER:
    return ", in <server>";
  case PATH:
    return ", in <path>";
  case GUI:
    return ", in <gui>";
  case DIANA:
    return ", in <diana>";
  case DOC:
    return ", in <doc>";

  case WDB:
    return ", in <wdb>";
  case WDBPARAMETER:
    return  ", in <wdbparameter>";
  case WDBVECTORFUNCTIONS:
    return  ", in <wdbVectorFunctions>";
  }
  return ",in <unknown>";


};

void tsSetup::warn(miString& k, warning w)
{
  cerr << "Warning! ";

    switch(w) {
    case wKEY:
      cerr <<"unexpected Key: "   << k << inSection();
      break;
    case wTOKEN:
      cerr <<"unexpected Token: " << k << inSection();
      break;
    case wSECTION:
      cerr <<"unknown Section: " << k;
      break;
    case wFILE:
      cerr <<"Setupfile: " << k <<" doesn\'t exist" << endl;
      return;
    }
  cerr << " in " << fname << " at line " << line << endl;
}


void tsSetup::fetchSite(miString token)
{
  token = token.upcase();
  token.replace("[","");
  token.replace("]","");
  token.trim();

  vector<miString> tokens = token.split(':');
  actualSites.clear();

  if(tokens.size() < 1 )
     return;

  for(unsigned int i=0;i<tokens.size();i++)
    actualSites.insert(tokens[i]);
}


void tsSetup::fetchSection(miString token)
{
  token = token.upcase();

  if( token.contains("PUBLIC"))
    sec =  PUBLIC;
  else if( token.contains("FILES"))
    sec =  FILES;
  else if( token.contains("STREAMS"))
    sec = STREAMS;
  else if( token.contains("SERVER"))
    sec = SERVER;
  else if( token.contains("PATH"))
    sec = PATH;
  else if( token.contains("GUI"))
    sec = GUI;
  else if( token.contains("DIANA"))
    sec = DIANA;
  else if( token.contains("DOC"))
    sec = DOC;
  else if( token.contains("WDBPARAMETER"))
    sec = WDBPARAMETER;
  else if ( token.contains("WDBVECTORFUNCTIONS"))
    sec = WDBVECTORFUNCTIONS;
  else if( token.contains("WDB"))
    sec = WDB;
  else
    warn(token,wSECTION);

}

void tsSetup::setup(int& to, const miString& from)
{
  to = atoi(from.cStr());
}
void tsSetup::setup(float& to, const miString& from)
{
  to = atof(from.cStr());
}
void tsSetup::setup(miString& to, const miString& from)
{
  to = from;
}

bool tsSetup::splitToken(const miString& token,miString& key,miString& content, bool upper)
{
  content ="";
  if(!token.contains("="))
     return false;

  vector<miString> vtmp = token.split('=');

  key = ( upper ? vtmp[0].upcase() : vtmp[0] );

  if(vtmp.size() ==2)
    content = vtmp[1];
  return true;
}

void tsSetup::stripComments(miString& token)
{
  if(token.contains("#")) {
    int c = token.find_first_of("#",0);
    int k=token.length() -  c;
    token.erase(c,k);
  }
  token.trim();
}


bool tsSetup::read(const miString& f, miString s)
{
  site  = s.upcase();

  path.home      = getenv("HOME");
  lookup["HOME"] = path.home;

  files.filter = path.home + "/.tseries/tseries.filter";
  files.bookmarks=path.home + "/.tseries/bookmarks.def";

  if(!readsetup(f))
    if(!readsetup( path.home+"/.tseries/tseries.ctl"))
      if(!readsetup("/usr/local/etc/tseries/tseries.ctl")) {
	cerr << "NO setup found!" << endl;
	return false;
      }
  return true;
}

bool tsSetup::readsetup(miString fname)
{
  ifstream in(fname.cStr());

  cerr << "Trying setupfile  : ............. " << fname;

  if(!in) {
    cerr << " - Not found!" << endl;
    return false;
  }
  cerr << " - OK!" <<  endl;

  miString token;

  while(in) {
    getline(in,token);
    line++;
    stripComments(token);

    if(!token.exists())
      continue;

    if(token.size() < 2)
      continue;

    if(token[0] =='<' && token[token.size()-1] == '>' ) {
      fetchSection(token);
      continue;
    }
    if(token[0] =='[' && token[token.size()-1] == ']' ) {
      fetchSite(token);
      continue;
    }

    setSimpleToken(token);
  }

  in.close();

  if(!Initialised)
    wsymbols.readSymbols(files.weatherSymbols);

  Initialised = true;

  return true;

}

bool tsSetup::checkLookup(miString& t)
{
  if(!t.contains("$("))
    return false;

  int start,stop;

  start = t.find("$(",0) + 2;
  stop  = t.find(")",start);

  if(stop < start ) {
    warn(t,wTOKEN);
    return false;
  }

  miString s = t.substr(start, stop-start);
  miString r = miString("$(") + s + ")";
  miString n;
  s = s.upcase();

  if( lookup.count(s) > 0 )
    n = lookup[s];

  t.replace(r,n);
  return true;
}



bool tsSetup::checkEnvironment(miString& t)
{
if(!t.contains("${"))
    return false;

  int start,stop;

  start = t.find("${",0) + 2;
  stop  = t.find("}",start);

  if(stop < start ) {
    warn(t,wTOKEN);
    return false;
  }

  miString s = t.substr(start, stop-start);
  miString r = miString("${") + s + "}";

  s = s.upcase();

  miString n = getenv(s.cStr());

  t.replace(r,n);
  return true;
}


void tsSetup::setSimpleToken(miString token)
{
  if(!actualSites.empty())
    if(!actualSites.count(site))
      return;

  if(sec==WDBVECTORFUNCTIONS) {
    wdb.vectorFunctions.push_back(token);
    return;
  }

  bool upper=(sec != WDBPARAMETER);

  miString content,key;
  if(!splitToken(token,key,content,upper))
    return;

  while(checkLookup(content))
    ;

  while(checkEnvironment(content))
    ;

  lookup[key] = content;

  switch(sec) {
  case  PUBLIC:
    setPublic(key,content);
    break;
  case   FILES:
    setFiles(key,content);
    break;
  case  STREAMS:
    setStreams(key,content);
    break;
  case  SERVER:
    setServer(key,content);
    break;
  case GUI:
    setGui(key,content);
    break;
  case DIANA:
    setDiana(key,content);
    break;
  case DOC:
    setDoc(key,content);
    break;
  case PATH:
    setPath(key,content);
    break;
  case WDB:
    setWdb(key,content);
    break;
  case WDBPARAMETER:
    setWdbParameter(key,content);
    break;
  }
}



// DIRECT SET: INSERT NEW STUFF HERE!!!!--------------------------


void tsSetup::setPublic(miString& key, miString& content)
{
  if(key == "LANG")
    setup(lang,content);

}

void tsSetup::setFiles(miString& key, miString& content)
{
  if(key == "DEFS" )
    setup(files.defs,content);
  else if(key == "CONFIGURE")
    setup(files.configure,content);
  else if(key == "WEATHERSYMBOLS")
     setup(files.weatherSymbols,content);
  else if(key == "STDIMAGE")
     setup(files.std_image,content);
  else if(key == "FINIMAGE")
     setup(files.fin_image,content);
  else if(key == "ICONIMAGE")
     setup(files.icon_image,content);
  else if(key == "BASEFILTER")
     setup(files.baseFilter,content);
  else if(key == "COMMONBOOKMARKS")
    setup(files.commonBookmarks,content);
  else if(key == "BOOKMARKS")
       setup(files.bookmarks,content);
  else
    warn(key,wKEY);
}


/////////////////////////////////////

void tsSetup::setStreams(miString& key, miString& content)
{
  if (key.contains("COLLECTIONNAME")) {
    streams.push_back(dsStruct());
    idx=streams.size()-1;
    ids=0;

    setup(streams[idx].collectionName,content);
    return;
  }

  if(streams.empty()) return;

  if (key == "INITIALOPEN") {
    setup(streams[idx].InitialOpen,content);
    return;
  }
  else if (key == "PREFERREDDIAGRAM") {
    setup(streams[idx].preferredStyle,content);
    return;
  }
  else if (key == "DATAFILE" ) {
    streams[idx].data.push_back(sStruct());
    ids=streams[idx].data.size()-1;
    setup(streams[idx].data[ids].name,content);
    return;
  }

  if(streams[idx].data.empty())
    return;

  if(key == "DATADESCRIPTION" )
     setup(streams[idx].data[ids].descript,content);
  else if(key == "DATATYPE" )
    setup(streams[idx].data[ids].type,content);
  else if(key == "CONTENTS")
    setup(streams[idx].data[ids].contents,content);
  else
    warn(key,wKEY);

}

void tsSetup::setWdbParameter(miString& key, miString& content)
{
   wdb.parameters[key]=content;
}


void tsSetup::setWdb(miString& key, miString& content)
{
  if(key == "HOST" )
     setup(wdb.host,content);
   else if(key == "USER")
     setup(wdb.user,content);
   else if(key=="READTIME") {
     int rtime;
     setup(rtime,content);
     wdb.readtime=rtime;
   } else if(key == "BUSYMOVIE") {
     setup(wdb.busyMovie,content);
   }else if (key == "MAXRECORD") {
     setup(wdb.maxRecord,content);
   }else
     warn(key,wKEY);
}



void tsSetup::setServer(miString& key, miString& content)
{
  if(key == "CLIENT" )
    setup(server.client,content);
  else if (key == "COMMAND" )
    setup(server.command,content);
  else if (key == "NAME" )
    setup(server.name,content);
  else
    warn(key,wKEY);

}

void tsSetup::setGui(miString& key, miString& content)
{
  if(key == "ORIGOLON")
    setup(gui.origoLon,content);
  else if (key == "ORIGOLAT")
    setup(gui.origoLat,content);
  else if (key == "STYLE")
    setup(gui.style,content);
  else
    warn(key,wKEY);
}


void tsSetup::setPath(miString& key, miString& content)
{
  if(key == "WORK" )
     setup(path.work,content);
  else if(key == "IMAGES" )
    setup(path.images,content);
  else if(key == "ETC" )
    setup(path.etc,content);
  else if(key == "TMP" )
    setup(path.tmp,content);
  else if(key == "SAVES")
    setup(path.saves,content);
  else if(key == "DOC")
    setup(path.doc,content);
  else if(key == "LANG")
    path.lang = content.split(":");
  else
    warn(key,wKEY);
}

void tsSetup::setDiana(miString& key, miString& content)
{
  if(key == "NAME" )
    setup(diana.name,content);
  else if (key == "COMMAND" )
    setup(diana.command,content);
  else if (key == "WORKDIR" )
    setup(diana.workdir,content);
  else if (key == "ARGS" )
    setup(diana.args,content);
  else
    warn(key,wKEY);
}


void tsSetup::setDoc(miString& key, miString& content)
{
  if(key == "MAINSOURCE" )
    setup(doc.mainSource,content);
  else if (key == "MAINNAME" )
    setup(doc.mainName,content);
  else if (key == "MAINLINK" )
    setup(doc.mainLink,content);
  else if (key == "NEWSSOURCE" )
    setup(doc.newsSource,content);
  else if (key == "NEWSNAME" )
    setup(doc.newsName,content);
  else if (key == "NEWSLINK" )
    setup(doc.newsLink,content);
  else
    warn(key,wKEY);
}

void tsSetup::overrideToken(miutil::miString line )
{
  vector<miString> vl=line.split(":");
  if(vl.size() < 2 ) {
    cerr << "Override token failed with " << line << " no section found! Usage [section:key=token]" << endl;
    return;
  }
  miString section= vl[0];
  miString token  = vl[1];

  cout << "fetching section: -------- " << section << endl;

  fetchSection(section);

  cout << " set simple token: -------- " << token << endl;

  setSimpleToken(token);
}



