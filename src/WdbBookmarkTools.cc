#include "WdbBookmarkTools.h"
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "directory.xpm"
#include "media-record.xpm"
#include "metno.xpm"
#include "trashcan.xpm"

#include <QStringList>


using namespace std; 

WdbBookmarkTools::WdbBookmarkTools()
{
  maxRecords=30;
  record=0;
  folders.clear();
  directoryIcon.addPixmap(QPixmap(directory_xpm));
  recordIcon.addPixmap(QPixmap(media_record_xpm));
  metnoIcon.addPixmap(QPixmap(metno_xpm));
  trashIcon.addPixmap(QPixmap(trashcan_xpm));


}

bool WdbBookmarkTools::read(std::string filename,bool ignoreFromSave)
{
  ifstream in(filename.c_str());
  if(!in ) {
    cerr << "Bookmarkfile " << filename << " does not exist - skipping " << endl;
    return false;
  }
  while(in) {
    string line;
    getline(in,line);
    int c = line.find_first_of("#",0);
    if(0 <= c &&  c < line.length() ) {
      int k=line.length() -  c;
      line.erase(c,k);
    }
    boost::algorithm::trim(line);
    if(!line.empty())
      addLine(line,ignoreFromSave);
  }
  return true;
}

void WdbBookmarkTools::addLine(string line,bool ignoreFromSave, bool reverse)
{
  vector<string> words;
  boost::split( words, line, boost::algorithm::is_any_of("|") );
  if(words.size() < 2 ) return;
  string data  = words[1];
  string token = words[0];
  string folder;

  boost::split( words,token, boost::algorithm::is_any_of(".") );

  QStandardItem *parentItem = model->invisibleRootItem();

  int size = words.size();
  int last = size-1;


  for (int col = 0; col < size; ++col) {
    // this is the item
    if(col == last) {
      int r=(reverse ? 0 : parentItem->rowCount());
      QStandardItem *childItem  = new QStandardItem(words[col].c_str());
      childItem->setData(data.c_str());
      childItem->setDropEnabled(false);
      parentItem->insertRow(r,childItem);

    } else {
      // old folder found - use it as the parent
      folder+=( col ? "." : "") + words[col];
      if(folders.count(folder)) {
	parentItem = model->itemFromIndex ( folders[folder] );

      } else {
        // create a new folder
        QStandardItem *childItem = createFolder(words[col],ignoreFromSave);

        parentItem->appendRow(childItem);
        parentItem      = childItem;
        folders[folder] = childItem->index();
        if(ignoreFromSave){
          ignores.insert(folder);
        }
      }
    }
  }
}

QStandardItem * WdbBookmarkTools::createFolder(string folder,bool ignoreFromSave)
{
  QStandardItem *childItem = new QStandardItem(folder.c_str());

  if(folder=="RECORD") {
     childItem->setIcon(recordIcon);
   } else if ( folder =="TRASH") {
       childItem->setIcon(trashIcon);
   } else {

     if(ignoreFromSave)
       childItem->setIcon(metnoIcon);
     else
       childItem->setIcon(directoryIcon);
   }

  if(ignoreFromSave){
    childItem->setEditable(false);
  }
  childItem->setDragEnabled(false);


  return childItem;
}



void WdbBookmarkTools::addFolder(string folder,bool ignoreFromSave)
{
  if(folders.count(folder))
    return;

  QStandardItem *parentItem = model->invisibleRootItem();
  QStandardItem *childItem  = createFolder(folder,ignoreFromSave);

  parentItem->appendRow(childItem);
  folders[folder] = childItem->index();

  if(ignoreFromSave) {
    ignores.insert(folder);
  }
}


void WdbBookmarkTools::write(string filename)
{
  ofstream out(filename.c_str());
  if(!out) {
    cerr << "Unable to write stationlist to " << filename << endl;
    return;
  }


  QStandardItem *parentItem = model->invisibleRootItem();
  if(!parentItem->hasChildren()) return;

  for(int i=0;i<parentItem->rowCount();i++) {
    QStandardItem* item = parentItem->child(i);
    if(!item->hasChildren()) continue;
    string dirname= item->text().toStdString();
    if(ignores.count(dirname)) continue;

    for(int i=0;i<item->rowCount();i++) {
      QStandardItem* child = item->child(i);
      QVariant var         = child->data();
      QString  coor        = var.toString();
      QString  name        = child->text();

      if(name.isEmpty() || coor.isEmpty()) continue;
      out << dirname << "." << name.toStdString() << "|" << coor.toStdString() << endl;
    }
  }

}



std::string WdbBookmarkTools::createRecordName(float f,char pos, char neg)
{
  float fdeg = f;
  int   deg = int(fdeg);
  fdeg-=deg;
  fdeg*=60;
  int min = int(fdeg);

  ostringstream ost;
  ost << abs(deg) << "° " << abs(min) << "\' " << ( deg >=0 ? pos : neg );
  return ost.str();
}



void WdbBookmarkTools::addRecord(float lon,float lat,std::string name)
{

  ostringstream ost;
  ost << "RECORD.";
   if(name.empty()) {
     ost << createRecordName(lon,'E','W') << " " << createRecordName(lat,'N','S');
   } else
    ost << name;


  ost << "|" << lat << ":" << lon;
  addLine(ost.str(),false,true);
  cutRecord();
}


void WdbBookmarkTools::cutRecord()
{
  if(!folders.count("RECORD")      ) return;

  QStandardItem* item = model->itemFromIndex (folders ["RECORD"]);

  if(!item                          ) return;
  if(!item->hasChildren()           ) return;
  if( item->rowCount() < maxRecords ) return;

  item->setRowCount(maxRecords);

}


