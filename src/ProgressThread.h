
#ifndef PROGRESSTHREAD_H_
#define PROGRESSTHREAD_H_



  /*
   $Id$

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

#include <QThread>
#include <QProgressBar>
#include <string>


namespace pets {

class ProgressThread  : public QThread {
  QProgressBar* progress;

public:
    ProgressThread(QProgressBar* p){progess=p;}

    int exec();
    void run() { exec(); }

};

} /* namespace pets */
#endif /* PROGRESSCACHE_H_ */
