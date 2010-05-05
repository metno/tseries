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

#include <QStringList>


using namespace std; 

WdbBookmarkTools::WdbBookmarkTools()
{
  record=0;
  folders.clear();
  directoryIcon.addPixmap(QPixmap(directory_xpm));
  recordIcon.addPixmap(QPixmap(media_record_xpm));
  metnoIcon.addPixmap(QPixmap(metno_xpm));
}

bool WdbBookmarkTools::read(std::string filename,bool addToSave)
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
      addLine(line,addToSave);
  }
  return true;
}

void WdbBookmarkTools::addLine(string line,bool markToSave)
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
    if(col==last) {
      int r=parentItem->rowCount();
      QStandardItem *childItem  = new QStandardItem(words[col].c_str());
      childItem->setData(data.c_str());
      parentItem->insertRow(r,childItem);

    } else {
      folder+=( col ? "." : "") + words[col];
      if(folders.count(folder)) {
	parentItem = model->itemFromIndex ( folders[folder] );

      } else {
        QStandardItem *childItem = new QStandardItem(words[col].c_str());

        if(markToSave)
          childItem->setIcon(directoryIcon);
        else
          childItem->setIcon(metnoIcon);

	parentItem->appendRow(childItem);
	parentItem      = childItem;
	folders[folder] = childItem->index();
	if(markToSave)
	    saves.insert(folder);
      }
    }
  }  
  
}

void WdbBookmarkTools::addFolder(string folder,bool markToSave)
{
  QStandardItem *parentItem = model->invisibleRootItem();

  if(folders.count(folder))
    return;

  QStandardItem *childItem = new QStandardItem(folder.c_str());
  if(folder=="RECORD") {
    childItem->setIcon(recordIcon);
  } else {

    if(markToSave)
      childItem->setIcon(directoryIcon);
    else
      childItem->setIcon(metnoIcon);
  }
  parentItem->appendRow(childItem);
  folders[folder] = childItem->index();
  if(markToSave)
    saves.insert(folder);

}


void WdbBookmarkTools::write(string filename)
{
  ofstream out(filename.c_str());
  if(!out) {
    cerr << "Unable to write stationlist to " << filename << endl;
    return;
  }
  set<string>::iterator itr=saves.begin();
  for(;itr!=saves.end();itr++) {
    QList<QStandardItem*> ilist = model->findItems (itr->c_str());

    if(ilist.empty()) continue;

    QStandardItem* item = ilist.at(0);

    string dirname=item->text().toStdString();
    if(!item->hasChildren()) continue;
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


void WdbBookmarkTools::addRecord(float lon,float lat)
{
  ostringstream ost;

  ost << "RECORD." << ++record << "|" << lat << ":" << lon;
  addLine(ost.str(),false);

}




