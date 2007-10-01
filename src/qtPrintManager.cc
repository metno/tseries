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


#include <qtPrintManager.h>


// conversion from Qt-printing types to diana-types
d_print::Orientation getOrientation(QPrinter::Orientation ori)
{
  d_print::Orientation dori= d_print::ori_automatic;

  if (ori==QPrinter::Landscape)
    dori= d_print::ori_landscape;
  else if (ori==QPrinter::Portrait)
    dori= d_print::ori_portrait;

  return dori;
}

d_print::ColourOption getColourMode(QPrinter::ColorMode cm)
{
  d_print::ColourOption dcol= d_print::incolour;

  if (cm==QPrinter::Color)
    dcol= d_print::incolour;
  else if (cm==QPrinter::GrayScale){
    dcol= d_print::greyscale;
  }

  return dcol;
}

d_print::PageSize getPageSize(QPrinter::PageSize ps)
{
  d_print::PageSize dps= d_print::A4;

  if (ps==QPrinter::A4){
    dps= d_print::A4;
  } else if (ps==QPrinter::A3){
    dps= d_print::A3;
  } else if (ps==QPrinter::A2){
    dps= d_print::A2;
  } else if (ps==QPrinter::A1){
    dps= d_print::A1;
  } else if (ps==QPrinter::A0){
    dps= d_print::A0;
  }

  return dps;
}


// fill printOptions from QPrinter-selections
void fillPrintOption(const QPrinter* qp, printOptions& priop)
{
  if (!qp) return;

  priop.colop= getColourMode(qp->colorMode());
  if (priop.colop==d_print::greyscale) priop.drawbackground= false;

  priop.orientation= getOrientation(qp->orientation());
  
  priop.pagesize= getPageSize(qp->pageSize());
}

