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
#include "tsConfigure.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace miutil;

map<miString,miString>             tsConfigure::contents;
vector<tsConfigure::tsCustoms>     tsConfigure::custom;



void tsConfigure::setDefaults()
{
  contents.clear();
  contents["SORT"]   = "alpha";     // alphabetical sort. alternative: origo
  contents["LON" ]   = "20";        // lon for origo
  contents["LAT" ]   = "66";        // lat  --"--
  contents["ALL"]    = "0";         // station list is not style depended
  contents["SAVEONQUIT"] = "1";     // save settings on quit
  contents["SHOWNORMAL"] = "1";     // show pos names in DIANA
  contents["SHOWSELECT"] = "1";     // show selected pos names in DIANA
  contents["TIMEMARK"]   = "1";     // show now in Diagram
  contents["VISIBLESTART"] ="0";
  contents["VISIBLELENGTH"]="0";
  contents["LOCKHOURSTOMODEL"]="0";
  contents["SHOWGRIDLINES"]="1";

}

void tsConfigure::fetchSection(miString token)
{
  token = token.upcase();

  if ( token.contains("STYLE")) {
    sec =  STYLE;
    its++;
  }
  else if( token.contains("LIST")) {
    sec = LIST;
    itl++;
  }
  else
    sec =  PUBLIC;
}


bool tsConfigure::splitToken(const miString& token,miString& key,miString& cont)
{
  cont ="";
  if(!token.contains("="))
     return false;

  vector<miString> vtmp = token.split('=');

  key = vtmp[0].upcase();

  if(vtmp.size() ==2)
    cont = vtmp[1];
  return true;
}

void tsConfigure::stripComments(miString& token)
{
  if(token.contains("#")) {
    int c = token.find_first_of("#",0);
    int k=token.length() -  c;
    token.erase(c,k);
  }
  token.trim();
}


bool tsConfigure::read(const miString& fname)
{
  ifstream in(fname.cStr());

  setDefaults();

  if(!in)
    return false;

  miString token;
  its = -1;
  itl = -1;

  sec = PUBLIC;

  while(in) {
    getline(in,token);
    stripComments(token);

    if(!token.exists())
      continue;
    if(token.contains("<") && token.contains(">")) {
      fetchSection(token);
      continue;
    }
    setToken(token);
  }

  in.close();

  return true;

}

void tsConfigure::setToken(miString token)
{
  miString cont,key;

  if(sec == LIST )
    setList(token);

  if(!splitToken(token,key,cont))
    return;

  if(sec == STYLE )
    setStyle(key,cont);
   else
    contents[key] = cont;
}

void tsConfigure::set(miString key, const miString token)
{
  contents[key.upcase()] = token;
}

void tsConfigure::set(miString key, const int token)
{
  contents[key.upcase()] = miString(token);
}

void tsConfigure::set(miString key, const float token)
{
  contents[key.upcase()] = miString(token);
}

void tsConfigure::set(miString key, const bool token)
{
  contents[key.upcase()] = ( token ? "1" : "0");
}



void tsConfigure::setStyle(miString& key, miString& cont)
{

}


void tsConfigure::setList(miString token)
{
  miString cont,key;
  if(splitToken(token,key,cont)) {
    if( key == "NAME" ) {
      if(cont.exists())
	custom[itl].name = cont;
      else
	custom[itl].name = "Untitled" + miString(itl);
    }
  }
  else
    custom[itl].list.push_back(token);
}

vector<miString> tsConfigure::getList(miString search)
{
  for(unsigned int i=0; i< custom.size();i++)
    if(custom[i].name == search)
      return custom[i].list;
  vector<miString> empty;
  return  empty;
}

vector<miString> tsConfigure::getCustoms()
{
  vector<miString> names;
  for(unsigned int i=0; i< custom.size();i++)
    names.push_back(custom[i].name);
  return names;
}


bool tsConfigure::get(const miString& key, miString& cont)
{
  if(contents.count(key)) {
    cont = contents[key];
    return true;
  }
  return false;
}


bool tsConfigure::get(const miString& key, int& cont)
{
  if(contents.count(key)) {
    cont = atoi(contents[key].cStr());
    return true;
  }
  return false;
}

bool tsConfigure::get(const miString& key, float& cont)
{
  if(contents.count(key)) {
    cont = atof(contents[key].cStr());
    return true;
  }
  return false;
}

bool tsConfigure::get(const miString& key, bool& cont)
{
  if(contents.count(key)) {
    int a = atoi(contents[key].cStr());
    cont = bool(a);
    return true;
  }
  return false;
}



bool tsConfigure::save(miString fname)
{
  ofstream out(fname.cStr());
  map<miString,miString>::iterator itr = contents.begin();

  if(!out)
    return false;

  out << "## Auto-generated config file for tseries do not edit!"
      <<  endl;

  for(;itr != contents.end();itr++)
    out << itr->first << "=" << itr->second << endl;

  for( unsigned int i=0; i< custom.size(); i++) {
    out << "<LIST>" << endl
	<< "name="  << custom[i].name << endl;
    for(unsigned int j=0; j< custom[i].list.size(); j++ )
      out << custom[i].list[j] << endl;

  }


  out.close();
  return true;
}
