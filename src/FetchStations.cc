#include <FetchStations.h>
#include <iostream>
#include <sstream>
#include <QDomDocument>



FetchStations::FetchStations(QString baseurl, QString lang) : base_url(baseurl)
{
  manager = new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply*)),
      this, SLOT(replyFinished(QNetworkReply*)));

  feature_class_name="feature_class";

  if(lang=="no" || lang=="nn")
    feature_class_name = QString("feature_class_%1").arg(lang);

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
        std::ostringstream feature;

        while(!entry.isNull()) {
          QString value  = entry.toElement().text();
          QString key    = entry.toElement().attributeNode("name").value();

          if(key=="name")
            name = value.toStdString();
          else if(key=="lat")
            latitude=value.toFloat();
          else if(key=="long")
            longitude = value.toFloat();
          else if(key==feature_class_name) {
            feature << " ( " <<  value.toStdString() << " )";
          }
          std::cerr << key.toStdString() << " : " << value.toStdString() << std::endl;

          entry = entry.nextSibling();
        }

        if(longitude > -99999 && latitude > -99999 && !name.empty()) {
          std::ostringstream ost;
          ost  << name << feature.str() << "|" << latitude << ":" << longitude;
          result.push_back(ost.str());
        }
      }
    }

  }

  emit searchResult(result);
}

