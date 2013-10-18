/*
 * ProgressThread.cc
 *
 *  Created on: Sep 19, 2013
 *      Author: juergens
 */
#include <pets2/FimexStream.h>

#include "ProgressThread.h"

namespace pets {


void ProgressThread::exec()
{
      int lastProgress=-1;
      std::string lastMessage="";

  while(1) {
    int newprogress = pets::FimexStream::getProgress();
    if(progress >= 100) {
      progressbar->hide();
    } else {
      progressbar->show();

      if(progress!=lastProgress) {
        lastProgress=progress;
        progressbar->setValue(lastProgress);
      }

        if(lastMessage != message) {
          lastMessage = message;
          QString text = QString( lastMessage.c_str() );
          progressbar->setFormat( text );
        }
      }
    }
  }
}


} /* namespace pets */
