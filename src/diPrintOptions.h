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
#ifndef _diPrintOptions_h
#define _diPrintOptions_h

#include <puTools/miString.h>
#include <map>

namespace d_print {

  // postscript options

  enum Orientation { // paper orientation
    ori_automatic,
    ori_portrait,
    ori_landscape
  };

  enum ColourOption { // use of colour
    incolour,
    greyscale,
    blackwhite
  };

  enum PageSize { // standard pagesizes
    A4, B5, Letter, Legal, Executive,
    A0, A1, A2, A3, A5, A6, A7, A8, A9,
    B0, B1, B10, B2, B3, B4, B6, B7, B8, B9,
    C5E, Comm10E, DLE, Folio, Ledger, Tabloid, NPageSize
  };

  struct PaperSize { // size of paper in mm (default A4)
    float hsize;
    float vsize;
    PaperSize(): hsize(210),vsize(297) {}
    PaperSize(float h, float v)
      : hsize(h), vsize(v) {}
  };

}; // end of namespace

class printOptions {
public:
  miutil::miString fname;                   // name of output file
  miutil::miString printer;                 // name of printer
  d_print::Orientation orientation; // paper-orientation
  d_print::ColourOption colop;      // use of colour
  d_print::PageSize pagesize;       // pagesize in standard notation
  d_print::PaperSize papersize;     // size of paper in mm
  int numcopies;                    // number of copies
  bool usecustomsize;               // use papersize instead of pagesize
  bool fittopage;                   // fit output to page
  bool drawbackground;              // fill with background colour
  bool doEPS;                       // make encapsulated postscript
  int viewport_x0;                  // OpenGL viewport coordinates
  int viewport_y0;                  //
  int viewport_width;               //
  int viewport_height;              //

  printOptions() :
    orientation(d_print::ori_automatic),
    colop(d_print::incolour),
    pagesize(d_print::A4),
    numcopies(1),
    usecustomsize(false),
    fittopage(true),
    drawbackground(true),
    doEPS(false),
    viewport_x0(0),viewport_y0(0),
    viewport_width(0),viewport_height(0)
  {}
};

class printerManager {
private:
  struct printerExtra { // extra commands for postscript
    std::map<miutil::miString,miutil::miString> keys;// keys for matching..
    std::map<std::string,std::string> commands;// Extra output-commands
  };
  static std::vector<printerExtra> printers;
  static std::map<miutil::miString,d_print::PageSize> pages;
  static std::map<d_print::PageSize,d_print::PaperSize> pagesizes;
  static miutil::miString pcommand; // printercommand
public:
  printerManager();

  bool readPrinterInfo(const miutil::miString fname); // parse printer-info file
  d_print::PageSize  getPage(const miutil::miString s); // page from string
  d_print::PaperSize getSize(const d_print::PageSize ps);// size from page
  // check if special commands exist for this setup
  bool checkSpecial(const printOptions& po, std::map<std::string,std::string>& mc);
  // expand variables in print-command
  bool expandCommand(miutil::miString& com, const printOptions& po);

  void setPrintCommand(const miutil::miString pc){pcommand= pc; }
  miutil::miString printCommand() const {return pcommand; }
};

#endif
