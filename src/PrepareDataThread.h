/*
   Copyright (C) 2006 met.no

   Contact information:
   Norwegian Meteorological Institute
   Box 43 Blindern
   0313 OSLO
   NORWAY
   email: diana@met.no

   This file is part of generated by met.no

   This is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Tseries; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef PREPAREDATATHREAD_H_
#define PREPAREDATATHREAD_H_

#include <tsData/ptParameterDefinition.h>

#include <QThread>

#include <string>
#include <vector>

namespace pets {

class FimexStream;

class PrepareDataThread  : public QThread {
  Q_OBJECT

private:
  pets::FimexStream* fimex;
  double latitude;
  double longitude;
  std::string stationname;
  std::vector<ParId> inpar;
  std::vector<ParId> outpar;

public:
  PrepareDataThread(QObject* parent=0) :  QThread (parent){}
  void setFimexParameters(pets::FimexStream* f, const std::string& pl, double lat, double lon, std::vector<ParId>& ip, std::vector<ParId>& op);
  void run() override;

Q_SIGNALS:
  void post_dataLoad(bool);
};

} /* namespace pets */

#endif /* PREPAREDATATHREAD_H_ */
