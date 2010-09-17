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
// sessionManager

#include "tsSession.h"
#include <fstream>

using namespace std;
using namespace miutil;

static ptStyle emptysty;

int SessionManager::getStyleTypes(vector<miString>& stylename)
{
  if (styles.size()){
    for (unsigned int i=0; i<styles.size();i++){
      stylename.push_back(styles[i].stylename);
    }
    return (signed int)styles.size();
  } else return 0;
}


int SessionManager::getWdbStyles(vector<miString>& stylename)
{
  int wdbidx=-1;
  for(int i=0;i<models.size();i++)
    if(models[i].modelid=="WDB") {
      wdbidx=i;
      break;
    }

  if (styles.size()){
    for (unsigned int i=0; i<styles.size();i++){
      for( int j=0; j<styles[i].modelidx.size(); j++ )
        if( styles[i].modelidx[j] == wdbidx) {
          stylename.push_back(styles[i].stylename);
          break;
        }
    }
    return (signed int)styles.size();
  } else return 0;

}




ptStyle& SessionManager::getStyle(const miString name)
{
  return getStyle( getStyleIndex(name) );
}


int SessionManager::getStyleIndex(const miString name)
{
  for (unsigned int i=0; i<styles.size();i++)
    if (name==styles[i].stylename)
      return i;
  return -1;
}



ptStyle& SessionManager::getStyle(int idx){
  if (idx>=0 && idx <(signed int)styles.size()){
    return styles[idx].style;
  } else {
    return emptysty;
  }
}

int SessionManager::getModels(const miString& stylename,
			      map<miString,Model>& modid,
			      vector<miString>& modname){
  int midx;
  int idx = -1;
  for (unsigned int i=0; i<styles.size();i++)
    if (stylename==styles[i].stylename)
      idx = i;
  modid.clear();

  // return models contained in given style
  if (idx>=0 && idx <(signed int)styles.size()){
    if (styles[idx].modelchoice){
      for (int i=0; i<(signed int)styles[idx].modelidx.size();i++){
	midx = styles[idx].modelidx[i];
	modname.push_back(models[midx].modelname);
	modid[models[midx].modelname] =  models[midx].modelid;
      }
      return modname.size();
    } else { // style does not permit modelchoices, but it still needs models!!
      for (unsigned int i=0; i<styles[idx].fullparams.size();i++){
	midx = styles[idx].fullparams[i].midx;
	modname.push_back(models[midx].modelname);
	modid[models[midx].modelname] =  models[midx].modelid;
      }
      return -modname.size(); // return a negative number to indicate "no modelchoice"
    }
  } else if (idx==-1) { // return all models
    for (unsigned int i=0; i<models.size();i++){
      modname.push_back(models[i].modelname);
      modid[models[i].modelname] =  models[i].modelid;
    }
    return modname.size();
  }
  return 0;
}


int SessionManager::getRuns(const int sidx, const int midx,
			    vector<Run>& runid,
			    vector<miString>& runname){
  runid.clear();
  runname.clear();

  if (sidx==-1){
    runid.push_back(0);
    runid.push_back(6);
    runid.push_back(12);
    runid.push_back(18);

    runname.push_back("00");
    runname.push_back("06");
    runname.push_back("12");
    runname.push_back("18");

  } else {
    runid.push_back(0);
    runid.push_back(6);
    runid.push_back(12);
    runid.push_back(18);

    runname.push_back("00");
    runname.push_back("06");
    runname.push_back("12");
    runname.push_back("18");

  }
  return runname.size();
}


bool SessionManager::getShowOption(SessionOptions& opt,
				   int idx,
				   Model model,
				   Run run)
{
  int i,j,midx;
  ParId parid;
  Model mod;
  int moptidx=-1,temp;
  opt.Erase();

  if (idx<0 || idx >=(signed int)styles.size()) return false;

  // first add full paramid's
  for (i=0;i<(signed int)styles[idx].fullparams.size();i++){
    midx = styles[idx].fullparams[i].midx;
    temp = opt.addModel(models[midx].modelid, models[midx].modelname);

    if (temp!=-1){ // addModel ok..
      if (models[midx].modelid == model)
        moptidx = temp; // remember this index for later
      for (j=0;j<(signed int)styles[idx].fullparams[i].params.size();j++){
        opt.addParam(styles[idx].fullparams[i].params[j],i);
      }
    }
  }

  // then add modeldependent id's
  if (styles[idx].modelchoice){
    for (i=0;i<(signed int)styles[idx].modelidx.size();i++){
      midx = styles[idx].modelidx[i];
      mod  = models[midx].modelid;
      if (model == mod){
        if (moptidx<0) // check if already added
          moptidx=opt.addModel(mod,models[midx].modelname);
        if (moptidx!=-1){
          for (j=0;j<(signed int)styles[idx].params.size();j++){
            parid = styles[idx].params[j];
            parid.model = mod;
            opt.addParam(parid,moptidx);
          }
        }
      }
    }
  }
  return true;
}

bool SessionManager::getShowOption(SessionOptions& opt,
				   const tsRequest* req)
{
  int idx = -1;
  miString stylename = req->style();
  Model mod = req->model();
  Run run = req->run();

  for (unsigned int i=0; i< styles.size();i++)
    if (stylename==styles[i].stylename)
      idx = i;

  return getShowOption(opt,idx,mod,run);
}

bool SessionManager::checkEnvironment(miString& t)
{
  if(!t.contains("${"))
    return false;
  
  int start,stop;
  
  start = t.find("${",0) + 2;
  stop  = t.find("}",start);
  
  if(stop < start ) {
    cerr << "Missing end }" << endl;
    return false;
  }
  
  miString s = t.substr(start, stop-start);
  miString r = miString("${") + s + "}";
  
  s = s.upcase();
  
  miString n = getenv(s.cStr());
  
  t.replace(r,n);
  return true;
}

void SessionManager::readSessions(const miString& fname,
				  bool verbose){
  const int f_models= 1;
  const int f_diagram= 2;
  const int f_legalmodels= 3;
  const int f_modelindependent= 4;
  const int f_modelspecific= 5;

  ifstream sfile;
  miString buf, keyw, argu;
  vector<miString> parts;

  ParId parid = ID_UNDEF;
  Model model= M_UNDEF;
  styledata sdata;
  pardata pdata;
  modeldata mdata;
  int i,j,n,m;
  int nums= 0;
  int status= 0;

  if (verbose) cout << "Reading diagramdefinitions from:" << fname << endl;
  sfile.open(fname.cStr());
  if (sfile.bad()){
    cerr << "SessionManager::readSessions. Can't open file: " << fname << endl;
    return;
  }

  // clean up styles and models
  styles.clear();
  models.clear();

//   buf.setMaxIstream(150);

  while (sfile.good()){
//     sfile >> buf;
    getline(sfile,buf);
    buf.trim();
    checkEnvironment(buf);
    // discard empty lines and comments
    if (!buf.length() || buf[0]=='#') continue;
    // check for commands (keyword in brackets)
    if (buf[0]=='['){
      if (buf.contains("DIAGRAM")){
	// Starting new diagram-definition
	sdata.modelchoice= false;
	sdata.modelidx.erase(sdata.modelidx.begin(),
			     sdata.modelidx.end());
	sdata.params.erase(sdata.params.begin(),
			   sdata.params.end());
	sdata.fullparams.erase(sdata.fullparams.begin(),
			       sdata.fullparams.end());
	pdata.params.erase(pdata.params.begin(),
			   pdata.params.end());
	status= f_diagram;

      } else if (buf.contains("ADD")){
	if (status!=f_models){
	  // Add a new diagram
	  styles.push_back(sdata);
	  // read style
	  if (styles[nums].stylefile.exists())
	    styles[nums].style.readStyle(styles[nums].stylefile,
					 verbose);
	  else
	    cerr << "ptStyle::readStyle. name of stylefile undefined" << endl;
	  nums++;
	}
      } else if (buf.contains("LEGALMODELS")){
	// Starting list of legal models for modelindependent params
	status= f_legalmodels;
      } else if (buf.contains("MODELINDEPENDENT")){
	// Starting list of modelindependent params
	status= f_modelindependent;
      } else if (buf.contains("MODELSPECIFIC")){
	// Starting a new list of modelspecific params
	status= f_modelspecific;
	model= M_UNDEF;
	pdata.midx= -1;
	pdata.params.erase(pdata.params.begin(),
			   pdata.params.end());
      } else if (buf.contains("END")){
	if (status==f_modelspecific){
	  // Ended a modelspecific list
	  if (pdata.params.size() && pdata.midx!=-1)
	    sdata.fullparams.push_back(pdata);
	  else
	    cerr << "rgSessionManager::readSessions." <<
	      " error in modelspecific list for diagram: " <<
	      sdata.stylename << endl;
	}
      } else if (buf.contains("MODELS")){
	// Starting global model definition
	status= f_models;
      } else
	cout << "rgSessionManager::readSessions: unknown command:" <<
	  buf << endl;
      continue;
    }
    // split into keyword and argument
    parts= buf.split('=',true);
    if (parts.size()<2) continue;
    keyw= parts[0];
    argu= parts[1];
    // global modellist
    if (status==f_models){
      mdata.modelname= argu;
      mdata.modelid= keyw;
      if (mdata.modelid != M_UNDEF){
	// add a new model-def to global list
	models.push_back(mdata);
      }
      continue;
    }
    // diagram-specific
    if (keyw=="NAME") {
      // name of diagram-def. (for menues etc.)
      sdata.stylename= argu;
    }
    else if (keyw=="STYLEFILE") {
      // name of file containing pets style-definition
      sdata.stylefile= argu;
    }
    // model
    else if (keyw=="MODEL"){
      // check if model in global list
      model= argu;
      n= models.size();
      for (i=0; i<n; i++){
	if (model==models[i].modelid){
	  if (status==f_legalmodels){
	    // add to list of legal models for independent parameters
	    sdata.modelchoice= true;
	    sdata.modelidx.push_back(i);
	  } else if (status==f_modelspecific){
	    // model for specific parameters
	    pdata.midx= i;
	    // set model for parameters already defined
	    m= pdata.params.size();
	    for (j=0; j<m; j++) pdata.params[j].model= model;
	  }
	  break;
	}
      }
      if (i==n)
	cerr << "ptStyle::readStyle. model: " <<
	  argu << " for diagram: " << sdata.stylename <<
	  " not found in global list" << endl;
      continue;
    }
    // parameter
    else if (keyw=="PARAM"){
      // convert string to proper parameter-id
      parid= pdef.Str2ParId(argu);
      if (status==f_modelindependent){
	// add a new param to independent list
	sdata.params.push_back(parid);
      } else if (status==f_modelspecific){
	// add a new param to specific list
	parid.model= model;
	pdata.params.push_back(parid);
      }
      continue;
    }
  }
  sfile.close();
}
