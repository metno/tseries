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
#include <tsDrawArea.h>
#include <puMet/miSymbol.h>
#include <puMet/symbolMaker.h>
#include <pets2/ptSymbolElement.h>
#include <pets2/ptTimemarkerElement.h>
#include <pets2/ptFontManager.h>
#include <glText/glTextX.h>

using namespace d_print;

tsDrawArea::tsDrawArea( tsRequest* tsr,
			DatafileColl* tsd,
			SessionManager* ses) 
  : request(tsr), data(tsd), session(ses), theData(0), diagram(0),
    width(1), height(1), pixwidth(1), pixheight(1),
    hardcopy(false), hardcopystarted(false), Initialised(false)
{
  minProg=0;
  maxProg=300;

}


void tsDrawArea::prepare()
{
  if (!Initialised){
    //cout << "Preparing FONTS, display:" << FM.display_name() << endl;
    glText* gltext= new glTextX(FM.display_name());
    gltext->testDefineFonts();
    FM.addFontCollection(gltext, XFONTSET);
    FM.setFontColl(XFONTSET);
    
    Initialised= true;
  }

  if(!prepareData()){
    cerr << "tsDrawArea Warning: prepareData failed" << endl;
    return;
  }
  if(!prepareDiagram()){
    cerr << "tsDrawArea Warning:  prepareDiagram failed" << endl;
  }
}


void tsDrawArea::setViewport(int w,int h,float pw,float ph)
{
  width  = w;
  height = h;
  
  pixwidth  = pw;
  pixheight = ph;
  
  if (diagram)
    diagram->setPixSize(pixwidth, pixheight);
}


bool tsDrawArea::prepareData()
{ 
  SessionOptions options;

  if(!session->getShowOption(options,request)) 
    return false;
  diaStyle = session->getStyle(request->style());

  DataStream *datastream; // Current datafile
  vector<ParId> inlist, outlist;
  int first, last;
  ErrorFlag error;
  ParId modid = ID_UNDEF;
  int i,numstreams,streamidx[10];
  bool datafound;
  WeatherParameter wp;
  miTime btime, etime;
  int currunidx = request->run();
  

  if (theData) 
    delete theData;
  
  theData = new ptDiagramData(setup.wsymbols);

//   cerr << "RUN:" << request->run() << endl;
//   cerr << "POS:" << request->pos() << endl;
//   cerr << "STYLE:" << request->style() << endl;
//   cerr << "MODEL:" << request->model() << endl;

  // fetch data
  for (i=0; i<options.numModels(); i++) {
//     cerr << "option.model:" << options.getmodel(i) << endl;
    inlist = options.paramVector(i); // ParId vector
    if (inlist.size()) {
      modid.model = options.getmodel(i);
      modid.run = ( request->run()>-1)? request->run():R_UNDEF;
      numstreams = data->findModel(modid.model,modid.run,
				   streamidx,10);

      if (numstreams < 1)
	cerr << "tsDrawArea::prepareData, found " << numstreams
	     << " matching streams for model " << modid.model << endl;

      if (numstreams>0) {
	datafound = false;
	for (int j=0; j<numstreams && !datafound;j++){
	  datastream = data->getDataStream(streamidx[j]);
	  if (datastream) {
	    if (!(datastream->isOpen())) {
	      data->openStream(streamidx[j]);
	      datastream= data->getDataStream(streamidx[j]);
	    }
	    miPosition station;
	    station.setName(request->pos());

	    //vector<ParId> parlist;
	    theData->fetchDataFromFile(datastream, station,
				       modid, btime, etime, inlist,
				       &first, &last, 
				       outlist, true, 
				       &error);
	    datafound = ((error!=DF_STATION_NOT_FOUND)&&
			 (error!=DD_NO_PARAMETERS_FOUND));
	    if (datafound && error==DD_SOME_PARAMETERS_NOT_FOUND){
	      // Find any missing params
	      ParId inpid;
	      bool pidok;
	      int inp, outp;
	      vector<ParId> missingwp;
	      for (inp=0;inp<inlist.size();inp++){
		inpid = inlist[inp];
		pidok=false;
		for (outp=0; outp<outlist.size();outp++)
		  if (inpid==outlist[outp]){
		    pidok=true;
		    break;
		  }
		if (!pidok) missingwp.push_back(inpid);
	      }
	      theData->makeParameters(missingwp, true);
	    }
	  }
	}
      }
    }
  }

//   cerr << *theData << endl;
  return true;
}



bool tsDrawArea::prepareDiagram()
{
  if (!theData) {
    cerr << "tsDrawArea::prepareDiagram(): !theData" << endl;
    return(false);
  }
  if (diagram) delete diagram;
  diagram = new ptDiagram(&diaStyle);
 
  if (!diagram->attachData(theData)){
    cerr << "tsDrawArea::prepareDiagram(): !diagram->attachData(theData)" << endl;
    return(false);
  }

  

  // this is the place to change the station name
  if (request->posname() != request->pos()){
    miPosition s= theData->getStation();
    s.setName(request->posname());
    theData->setStation(s);
  }

  ptColor bgColor;
  if (!diagram->makeDefaultPlotElements(&bgColor)){
    cerr << "tsDrawArea::prepareDiagram(): !diagram->makeDefaultPlotElements(&bgColor)" << endl;
    return false;
  }

  diagram->setPixSize(pixwidth, pixheight);

  float bgRGB[3];
  bgRGB[0] = bgColor.colTable[0];
  bgRGB[1] = bgColor.colTable[1];
  bgRGB[2] = bgColor.colTable[2];

  glClearColor(bgRGB[0],bgRGB[1],bgRGB[2],1.0);

  // set correct weathersymbols
  miSymbol tmpSymbol;
  PlotElement* pe=0;
  vector<SymbolElement*> sev;
  int i;
  int minsymb =  setup.wsymbols.minCustom(), maxsymb= setup.wsymbols.maxCustom();
  while (pe = diagram->findElement(SYMBOL,pe)){
    sev.push_back((SymbolElement*)pe);
    pe= pe->next;
  }
  if (sev.size()){
    vector<miString> symbimages;
    miString stmp;
    for(i= minsymb; i<=maxsymb; i++){
      tmpSymbol = setup.wsymbols.getSymbol(i);
      stmp= setup.path.images+tmpSymbol.picture();
      symbimages.push_back(stmp);
    }
    for (i=0; i<sev.size(); i++){
      sev[i]->setImages(minsymb,symbimages);
    }
  }
 
  return true;
}

void tsDrawArea::setTimemark(miTime nt,miString name)
{
  timemarks[name] = nt;
}

void tsDrawArea::clearTimemarks(miString el)
{
  if(!el.exists())
    timemarks.clear();
  
  if(timemarks.count(el))
    timemarks.erase(el);
}

void tsDrawArea::useTimemarks()
{
  if(timemarks.empty() || !theData || !diagram)
    return;

  map<miString, miTime>::iterator itr = timemarks.begin();
  vector<miTime> mark(1);

  for(;itr!=timemarks.end();itr++) {
    PlotElement* pe=0;
    TimemarkerElement* ptm;
    if (pe = diagram->findElement(itr->first,pe)){
      ptm= (TimemarkerElement*)pe;
      mark[0] = itr->second;
      ptm->setTimes(mark);
    } 
  }
}


void tsDrawArea::plot()
{
  if(!diagram)
    return;


  diagram->setProgInterval(minProg, maxProg);
  
  if (hardcopy && !hardcopystarted){
    oco = diagram->getColour();
    ico = (printoptions.colop == incolour);
  
    if (oco != ico)
      diagram->toggleColour(ico);
  
    bool ila  = (printoptions.orientation == ori_landscape);
    bool eps  =  printoptions.doEPS; 
    
    diagram->startPSoutput(printoptions.fname, ico, ila, eps);
    hardcopystarted= true;
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  useTimemarks();
  
  diagram->plot();
  
  glFlush();
}

void tsDrawArea::startHardcopy(const printOptions& po,
			       bool delay_creation)
{
  if(!diagram)
    return;

  if (hardcopy){
    // if hardcopy in progress, and same filename: make new page
    if (po.fname == printoptions.fname){
      diagram->startPSnewpage();
      return;
    }
    // different filename: end current output and start a new
    endHardcopy();
  }
  hardcopy= true;
  printoptions= po;

  if (delay_creation)
    hardcopystarted= false;
  else {
    hardcopystarted= true;

    oco = diagram->getColour();
    ico = (printoptions.colop == incolour);
  
    if (oco != ico)
      diagram->toggleColour(ico);
  
    bool ila  = (printoptions.orientation == ori_landscape);
    bool eps  =  printoptions.doEPS; 
    
    diagram->startPSoutput(printoptions.fname, ico, ila, eps);
    hardcopystarted= true;
  }
}

void tsDrawArea::endHardcopy()
{
  if(!diagram || !hardcopy)
    return;

  diagram->endPSoutput();
  if  (oco != ico)
    diagram->toggleColour(oco); 
  
  hardcopy= false;
}








