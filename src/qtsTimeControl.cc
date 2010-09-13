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

#include "qtsTimeControl.h"

TimeControl::TimeControl(QWidget* parent)
  : QFrame( parent)
{
  QGridLayout* timeLayout = new QGridLayout(this);
  setFrameStyle(QFrame::Panel | QFrame::Sunken);


  QLabel  *lab1 = new QLabel(tr("Start:"),this);
  QLabel  *lab2 = new QLabel(tr("Hours:"),this);
  
  startSlider   = new QSlider( Qt::Horizontal,this);
  stopSlider    = new QSlider( Qt::Horizontal,this);
  
  startSlider->setTickPosition(QSlider::TicksBelow);
  startSlider->setTickInterval(24);
  startSlider->setRange(0,300);
  startSlider->setValue(0);
  stopSlider->setTickPosition(QSlider::TicksBelow);
  stopSlider->setTickInterval(24);
  stopSlider->setRange(0,300);
  stopSlider->setValue(300);

  startLabel    = new QLabel("0",this);
  stopLabel     = new QLabel("300",this);


  connect( startSlider, SIGNAL( valueChanged(int)),SLOT(startchanged(int)));
  connect( stopSlider, SIGNAL(  valueChanged(int)),SLOT(stopchanged(int)));



  timeLayout->addWidget(lab1,0,0);
  timeLayout->addWidget(lab2,1,0);
  
  timeLayout->addWidget ( startSlider,0,1,1,3 );
  timeLayout->addWidget ( stopSlider ,1,1,1,3 );
  
  timeLayout->addWidget(startLabel,0,4);
  timeLayout->addWidget(stopLabel, 1,4);
   
}

void TimeControl::startchanged(int v)
{
  startLabel->setNum(v);
  minmaxSlot();
}

void TimeControl::stopchanged(int v)
{
  stopLabel->setNum(v);
  minmaxSlot(); 
}





void TimeControl::minmaxSlot()
{
  int istart, istop;    
  istart= startSlider->value();
  istop=  stopSlider->value(); 
  
  istop+=istart;
  
  emit   minmaxProg(istart,istop);
}
