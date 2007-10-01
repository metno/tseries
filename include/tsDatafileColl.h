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
#include <sys/stat.h>

#include <miString.h>

#include <ptDataStream.h>
#include <ptParameterDefinition.h>

#define MAXDATASETS 16
#define MAXMODELSINSTREAM 100


using namespace std;



class dataset {
  unsigned int bits;
  static unsigned int bmask[MAXDATASETS];
public:
  dataset() { clear(); }

  // check if dataset d exists
  bool isdata(const int& d) const {
    if (d<0) return true;
    else if (d>=MAXDATASETS) return false;
    return (bits & bmask[d]);
  }
  // set dataset d, return prior status
  bool setdata(const int& d){
    bool is = isdata(d);
    if (!is && d<MAXDATASETS) bits |= bmask[d];
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

  bool empty() const { return (bits==0);      }
  void clear()       { bits = (unsigned int)0;}

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
  miString streamname;    // name of stream (filename)
  miString descript;      // a short description
  DataStream *dataStream; // the datastream
  miString    sType;      // the streamtype
  int dataSet;            // dataset number to stream
  int numindset;          // number in dataset
  bool  streamOpen;       // true if stream open
  unsigned long mtime;    // modification time

  int   numModels;        // number of models in file
  Model modelList[MAXMODELSINSTREAM];// model id
  Run   runList[MAXMODELSINSTREAM];  // run id
  int   idList[MAXMODELSINSTREAM];   // model production number
  vector<miString> txtList[MAXMODELSINSTREAM]; // info texts
};

class DatafileColl
{
private:
  miString collectName;          // file collection name
  vector<DsInfo> datastreams;    // List of datafiles
  vector<ExtStation> stations;   // List of stations
  vector<miString> datasetname;  // name of dataset
  int numStationsDS[MAXDATASETS];// number of positions in each dataset
  vector<miString> priorStations;// ..name of these
  float tolerance;               // 10000*degrees
  dataset customerds;            // datasets with customerinfo
  ParameterDefinition parDef;
  bool verbose;
  
  unsigned long _modtime(miString&); // get file modification time
  void _filestat(miString&, struct stat&); // get file stats
  bool _isafile(const miString&); // check if stream is a file
protected:
  bool findpos(const miString& name, int& idx);

public:
  DatafileColl();
  ~DatafileColl();

  // adds a new dataset
  int  addDataset(miString);
  // adds a name to a dataset
  bool addNameToDataset(int dset, miString name);
  // adds a new station
  bool addStation(ExtStation&);
  // name of station with priority=1
  bool addPriorStation(const miString);
  // check if name is prioritized
  bool isPriorStation(const miString);
  // adds file to collection, return index
  int  addStream(const miString,const miString,
		 const miString,
		 const int, const int,
		 const miString= "x,x,x,x");
  // opens one stream
  bool openStream(const int);
  // opens all streams in collection
  bool openStreams();
  // make list of unique positions
  void makeStationList();
  // closes all streams
  void closeStreams();
  // check if newer files on disk, return indexes
  bool check(vector<int>&);
  // returns name and number of streams in collection
  void getCollInfo(int&,miString&);
  // returns name, desc, filesize, dataset and n_in_dataset for file
  bool getStreamInfo(int,miString&,miString&,int&,int&,int&);
  // returns pointer to datastream object
  DataStream* getDataStream(int idx);
  // returns number of unique positions in dataset
  int getNumPositions(int dset =-1);
  // returns info about position idx in dset
  bool getPosition(int,int&,miString&,int&,
		   float&,float&,int&);
  // returns info about position idx in dset
  bool getPosition(int dset, int &idx, ExtStation** es);
  // returns next station with Union(dataset,dset)
  bool getPosition(const dataset& dset, int& idx, ExtStation** es);
  // get list of indices for files which contain data for a
  // specific model and run. Returns number of files found
  map<miString,miString> getPositions(const miString mod);
  int findModel(const Model& mid, const Run& rid, int* idx, int max);
  bool findStation(const miString& name, const Model& mid,const Run& rid);
  bool findStation(const miString& name, float& lat, float& lng);
  bool findStation(const miString& name, miPosition& pos);
  vector<miString> findRuns(const Model& mid);


  // get number of datasets with data for this posindex
  int getPosNumSets(int idx);
  void getDatasets(vector<miString>&);
  void getCustomers(dataset& ds){ds = customerds; }

  void setVerbose(bool v){verbose= v;}
};

#endif





