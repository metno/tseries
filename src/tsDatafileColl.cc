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
#include <tsDatafileColl.h>
#include <tsData/ptHDFFile.h>
#include <tsData/ptAsciiStream.h>
#include <set>


bool Union(const dataset& d1, const dataset& d2){
  for (int i=0; i<MAXDATASETS;i++)
    if (d1.isdata(i) && d2.isdata(i)) return true;
  return false;
}

bool Union(const dataset& d1, const dataset& d2, dataset& result){
  result.clear();
  for (int i=0; i<MAXDATASETS;i++)
    if (d1.isdata(i) && d2.isdata(i))
      result.setdata(i);

  return !result.empty();
}

DatafileColl::DatafileColl()
  : tolerance(1000.0), verbose(false), streams_opened(false) {
}

DatafileColl::~DatafileColl(){
  closeStreams();
}

int DatafileColl::addDataset(miString name){
  int n= datasetname.size();
  if (n < MAXDATASETS) {
    datasetname.push_back(name);
    numStationsDS[n] = 0;
    return n;
  }
  return -1;
}


// bool DatafileColl::addPriorStation(const miString name)
// {
//   priorStations.push_back(name);
//   return true;
// }

// bool DatafileColl::isPriorStation(const miString n)
// {
//   miString name= n.upcase();
//   int j= priorStations.size();
//   for (int i=0; i<j; i++)
//     if (name == priorStations[i].upcase())
//       return true;
//   return false;
// }

int DatafileColl::addStream(const miString name,
			    const miString desc,
			    const miString dsT,
			    const int dset,
			    const int numindset,
			    const miString sparid)
{
  int n= datastreams.size();
  if (dset<datasetname.size()) {

    DsInfo dsinfo;
    datastreams.push_back(dsinfo);

    datastreams[n].streamname = name;
    datastreams[n].descript = desc;
    datastreams[n].sType = dsT;
    datastreams[n].dataSet = dset;
    datastreams[n].numindset = numindset;
    datastreams[n].dataStream = 0;
    datastreams[n].streamOpen = false;
    datastreams[n].mtime = 0;

    vector<miString> sp= sparid.split(':');
    int m= sp.size();
    ParId parid;
    if (m>MAXMODELSINSTREAM) m= MAXMODELSINSTREAM;
    datastreams[n].numModels= m;
    for (int i=0; i<m; i++){
      parid= parDef.Str2ParId(sp[i]);
      datastreams[n].modelList[i]= parid.model;
      datastreams[n].runList[i]  = parid.run;
    }

    if (verbose) cout << "Has added stream:"
		      << datastreams[n].streamname
		      << endl;

    return n+1;
  } else { return -1; }
}



bool DatafileColl::openStreams(const miString mod)
{
  if (verbose) cout << "- Open streams with model "<< mod << endl;
  bool b=false;
  for (int i=0; i<datastreams.size(); i++) {
    if (datastreams[i].streamOpen)
      continue;
    for (int m=0; m<datastreams[i].numModels; m++){
      if ( datastreams[i].modelList[m] == mod ) {
	b |= openStream(i);
	break;
      }
    }
  }
  return b;
}

bool DatafileColl::openStream(const int idx)
{
  ErrorFlag ef=OK;

  if (idx<0 || idx>=datastreams.size()) return false;

  datastreams[idx].numModels=0;
  delete datastreams[idx].dataStream;
  datastreams[idx].dataStream=0;


  if (verbose) cout << "About to open stream:"
		    << datastreams[idx].streamname << endl;

  if(datastreams[idx].sType == "HDF") {
    datastreams[idx].dataStream =
      new HDFFile(datastreams[idx].streamname);
  } else if (datastreams[idx].sType == "ASCII") {
    datastreams[idx].dataStream =
      new AsciiStream(datastreams[idx].streamname);
  }

  if (datastreams[idx].sType != "CUSTOMER"){
    datastreams[idx].mtime = _modtime(datastreams[idx].streamname);
    if (datastreams[idx].dataStream)
      datastreams[idx].streamOpen =
	datastreams[idx].dataStream->openStream(&ef);
    if (!datastreams[idx].streamOpen) {
      // error message
      cerr << "ERROR Datafilecollection: could not open stream: "
	   << datastreams[idx].streamname << endl;
      return false;
    } else {
      // get model list from file
      int numm = 0;
      while (numm < MAXMODELSINSTREAM && datastreams[idx].dataStream->
	     getModelSeq(numm,
			 datastreams[idx].modelList[numm],
			 datastreams[idx].runList[numm],
			 datastreams[idx].idList[numm],
			 datastreams[idx].txtList[numm]))
	numm++;
      datastreams[idx].numModels=numm;
#ifdef DEBUG
      cout << "FILECOLLECTION: numModels:"<<datastreams[idx].numModels<<endl;
      for (int k=0;k<datastreams[idx].numModels;k++)
	cout << "Model:"<<datastreams[idx].modelList[k]<<
	  " Run:"<<datastreams[idx].runList[k]<<
	  " Id:"<<datastreams[idx].idList[k]<<endl;
#endif
    }
  }
  streams_opened = true;
  return true;
}


bool DatafileColl::openStreams()
{
  int i;
  bool ok = true;
  ErrorFlag ef=OK;

  if (verbose) cout << "- Open streams.."<< endl;
  for (i=0; i<datastreams.size(); i++) {
    openStream(i);
  }
  makeStationList();
  return ok;
}



void DatafileColl::closeStreams()
{
  int i;
  for (i=0; i<datastreams.size(); i++) {
    if (datastreams[i].dataStream && datastreams[i].streamOpen) {
      datastreams[i].streamOpen = false;
      delete datastreams[i].dataStream;
    }
  }
}

bool DatafileColl::_isafile(const miString& name){
  FILE *fp;
  if (fp=fopen(name.cStr(),"r")){
    fclose(fp);
    return true;
  } else return false;
}


unsigned long DatafileColl::_modtime(miString& fname)
{
  struct stat filestat;
  // first check if fname is a proper file
  if (_isafile(fname)){
    _filestat(fname, filestat);
    return (unsigned long)filestat.st_mtime;
  } else return 1;
}

void DatafileColl::_filestat(miString& fname, struct stat& filestat)
{
  stat(fname.cStr(),&filestat);
}

bool DatafileColl::check(vector<int>& idx)
{
  int i;
  unsigned long mtime;
  bool changed = false;
  idx.clear();
  if (datastreams.size() > 0) {
    for (i=0; i<datastreams.size(); i++) {
      if ( !datastreams[i].streamOpen )
	continue;
      mtime = _modtime(datastreams[i].streamname);
      if (mtime > datastreams[i].mtime) {
	changed = true;
	idx.push_back(i);
      }
    }
  }
  return changed;
}



void DatafileColl::makeStationList()
{
  int nums, i, j, k, n, posidx, ns=0, nsp;
  float lat, lng;
  bool exists;
  miPosition st;
  vector<ExtStation>::iterator p;

  if (verbose) cout << "- Reading positions from streams.."<< endl;
  stations.clear();
  pos_info.clear();
  for (i=0; i<datasetname.size(); i++) numStationsDS[i]=0;

  n= datastreams.size();
  if (n > 0) {
    for (i=0; i<n; i++) {
      if (datastreams[i].dataStream && datastreams[i].streamOpen) {
	nums = 0;
	while (datastreams[i].dataStream->getStationSeq(nums, st)) {
	  // force upcase on all stations
	  st.setName(st.Name().upcase());
	  // Check if station already exists
	  exists= findpos(st.Name(),posidx);
	  if (posidx==ns) p= stations.end();
	  else  p= stations.begin()+posidx;

	  if (!exists) {
	    ExtStation estat;
	    estat.station= st;
	    estat.priority = 2;
	    stations.insert(p,estat);
	    pos_info[st.Name()]=st;
	    p= stations.begin()+posidx;
	    ns++;
	  } else {
	    // if incoming station has a valid dbkey...keep it
	    if (st.DbKey()!=0 && p->station.DbKey()==0)
	      p->station= st;
	  }
	  // update number of stations in each dataset
	  if (!(p->d.setdata(datastreams[i].dataSet)))
	    numStationsDS[datastreams[i].dataSet]++;
	  nums++;
	} // while getStationSeq
      } // if file open
    } // for numdatastreams
  } // if numdatastreams > 0


  return;

  // Test for proximity and set priority

//   ns=  stations.size();
//   nsp= priorStations.size();
//   for (i=0; i<nsp; i++){
//     if (findpos(priorStations[i],posidx)){
//       stations[posidx].priority = 0;
//     }
//   }

//   // Set priority to the remaining stations based on proximity
//   float lngdiff, latdiff;
//   float tolerance2= 2*tolerance;
//   float tolerance3= 3*tolerance;
//   float tolerance4= 4*tolerance;
//   float tolerance5= 5*tolerance;

//   vector<int> area1,area2,area3,area4,area5;
//   bool *taken= new bool[ns];
//   for (j=0; j<ns; j++) taken[j]= false;

//   for (j=0; j<ns; j++){
//     if (!taken[j]){

//       lat  = stations[j].station.lat();
//       lng  = stations[j].station.lon();
//       taken[j]=true;
//       area1.clear();
//       area2.clear();
//       area3.clear();
//       area4.clear();
//       area5.clear();
//       for (k=0; k<ns; k++){
// 	if (!taken[k]){
// 	  latdiff = fabs(10000*(lat-stations[k].station.lat()));
// 	  lngdiff = fabs(10000*(lng-stations[k].station.lon()));
// 	  if ((latdiff<tolerance) && (lngdiff<tolerance) ) {
// 	    area1.push_back(k);
// 	    taken[k]= true;
// 	  }
// 	  else if ((latdiff<tolerance2) && (lngdiff<tolerance2) ) {
// 	    area2.push_back(k);
// 	    taken[k]= true;
// 	  }
// 	  else if ((latdiff<tolerance3) && (lngdiff<tolerance3) ) {
// 	    area3.push_back(k);
// 	    taken[k]= true;
// 	  }
// 	  else if ((latdiff<tolerance4) && (lngdiff<tolerance4) ) {
// 	    area4.push_back(k);
// 	    taken[k]= true;
// 	  }
// 	  else if ((latdiff<tolerance5) && (lngdiff<tolerance5) ) {
// 	    area5.push_back(k);
// 	    taken[k]= true;
// 	  }
// 	}
//       }
//       int i1=area1.size();
//       int i2=area2.size();
//       int i3=area3.size();
//       int i4=area4.size();
//       int i5=area5.size();
//       if (i1>2)
// 	for (k=0; k<i1; k++)
// 	  if (stations[area1[k]].priority>1)
// 	    stations[area1[k]].priority = 7;
//       if (i2>2)
// 	for (k=0; k<i2; k++)
// 	  if (stations[area2[k]].priority>1)
// 	    stations[area2[k]].priority = 6;
//       if (i3>2)
// 	for (k=0; k<i3; k++)
// 	  if (stations[area3[k]].priority>1)
// 	    stations[area3[k]].priority = 5;
//       if (i4>2)
// 	for (k=0; k<i4; k++)
// 	  if (stations[area4[k]].priority>1)
// 	    stations[area4[k]].priority = 4;
//       if (i5>2)
// 	for (k=0; k<i5; k++)
// 	  if (stations[area5[k]].priority>1)
// 	    stations[area5[k]].priority = 3;
//     }
//   }

//   delete[] taken;

}

bool DatafileColl::getStreamInfo(int idx,
				 miString& name,
				 miString& desc,
				 int& size,
				 int& dset,
				 int& nindset)
{
  struct stat fstat;
  if (idx>=0 && (idx<datastreams.size())) {
    name = datastreams[idx].streamname;
    desc = datastreams[idx].descript;
    dset = datastreams[idx].dataSet;
    nindset = datastreams[idx].numindset;
    if (_isafile(name)){
      _filestat(name,fstat);
      size = fstat.st_size;
    } else size= 50000; // ..good as any
    return true;
  } else return false;
}

DataStream* DatafileColl::getDataStream(int idx)
{
  if ((idx>=0) && (idx<datastreams.size())) {
    bool b=true;
    if ( !datastreams[idx].dataStream || !datastreams[idx].streamOpen ){
      b = openStream(idx);
    }
    if ( b ) return datastreams[idx].dataStream;
  }
  return 0;
}

// return number of positions in dataset dset.
// if dset < 0, return total number of positions in collection.
int DatafileColl::getNumPositions(int dset)
{
  if (dset<0)
    return stations.size();
  else if (dset<datasetname.size())
    return numStationsDS[dset];
  else return 0;
}
miPosition DatafileColl::getPositionInfo(miString name)
{
  if(pos_info.count(name)) return pos_info[name];
  miPosition empty;
  return empty;
}


bool DatafileColl::getPosition(int dset, int &idx,
			       miString& name, int &id,
			       float& lat, float& lng,
			       int &prio)
{
  int n= stations.size();
  if ((idx <0) || (idx>=n)) return false;

  if (dset>=0){
    if (dset>=datasetname.size()) return false;
    while (!(stations[idx].d.isdata(dset))
	   && (idx<n)) idx++;
    if (!(stations[idx].d.isdata(dset))) return false;
  }

  name = stations[idx].station.Name();
  id = idx;
  lat = stations[idx].station.lat();
  lng = stations[idx].station.lon();
  prio = stations[idx].priority;
  idx++;
  return true;
}

bool DatafileColl::getPosition(int dset, int &idx, ExtStation** es)
{
  int n= stations.size();
  if ((idx <0) || (idx>=n)) return false;

  if (dset>=0){
    if (dset>=datasetname.size())
      return false;

    while (!(stations[idx].d.isdata(dset)) && (idx<n))
      idx++;
    if (idx>=n)
      return false;
  }

  *es = &(stations[idx]);
  idx++;
  return true;
}

map<miString,miString> DatafileColl::getPositions(const miString mod)
{
  map<miString,miString> result;

  dataset ds;

  int i,j,n= datastreams.size();

  for (i=0; i<n; i++)
    for (j=0; j<datastreams[i].numModels;j++)
      if ( mod == datastreams[i].modelList[j] )
	ds.setdata(datastreams[i].dataSet);

  n= stations.size();
  for( i=0 ; i<n ; i++ ) {
    if (Union(ds,stations[i].d)){
      result[stations[i].station.Name()] =
	miString(stations[i].station.lat()) + ":" +
	miString(stations[i].station.lon());
    }
  }
  return result;
}


// Get list of indices for files which contain data for a
// specific model and run (returned in idx).
// Returnvalue is number of files found
int DatafileColl::findModel(const Model& mid,
			    const Run& rid,
			    int* idx, int max)
{
  int numi=0;
  int n= datastreams.size();
  for (int i=0; i<n; i++){
    for (int j=0; j<datastreams[i].numModels;j++){
      if ((mid == datastreams[i].modelList[j] || mid == M_UNDEF) &&
	  (rid == datastreams[i].runList[j] || rid == R_UNDEF ||
	   datastreams[i].runList[j]==R_UNDEF)){
	if (numi<max){
	  idx[numi++]= i;
	}
      }
    }
  }
  return numi;
}

vector<miString> DatafileColl::findRuns(const Model& mid)
{
  Run rid;
  vector<miString> vrid;
  set<int> srid;
  int n= datastreams.size();
  for (int i=0; i<n; i++)
    for (int j=0; j<datastreams[i].numModels;j++) {
      if (mid == datastreams[i].modelList[j] ) {
	rid = datastreams[i].runList[j];
	srid.insert(rid);
      }
    }
  set<int>::iterator itr = srid.begin();
  for(;itr !=srid.end();itr++)
    vrid.push_back(miString(*itr));

  return vrid;
}


// Binary search for position by name
bool DatafileColl::findpos(const miString& name, int& idx)
{
  miString pname;
  int n= stations.size(), min=0, max=n-1, p;

  while (!(max<min)){
    p= (min+max)/2;
    pname= stations[p].station.Name();
    if (name == pname){
      idx= p;
      return true;
    }
    if (name < pname)
      max= p-1;
    else
      min= p+1;
  }

  // pos not found, increase index by one for frontal insertion
  idx= max+1;
  return false;
}
