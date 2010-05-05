#include "WdbBookmarkTools.h"
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "directory.xpm"
#include "media-record.xpm"

#include <QStringList>


using namespace std; 



std::string  WdbBookmarkItem::toFile()
{
  string res;
 
  if(isLocal) {
    res= text().toStdString();
    QStandardItem *item=this;
    while(item->parent()) {
    
      res=item->text().toStdString() + res;
      item=item->parent();
    }
  }
  return res;
}


WdbBookmarkFiles::WdbBookmarkFiles()
{
  folders.clear();
  directoryIcon.addPixmap(QPixmap(directory_xpm));
  recordIcon.addPixmap(QPixmap(media_record_xpm));
}

bool WdbBookmarkFiles::read(std::string filename) 
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
    cout << " adding line " << line << endl;
    boost::algorithm::trim(line);
    if(!line.empty())
      addLine(line);
  }
  return true;
}

void WdbBookmarkFiles::addLine(string line)
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
      cerr << "found item " << words[col] << endl;
      int r=parentItem->rowCount();
      WdbBookmarkItem *childItem  = new WdbBookmarkItem(words[col].c_str());
      //childItem->setIcon(bookmarkIcon);
      childItem->setData(data.c_str());
      parentItem->insertRow(r,childItem);

    } else {
      folder+=( col ? "." : "") + words[col];
      if(folders.count(folder)) {
	parentItem = model->itemFromIndex ( folders[folder] );

      } else {
        WdbBookmarkItem *childItem = new WdbBookmarkItem(words[col].c_str());
	cout << "new folder " << words[col] << endl;
        if(folder=="RECORD") {
	  childItem->setIcon(recordIcon);
	} else
	  childItem->setIcon(directoryIcon);
	parentItem->appendRow(childItem);
	parentItem      = childItem;
	folders[folder] = childItem->index();
      }
    }
  }  
  
}

