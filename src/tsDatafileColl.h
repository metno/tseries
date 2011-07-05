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
#ifndef _DatafileColl_h
#define _DatafileColl_h

#include <vector>
#include <map>
#include <set>
#include <sys/stat.h>

#include <puTools/miString.h>

#include <tsData/ptDataStream.h>
#include <tsData/ptParameterDefinition.h>
#include <tsData/WdbStream.h>
#include <tsData/KlimaStream.h>

#define MAXDATASETS 50
#define MAXMODELSINSTREAM 100


class dataset {
  bool bits[MAXDATASETS];
public:
  dataset() { clear(); }

  // check if dataset d exists
  bool isdata(const int& d) const {
    if (d<0) return true;
    else if (d>=MAXDATASETS) return false;
    return (bits[d]);
  }
  // set dataset d, return prior status
  bool setdata(const int& d){
    bool is = isdata(d);
    if (!is && d<MAXDATASETS) bits[d]=true;
    return is;
  }
  // return first dataset and total number of datasets
  int firstdata(int& num) const {
    int f=-1;
    num=0;
    if (empty()) return -1;
    for (int i=0; i<MAXDATASETS; i++)
      if (isdata(i)){
	num++;
	if (f<0) f=i;
      }
    return f;
  }

  bool empty() const {
    for (int i=0; i<MAXDATASETS; i++)
      if (bits[i])
	return false;
    return true;
  }
  void clear(){
    for (int i=0; i<MAXDATASETS; i++)
      bits[i] = false;
  }
};


bool Union(const dataset& d1, const dataset& d2);
bool Union(const dataset& d1, const dataset& d2, dataset& result);

struct ExtStation {
  miPosition station;
  int priority;
  dataset d;
  ExtStation(): priority(0){}
};


struct  DsInfo {
  miutil::miString streamname;    // name of stream (filename)
  miutil::miString descript;      // a short description
  DataStream *dataStream; // the datastream
  miutil::miString    sType;      // the streamtype
  int dataSet;            // dataset number to stream
  int numindset;          // number in dataset
  bool  streamOpen;       // true if stream open
  unsigned long mtime;    // modification time

  int   numModels;        // number of models in file
  Model modelList[MAXMODELSINSTREAM];// model id
  Run   runList[MAXMODELSINSTREAM];  // run id
  int   idList[MAXMODELSINSTREAM];   // model production number
  std::vector<miutil::miString> txtList[MAXMODELSINSTREAM]; // info texts
};

class DatafileColl
{
private:
  miutil::miString collectName;          // file collection name
  std::vector<DsInfo>   datastreams;    // List of datafiles
  pets::WdbStream*       wdbStream;      // the wdb data stream
  pets::KlimaStream*     klimaStream;    // the klima database from an url interface
  std::vector<ExtStation> stations;   // List of stations
  std::vector<miutil::miString> datasetname;  // name of dataset
  std::map<miutil::miString,miPosition> pos_info; // all positions ordered by name....
  int numStationsDS[MAXDATASETS];// number of positions in each dataset
  //vector<miutil::miString> priorStations;// names of prioritized stations
  float tolerance;               // 10000*degrees
  dataset customerds;            // datasets with customerinfo
  ParameterDefinition parDef;
  bool verbose;
  bool streams_opened;

  unsigned long _modtime(miutil::miString&); // get file modification time
  void _filestat(miutil::miString&, struct stat&); // get file stats
  bool _isafile(const miutil::miString&); // check if stream is a file

  void openWdbStream();
  void closeWdbStream();

  void openKlimaStream();
  void closeKlimaStream();

  bool wdbStreamIsOpen;

protected:
  bool findpos(const miutil::miString& name, int& idx);

public:
  DatafileColl();
  ~DatafileColl();

  // adds a new dataset
  int  addDataset(miutil::miString);
//   // name of station with priority=1
//   bool addPriorStation(const miutil::miString);
//   // check if name is prioritized
//   bool isPriorStation(const miutil::miString);
  // adds file to collection, return index
  int  addStream(const miutil::miString,const miutil::miString,
		 const miutil::miString,
		 const int, const int,
		 const miutil::miString= "x,x,x,x");
  // opens streams containing this model
  bool openStreams(const miutil::miString mod);
  // opens one stream
  bool openStream(const int);
  // opens all streams in collection
  bool openStreams();
  // make list of unique positions
  void makeStationList();
  // closes all streams
  void closeStreams();
  // check if newer files on disk, return indexes
  bool check(std::vector<int>&);
  // returns name, desc, filesize, dataset and n_in_dataset for file
  bool getStreamInfo(int,miutil::miString&,miutil::miString&,int&,int&,int&);
  // returns pointer to datastream object
  DataStream* getDataStream(int idx);
  // returns number of unique positions in dataset
  int getNumPositions(int dset =-1);
  // returns the miPosition for the place by name;
  miPosition getPositionInfo(miutil::miString name);
  // returns info about position idx in dset
  bool getPosition(int,int&,miutil::miString&,int&,
		   float&,float&,int&);
  // returns info about position idx in dset
  bool getPosition(int dset, int &idx, ExtStation** es);
  std::map<miutil::miString,miutil::miString> getPositions(const miutil::miString mod);
  // get list of indices for files which contain data for a
  // specific model and run. Returns number of files found
  int findModel(const Model& mid, const Run& rid, int* idx, int max);
  vector<miutil::miString> findRuns(const Model& mid);

  void setVerbose(bool v){verbose= v;}
  bool has_opened_streams() {
    bool b= streams_opened;
    streams_opened = false;
    return b;
  }

  // wdb --------------------
  bool has_wdb_stream() const { return wdbStreamIsOpen;}
  std::set<std::string> getWdbProviders();
  std::set<miutil::miTime> getWdbReferenceTimes(string provider);
  std::vector<std::string> getWdbParameterNames() const { return wdbStream->getWdbParameterNames(); }
  pets::WdbStream::BoundaryBox getWdbGeometry();
  pets::WdbStream*  getWdbStream() { return wdbStream;}
  pets::KlimaStream* getKlimaStream() { return klimaStream;}


  pets::KlimaStation getNearestKlimaStation(miCoordinates& pos) { return klimaStream->getNearestKlimaStation(pos);}


};

#endif
