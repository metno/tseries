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
#include "tsDrawArea.h"

#include <puMet/miSymbol.h>
#include <puMet/symbolMaker.h>

#include <pets2/ptSymbolElement.h>
#include <pets2/ptTimemarkerElement.h>

#define MILOGGER_CATEGORY "tseries.tsDrawArea"
#include <miLogger/miLogging.h>

using namespace std;
using namespace miutil;
using namespace pets2;

tsDrawArea::tsDrawArea(tsRequest* tsr, DatafileColl* tsd, SessionManager* ses, QObject* parent)
  : QObject(parent)
  , request(tsr)
  , data(tsd)
  , session(ses)
  , canvas(0)
  , diagram(0)
  , theData(0)
  , showGridLines(true)
{
  minProg = 0;
  maxProg = 300;
  totalLength=300;
  forecastLength=300;
  lengthChanged=false;
  observationStartTime = miTime::nowTime();
  observationStartTime.addHour(-(setup.klima.maxObservationLength));

  datathread = new pets::PrepareDataThread(this);
  connect (datathread, SIGNAL(post_dataLoad(bool)), this , SLOT(dataLoad_finished(bool)));
}

tsDrawArea::~tsDrawArea()
{
}

void tsDrawArea::prepare(bool)
{
  METLIBS_LOG_SCOPE();
  if (prepareData()) {
    prepareDiagram();
  } else {
    METLIBS_LOG_WARN("prepareData failed");
  }
}

void tsDrawArea::setViewport(ptCanvas* c)
{
  canvas = c;
  if (diagram)
    diagram->setViewport(c);
}

bool tsDrawArea::prepareData()
{
  return prepareFimexData();
}

bool tsDrawArea::prepareMoraData(vector<ParId>& inlist)
{
  METLIBS_LOG_SCOPE();
  if (setup.disabled.mora)
    return false;

  forecastLength= theData->timeLineLengthInHours();

  if (showObservations) {
    vector<ParId> obsParameters, unresolvedObs;
    set<ParId> allObservations;
    miTime lastTime=  theData->timelineEnd();

    for (const ParId& p : inlist) {
      ParId obsTmp = p;
      obsTmp.model = "OBS";
      if(allObservations.count(obsTmp))
        continue;
      allObservations.insert(obsTmp);
      obsParameters.push_back(obsTmp);
    }

    bool result = pets::fetchDataFromMoraDB(theData, data->getMoraStream(), obsParameters, unresolvedObs, observationStartTime, lastTime);
    if (!result) {
        METLIBS_LOG_ERROR("Error in fetchDataFromMoraDB");
    }

    if (unresolvedObs.size()) {
      theData->makeParameters(unresolvedObs, true);
    }
  }

  lengthChanged = true;
  totalLength = theData->timeLineLengthInHours();
  return true;
}

void tsDrawArea::prepareDiagram()
{
  METLIBS_LOG_SCOPE();
  if (!theData) {
    METLIBS_LOG_INFO("!theData  - prepareDiagram failed");
    return;
  }

  delete diagram;
  diagram = new ptDiagram(&diaStyle, showGridLines);

  if (!diagram->attachData(theData)) {
    METLIBS_LOG_INFO("!diagram->attachData(theData) - prepareDiagram failed");
    return;
  }

  if (!diagram->makeDefaultPlotElements()) {
    METLIBS_LOG_INFO("!diagram->makeDefaultPlotElements(&bgColor) - prepareDiagram failed");
    return;
  }

  diagram->setViewport(canvas);

  // set correct weathersymbols
  PlotElement* pe = 0;
  vector<SymbolElement*> sev;
  while ((pe = diagram->findElement(SYMBOL, pe)) != 0) {
    sev.push_back((SymbolElement*) pe);
    pe = pe->next;
  }

  if (!sev.empty()) {
    const int minsymb = setup.wsymbols.minCustom(), maxsymb = setup.wsymbols.maxCustom();
    vector<std::string> symbimages;
    for (int i = minsymb; i <= maxsymb; i++) {
      miSymbol tmpSymbol = setup.wsymbols.getSymbol(i);
      std::string stmp = setup.path.images + tmpSymbol.picture();
      symbimages.push_back(stmp);
    }
    for (const auto& s : sev) {
      s->setImages(minsymb, symbimages);
    }
  }
}

void tsDrawArea::setTimemark(const miTime& nt, const std::string& name)
{
  timemarks[name] = nt;
}

void tsDrawArea::clearTimemarks(const std::string& el)
{
  if (el.empty())
    timemarks.clear();

  if (timemarks.count(el))
    timemarks.erase(el);
}

void tsDrawArea::useTimemarks()
{
  if (timemarks.empty() || !theData || !diagram)
    return;

  vector<miTime> mark(1);
  for (const auto& tm : timemarks) {
    if (PlotElement* pe = diagram->findElement(tm.first)) {
      TimemarkerElement* ptm = (TimemarkerElement*) pe;
      mark[0] = tm.second;
      ptm->setTimes(mark);
    }
  }
}

void tsDrawArea::plot(ptPainter& painter)
{
  if (!diagram)
    return;

  diagram->setProgInterval(minProg, maxProg);
  useTimemarks();
  diagram->plot(painter);
}

/// FIMEX

bool tsDrawArea::prepareFimexData()
{
  METLIBS_LOG_SCOPE();
  std::string fimexname;
  double lat,lon;
  const std::string& fimexmodel = request->getFimexModel();
  const std::string& fimexstyle = request->getFimexStyle();
  const std::string& fimexrun   = request->getFimexRun();
  if(!request->getFimexLocation(lat,lon,fimexname)) {
    METLIBS_LOG_DEBUG("Empty position - dropping interpolation");
    return false;
  }

  SessionOptions options;
  vector<ParId> inlist, outlist;
  // the style (for plot)
  diaStyle = session->getStyle(fimexstyle);

  // the style index - needed to find parameters according to our model which
  // is probably unknown in tsDiagrams
  int styleIndex = session->getStyleIndex(fimexstyle);

  //  Run=0; run is not used in the function at all!

  session->getShowOption(options, styleIndex, fimexmodel, 0);

  if (!options.numModels()) {
    METLIBS_LOG_DEBUG("Empty model list in options in the retry");
    return false;
  }

  // fetch data
  delete theData;
  theData = new ptDiagramData(setup.wsymbols);

  // precheck - is something missing in the cache?

  bool cacheIsComplete=true;
  bool hasAtLeastOneParameter=false;
  pets::FimexStream* currentStream=0;
  for (int i = 0; i < options.numModels(); i++) {
    inlist = options.distinctParamVector(i);
    if(inlist.empty())
      continue;
    hasAtLeastOneParameter=true;
    try {
      currentStream = data->getFimexStream(inlist[0].model,fimexrun);
      if (currentStream && !currentStream->hasCompleteDataset(fimexname,lat,lon,inlist)) {
        cacheIsComplete = false;
        break;
      }
    } catch (exception& e){
      currentStream=0;
    }
  }

  if(!hasAtLeastOneParameter)
    return false;

  if (!cacheIsComplete) {
    // read from file - push to the cache
    if(currentStream) {
      try {
        readFimexData(currentStream, lat, lon, fimexname, inlist, outlist,false);
      } catch (exception& e) {
        METLIBS_LOG_ERROR("Exception from readFimexData: " << e.what());
      }
    }

  }  else {

    // cache is complete - everything is read sequential

    for (int i = 0; i < options.numModels(); i++) {
      inlist = options.distinctParamVector(i);
      if(inlist.empty())
        continue;

      try {
        pets::FimexStream* currentStream = data->getFimexStream(inlist[0].model,fimexrun);
        if (readFimexData(currentStream, lat, lon, fimexname, inlist, outlist, true)) {
          const bool fetched_ok = pets::fetchDataFromFimex(theData, currentStream, lat, lon, fimexname, inlist, outlist);
          METLIBS_LOG_DEBUG(LOGVAL(fetched_ok));
        }
      } catch (exception& e) {
        METLIBS_LOG_ERROR("Exception while reading fimex data: " << e.what());
      }
    }

    // Find any missing params
    prepareMoraData(inlist);

    if (outlist.size())
      theData->makeParameters(outlist, true);
  }

  const int tmpLength = theData->timeLineLengthInHours();
  lengthChanged = (tmpLength != forecastLength);
  forecastLength = tmpLength;
  totalLength = tmpLength;

  return true;
}

bool tsDrawArea::readFimexData(pets::FimexStream* fimex, double lat, double lon, std::string stationname,
    std::vector<ParId>& inpars, std::vector<ParId>& outpars, bool sequential_read)
{
  METLIBS_LOG_SCOPE();
  inpars.erase(std::remove_if(inpars.begin(), inpars.end(), [&](const ParId& p) { return pets::FimexStream::isFiltered(p.alias); }), inpars.end());

  // we already got the data in cache - just take them and paint - no extra thread needed
  if(sequential_read)  {
    try {
      if (!fimex->readData(stationname,lat,lon,inpars,outpars)) {
        return false;
      }
    } catch(exception& e) {
      METLIBS_LOG_ERROR("FIMEX::READDATA FAILED: " << e.what());
      return false;
    }
  } else  {

    // There are missing data! Reading might take some time - we put this in another
    // thread - to avoid that the main gui thread is froxen. We cannot paint anything yet.
    // Painting will be done by forced reititeration and sequential cache read when the thread is
    // finished

    if(!datathread->isRunning()) {
      threadedLoadRequest = dataloadrequest;

      datathread->setFimexParameters(fimex,stationname,lat,lon,inpars,outpars);
      datathread->start();

      emit dataread_started();
    }
    return false;
  }

  return true;
}

void tsDrawArea::dataLoad_finished(bool /*read_success*/)
{
  emit dataread_ended();
  emit post_dataLoad(threadedLoadRequest);
}
