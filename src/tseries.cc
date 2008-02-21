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
#include <qtsMain.h>
#include <qapplication.h>
#include <fstream>
#include <tsSetup.h>
#include <tsConfigure.h>
#include <puTools/miCommandLine.h> 
#include <qtranslator.h>
#include <puTools/miTime.h>

using namespace std;


main(int argc, char **argv)
{
  vector<miCommandLine::option> o;
  o.push_back(miCommandLine::option( 's',"setup"    , 1 ));
  o.push_back(miCommandLine::option( 'h',"help"     , 0 ));
  o.push_back(miCommandLine::option( 'f',"file"     , 1 ));
  o.push_back(miCommandLine::option( 'S',"site"     , 1 ));
  o.push_back(miCommandLine::option( 'l',"lang"     , 1 ));

  miCommandLine cl(o,argc, argv);

   // read setup

  miString    site="FOU";
  miString    setupfile="tseries.ctl";
  tsSetup     setup;
  tsConfigure config;
  miString    lang;

  if(cl.hasFlag('h')){
    cerr << "Usage: " << argv[0] << " [-s setupfile] [ -S site ] -l [lang] " << endl;  
    exit (0);
  }
  
  if(cl.hasFlag('S')) 
    site =  cl.arg('S')[0];
  
  if(cl.hasFlag('s')) 
    setupfile= cl.arg('s')[0];

  if(!setup.read(setupfile,site))
    exit(0);

  if(setup.lang.exists())
    lang=setup.lang;

  config.read(setup.files.configure);
  
  config.get("LANG",lang);

  if(cl.hasFlag('l')) 
    lang=cl.arg('l')[0];
 
  
  QApplication a( argc, argv );
  QTranslator myapp( 0 );
  QTranslator qt( 0 );


  miTime defTime;
  defTime.setDefaultLanguage(lang);

  if(setup.gui.style.exists())
    a.setStyle(setup.gui.style.cStr());
  

  if(lang.exists()) {

    miString qtlang   = "qt_"     +lang;
    miString langfile = "tseries_"+lang;

    // translation file for application strings

    for(int i=0;i<setup.path.lang.size(); i++ )
      if( qt.load( qtlang.cStr(),setup.path.lang[i].cStr()))
	break; 

    for(int i=0;i<setup.path.lang.size(); i++ ) 
      if( myapp.load( langfile.cStr(),setup.path.lang[i].cStr()))
	break;
    
    a.installTranslator( &qt    );
    a.installTranslator( &myapp );
  }


  qtsMain *main = new qtsMain(lang); 
  a.setMainWidget(main);
  main->setCaption( "T-series" );
  main->show();
  return a.exec();
}
