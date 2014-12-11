#include <FetchStations.h>
#include <iostream>
#include <sstream>
#include <QDomDocument>



FetchStations::FetchStations(QString baseurl) : base_url(baseurl)
{
 manager = new QNetworkAccessManager(this);
 connect(manager, SIGNAL(finished(QNetworkReply*)),
         this, SLOT(replyFinished(QNetworkReply*)));
}



void FetchStations::getData(QString name)
{
  QString url = base_url+name+"*";


 manager->get(QNetworkRequest(QUrl(url)));

}


void FetchStations::replyFinished(QNetworkReply* reply)
{
  if(reply->error()) 
    return;
      
  QByteArray data = reply->readAll();
  QDomDocument document;
  QString errorMsg;
  int errorLine, errorColumn;
  std::vector<std::string> result;
  
  if(document.setContent ( data, true, &errorMsg, &errorLine, &errorColumn) ) {
   
    QDomNodeList nodes = document.elementsByTagName("doc");
    
    for(int i = 0; i < nodes.count(); i++)
    {
      QDomNode element = nodes.at(i);
      if(element.isElement())
        {
	  std::string name="";
	  float latitude=-99999,longitude=-99999;


	  QDomNode entry = element.toElement().firstChild();

	  while(!entry.isNull()) {
	    QString value  = entry.toElement().text();
	    QString key    = entry.toElement().attributeNode("name").value();
	    
	    if(key=="name") 
	      name = value.toStdString();
	    else if(key=="lat")
	      latitude=value.toFloat();
	    else if(key=="long")
	      longitude = value.toFloat();
	   
	  
	    if(longitude > -99999 && latitude > -99999 && !name.empty()) {
	      std::ostringstream ost;
	      ost  << name << "|" << latitude << ":" << longitude;
	      result.push_back(ost.str());
	      break;
	    }
	    entry = entry.nextSibling();
	  }
        }
    }

    emit searchResult(result);
  }
}
