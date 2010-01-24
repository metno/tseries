
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
#include <iostream>
#include <qtsMain.h>
//Added by qt3to4:
#include <QTimerEvent>
#include <QPixmap>
#include <Q3PopupMenu>
#include <QCloseEvent>
#include <config.h>

#include <qfileinfo.h>
#include <qfontdialog.h>
#include <qUtilities/QLetterCommands.h>
#include <puTools/ttycols.h>
#include <qstringlist.h>

#include "tseries.xpm"

const miString thisTM  = "MARKEDTIME";
const miString dianaTM = "DIANATIME";

qtsMain::qtsMain(miString l) : lang(l), Q3MainWindow(0)

{
// Added to avoid unnessecary updates when connected to diana
  // and diana is in automatic update mode
  currentTime = miTime::nowTime();

  latlond=false;
  makeMenuBar();
  dianaconnected = false;

  work = new qtsWork(this);
  setCentralWidget(work);

  makeConnectButtons();

  work->latlonInDecimalToggled(latlond);

  printer = new QPrinter();

  setIcon(QPixmap(tseries_xpm));

  initHelp();

  restoreLog();
  setRemoteParameters();
  makeAccelerators();
  setTimemark(currentTime);

  // milliseconds
  int updatetimeout =  ( 1000 * 60 ) * 2;

  updateTimer= startTimer(updatetimeout);

}



void qtsMain::makeMenuBar()
{
  makeFileMenu();
  makeSettingsMenu();
  makeHelpMenu();
}


void qtsMain::makeHelpMenu()
{
  menu_help  = new Q3PopupMenu( this );
  menuBar()->insertItem( tr("Help"), menu_help);

  menu_help->insertItem(tr("Manual"), this, SLOT(showHelp()),Qt::Key_F1);
  menu_help->insertItem( tr("About.."), this, SLOT(about()));
}


void qtsMain::makeFileMenu()
{
  menu_file = new Q3PopupMenu( this );
  menuBar()->insertItem( tr("File"), menu_file);

  menu_file->insertItem( tr("Print"), this, SLOT(print()), Qt::CTRL+Qt::Key_P);
  menu_file->insertItem( tr("Save Image"),this,SLOT(raster()));
  menu_file->insertSeparator();
  menu_file->insertItem( tr("Change filter"),this,SLOT(manageFilter()));
  menu_file->insertSeparator();
  menu_file->insertItem( tr("Quit"), this, SLOT(quit()), Qt::CTRL+Qt::Key_Q);
}


void qtsMain::makeSettingsMenu()
{
  menu_setting  = new Q3PopupMenu( this );
  menuBar()->insertItem( tr("Preferences"), menu_setting);
  menu_setting->insertItem( tr("Reset Preferences"), this, SLOT(readLog()));
  menu_setting->insertItem( tr("Save Preferences"), this, SLOT(writeLog()));
  menu_setting->insertSeparator();
  sOnQuit = menu_setting->insertItem( tr("Save at exit"), this, SLOT(setSaveOnQuit()));
  menu_setting->insertSeparator();
  idsnormal = menu_setting->insertItem( tr("Show positions (DIANA)"),
					this, SLOT(toggleNormalNames()));
  idsselect = menu_setting->insertItem( tr("Show active position (DIANA)"),
					 this, SLOT(toggleSelectedNames()));

  idsicon = menu_setting->insertItem( tr("Show icons (DIANA)"),
				      this, SLOT(toggleIcon()));

  idsposition = menu_setting->insertItem( tr("Send positions (DIANA)"),
					  this, SLOT(togglePositions()));

  menu_setting->insertSeparator();

  idtmark  = menu_setting->insertItem( tr("Show timemark"), this, SLOT(toggleTimemark()));
  idLatLon = menu_setting->insertItem( tr("Lat/Lon in decimal"), this, SLOT(toggleLatLon()));
  idtmark =  menu_setting->insertItem( tr("Show timemark"), this, SLOT(toggleTimemark()));

  menu_setting->insertSeparator();
  menu_setting->insertItem(tr("Font"),this,SLOT(chooseFont()));
  menu_setting->insertSeparator();

  menu_lang= new Q3PopupMenu(this);


  findLanguages();
  menu_lang->setCheckable(true);

  menu_setting->insertItem(tr("Languages"),menu_lang);

  bool soq;

  sicon     = true;
  sposition = true; //no logging
  config.get("TIMEMARK",   tmark);
  config.get("SHOWNORMAL", snormal);
  config.get("SHOWSELECT", sselect);
  config.get("SHOWICON",   sicon);
  config.get("SAVEONQUIT", soq);
  config.get("LATLONDEC",  latlond);

  menu_setting->setItemChecked(sOnQuit,soq);
  menu_setting->setItemChecked(idsnormal,snormal);
  menu_setting->setItemChecked(idsselect,sselect);
  menu_setting->setItemChecked(idsicon,sicon);
  menu_setting->setItemChecked(idsposition,sposition);
  menu_setting->setItemChecked(idtmark,tmark);
  menu_setting->setItemChecked(idLatLon,latlond);


}


void qtsMain::makeConnectButtons()
{

  connect(work,SIGNAL(refreshStations()),this,SLOT(refreshDianaStations()));

  pluginB = work->sideBar()->pluginButton();
  targetB = work->sideBar()->targetButton();

  connect(targetB,SIGNAL(pressed()),this,SLOT(sendTarget()));
  connect(targetB,SIGNAL(released()),this,SLOT(clearTarget()));

  connect(pluginB, SIGNAL(receivedMessage(miMessage&)),
	  SLOT(processLetter(miMessage&)));
  connect(pluginB, SIGNAL(addressListChanged()),
	  SLOT(processConnect()));
  connect(pluginB, SIGNAL(connectionClosed()),
	  SLOT(cleanConnection()));
}


void qtsMain::makeAccelerators()
{

  Q3Accel *a = new Q3Accel( this );
  a->connectItem( a->insertItem(Qt::Key_Up+Qt::CTRL),
		  work->sideBar(),
		  SLOT(nextModel()) );
  a->connectItem( a->insertItem(Qt::Key_Down+Qt::CTRL),
		  work->sideBar(),
		  SLOT(prevModel()));

}

void qtsMain::closeEvent ( QCloseEvent * e)
{
  quit();
}


void qtsMain::quit()
{
  bool soq;
  config.get("SAVEONQUIT",soq);

  if(soq)
    writeLog();
  qApp->quit();
}

void qtsMain::raster()
{
  miString format = "PNG";
  miString fname  = "/" + work->file("png");

  static QString fpath = ".";

  QString s = Q3FileDialog::getSaveFileName( fpath + fname.cStr(),
					   "Pictures (*.png *.xpm *.bmp *.eps);;All (*.*)",
					    this, "save file dialog",tr("Save Image") );

  if (s.isNull())
    return;

  QFileInfo finfo(s);
  fpath= finfo.dirPath(TRUE);

  fname  = s.latin1();

  int quality= -1;

  cerr << "Saving: " << fname << endl;

  if (fname.contains(".xpm") || fname.contains(".XPM"))
    format= "XPM";
  else if (fname.contains(".bmp") || fname.contains(".BMP"))
    format= "BMP";
  else if (fname.contains(".eps") || fname.contains(".epsf")) {
    makeEPS(fname);
    return;
  }

  QImage img= work->Show()->grabFrameBuffer(true);
  img.save(fname.cStr(), format.cStr(), quality );
}


void qtsMain::print()
{
  miString command ;

#ifdef linux
  command= "lpr -h -r -{hash}{numcopies} -P {printer} {filename}";
#else
  command= "lp -c -n{numcopies} -d {printer} {filename}";
#endif

  printOptions priop;

  miString fname = work->file("ps");

  QString ofn = printer->outputFileName();


  if(ofn.isNull()) {
    QFileInfo p(fname.cStr());
    printer->setOutputFileName(p.absFilePath());
  }
  else {
    QFileInfo p(ofn);
    printer->setOutputFileName(p.dirPath(TRUE)+"/"+fname.cStr());
  }

  printer->setOutputToFile(false);

  if (printer->setup(this)){

    if (printer->outputToFile())
      priop.fname= printer->outputFileName().latin1();
    else if (command.substr(0,4)=="lpr ")
      priop.fname= miTime::nowTime().format("TS%d%H%M%S.ps");
    else
      priop.fname= fname;


    // fill printOption from qprinter-selections
    fillPrintOption(printer, priop);

    // set printername
    if (!printer->outputToFile())
      priop.printer= printer->printerName().latin1();

    // start the postscript production
    QApplication::setOverrideCursor( Qt::waitCursor );
    work->Show()->hardcopy(priop);

    // if output to printer: call appropriate command
    if (!printer->outputToFile()){
      priop.numcopies= printer->numCopies();

      // expand command-variables
      pman.expandCommand(command, priop);

      cerr<<"PRINT: "<< command << endl;

      system(command.c_str());
    }
    QApplication::restoreOverrideCursor();

    // reset number of copies (saves a lot of paper)
    printer->setNumCopies(1);
  }
}


void qtsMain::makeEPS(const miString& filename)
{
  QApplication::setOverrideCursor( Qt::waitCursor );
  printOptions priop;
  priop.fname= filename;
  priop.colop= d_print::incolour;
  priop.orientation= d_print::ori_automatic;
  priop.pagesize= d_print::A4;
  priop.numcopies= 1;
  priop.usecustomsize= false;
  priop.fittopage= false;
  priop.drawbackground= true;
  priop.doEPS= true;

  work->Show()->hardcopy(priop);

  QApplication::restoreOverrideCursor();
}






void qtsMain::about()
{

  QMessageBox::about( this, tr("About T-series"),
		      tr("T-series: Times series viewer\nVersion: %1\n\nmet.no 2002")
		      .arg(VERSION));
}


void qtsMain::writeLog()
{
  tsSetup setup;

  config.set("SIZEX", this->width());
  config.set("SIZEY", this->height());
  config.set("POSX",this->x());
  config.set("POSY",this->y());

  config.set("SHOWNORMAL",snormal);
  config.set("SHOWSELECT",sselect);
  config.set("SHOWICON",sicon);
  config.set("TIMEMARK",tmark);
  config.set("LATLONDEC",latlond);
  config.set("FONT",miString(qApp->font().toString().latin1()));

  if(lang.exists())
    config.set("LANG",lang);



  work->collectLog();

  config.save(setup.files.configure);
}


void qtsMain::restoreLog()
{
  int sx,sy,px,py;
  miString f;

  if( config.get("SIZEY",sy) && config.get("SIZEX",sx))
    this->resize(sx,sy);


  if( config.get("POSX",px) &&  config.get("POSY",py))
    this->move(px,py);

  if(config.get("FONT",f)) {
    QFont font;
    if (font.fromString(f.cStr()))
      qApp->setFont(font, true);
  }

  work->restoreLog();
}


void qtsMain::readLog()
{
  tsSetup     setup;
  config.read(setup.files.configure);
  restoreLog();
}

void qtsMain::setSaveOnQuit()
{
  bool soq;
  config.get("SAVEONQUIT",soq);
  config.set("SAVEONQUIT",!soq);

  menu_setting->setItemChecked(sOnQuit,!soq);
}





void qtsMain::toggleNormalNames()
{
  snormal = !snormal;
  menu_setting->setItemChecked(idsnormal,snormal);
  sendNamePolicy();
}

void qtsMain::toggleSelectedNames()
{
  sselect = !sselect;
  menu_setting->setItemChecked(idsselect,sselect);
  sendNamePolicy();
}

void qtsMain::toggleIcon()
{
  sicon = !sicon;
  menu_setting->setItemChecked(idsicon,sicon);
  sendNamePolicy();
}

void qtsMain::togglePositions()
{
  sposition = !sposition;
  menu_setting->setItemChecked(idsposition,sposition);
  if(sposition)
    refreshDianaStations();
  miMessage m;
  if(sposition)
    m.command= qmstrings::showpositions;
  else
    m.command= qmstrings::hidepositions;
  m.description= DATASET_TSERIES + work->lastList();
  pluginB->sendMessage(m);
}

void qtsMain::toggleTimemark()
{
  tmark = !tmark;
  menu_setting->setItemChecked(idtmark,tmark);
  setTimemark(miTime::nowTime());
}

void qtsMain::toggleLatLon()
{
  latlond = !latlond;
  menu_setting->setItemChecked(idLatLon,latlond);
  if(work)
    work->latlonInDecimalToggled(latlond);
}



void qtsMain::setTimemark(miTime mark)
{
  if(work) {
    if(tmark)
      work->Show()->setTimemark(mark,thisTM);
    else
      work->Show()->clearTimemarks(thisTM);
  }
}

// DIANA

void qtsMain::setDianaTimemark(miTime mark)
{
  if(work){
    if(dianaconnected)
      work->Show()->setTimemark(mark,dianaTM);
    else
      work->Show()->clearTimemarks(dianaTM);
  }
}

// send one image to diana (with name)
void qtsMain::sendImage(const miString name, const QImage& image)
{
  if (!dianaconnected) return;
  if (image.isNull()) return;

  QByteArray *a=new QByteArray();
  QDataStream s(a, QIODevice::WriteOnly);
  s << image;

  miMessage m;
  m.command= qmstrings::addimage;
  m.description= "name:image";

  ostringstream ost;
  ost << name << ":";

  int n= a->count();
  for (int i=0; i<n; i++) {
    ost << setw(7) << int((*a).data()[i]);
  }
  miString txt= ost.str();
  m.data.push_back(txt);

  pluginB->sendMessage(m);
}


void qtsMain::refreshDianaStations()
{
  if(!dianaconnected || !sposition)
    return;

  if(currentModel == work->lastList())
    return;

  miString prevModel = currentModel;
  currentModel = work->lastList();

  if(!sendModels.count(currentModel) )
    sendNewPoslist();

  sendNamePolicy();
  enableCurrentPoslist();
  disablePoslist(prevModel);

}


void qtsMain::disablePoslist(miString prev)
{
  if(prev == NOMODEL_TSERIES)
    return;

  miMessage m;
  m.command= qmstrings::hidepositions;
  m.description= DATASET_TSERIES + prev;
  pluginB->sendMessage(m);
}

void qtsMain::enableCurrentPoslist()
{
  miMessage m;
  m.command= qmstrings::showpositions;
  m.description= DATASET_TSERIES + currentModel;
  pluginB->sendMessage(m);
}

void qtsMain::sendNewPoslist()
{
  sendModels.insert(currentModel);
  miMessage m  = work->getStationList();

  m.common+=( snormal ? "true" : "false");
  m.common+=( sselect ? ":true" : ":false");

  pluginB->sendMessage(m);
}

void qtsMain::sendTarget()
{
  if(!dianaconnected)
    return;

  miMessage m,m2;

  m = work->target();
  pluginB->sendMessage(m);

  m2.command= qmstrings::showpositions;
  m2.description= TARGETS_TSERIES;
  pluginB->sendMessage(m2);
}

void qtsMain::clearTarget()
{
  if(!dianaconnected)
    return;

  miMessage m;
  m.command= qmstrings::hidepositions;
  m.description= TARGETS_TSERIES;
  pluginB->sendMessage(m);
}


// called when client-list changes

void qtsMain::processConnect()
{
  tsSetup s;

  if(pluginB->clientTypeExist(s.server.client)){
    dianaconnected= true;

    cout << ttc::color(ttc::Blue)
	 << "< CONNECTING TO: " << s.server.client << " > "
	 << ttc::reset << endl;

    QImage sImage(s.files.std_image.cStr());
    QImage fImage(s.files.fin_image.cStr());
    QImage iImage(s.files.icon_image.cStr());

    sendImage(IMG_STD_TSERIES,sImage);
    sendImage(IMG_FIN_TSERIES,fImage);
    sendImage(IMG_ICON_TSERIES,iImage);

    sendNamePolicy();
    refreshDianaStations();
  }
  else
    dianaconnected= false;


  setRemoteParameters();

}

void qtsMain::setRemoteParameters()
{
  targetB->setEnabled(dianaconnected);
  menu_setting->setItemEnabled(idsnormal, dianaconnected);
  menu_setting->setItemEnabled(idsselect, dianaconnected);
  menu_setting->setItemEnabled(idsicon, dianaconnected);

  if(!dianaconnected) {
    currentModel = NOMODEL_TSERIES;
    sendModels.clear();
  }
}



void qtsMain::sendNamePolicy()
{
  if(!dianaconnected)
    return;

  miMessage m;
  m.command = qmstrings::showpositionname;
  m.description="normal:selected:icon";

  m.data.push_back( snormal ? "true" : "false");
  m.data[0]+= (sselect ? ":true" : ":false");
  m.data[0]+= (sicon ? ":true" : ":false");
  pluginB->sendMessage(m);

}


// processes incoming miMessages

void qtsMain::processLetter(miMessage& letter)
{
#ifdef DEBUG
  cerr <<"Command: "<<letter.command<<"  ";
  cerr <<endl;
  cerr <<" Description: "<<letter.description<<"  ";
  cerr <<endl;
  cerr <<" commonDesc: "<<letter.commondesc<<"  ";
  cerr <<endl;
  cerr <<" Common: "<<letter.common<<"  ";
  cerr <<endl;
  for (int i=0;i<letter.data.size();i++)
    if (letter.data[i].length()<80)
      cerr <<" data["<<i<<"]:"<<letter.data[i]<<endl;;
  cerr <<" From: "<<letter.from<<endl;
  cerr <<"To: "<<letter.to<<endl;
  
#endif
  if(letter.command == qmstrings::removeclient) {
    tsSetup s;
    if( letter.common.contains(s.server.client.cStr()))
      cleanConnection();
  }

  if (letter.command == qmstrings::selectposition ) {
    if ( letter.data.size())
      work->changeStation(letter.data[0]);
  }
  else if(letter.command == qmstrings::timechanged ) {
    // Added to avoid invalid updates from diana when diana
    // is in automatic update mode
    miTime timeFromDiana = miTime(letter.common );
    if (timeFromDiana != currentTime) {
      currentTime = timeFromDiana;
      setDianaTimemark( currentTime );
    }
  }

}

void qtsMain::showHelp()
{
  helpdialog->showdoc(0,"");
}

void qtsMain::initHelp()
{
  tsSetup s;

  miString helpfile = s.path.doc + "/" + lang + "_" + s.doc.mainSource;
  miString newsfile = s.path.doc + "/" + lang + "_" + s.doc.newsSource;

  ifstream testhelpfile(helpfile.cStr());
  ifstream testnewsfile(newsfile.cStr());

  if(testhelpfile) {
    helpfile = lang + "_" + s.doc.mainSource;
    testhelpfile.close();
  } else
    helpfile = s.doc.mainSource;

  if(testnewsfile) {
    newsfile = lang + "_" + s.doc.newsSource;
    testnewsfile.close();
  } else
    newsfile = s.doc.newsSource;


  HelpDialog::Info info;
  HelpDialog::Info::Source helpsource;
  info.path              = s.path.doc;
  helpsource.source      = helpfile;
  helpsource.name        = tr("Manual").latin1();
  helpsource.defaultlink = s.doc.mainLink;
  info.src.push_back(helpsource);
  helpsource.source      = newsfile;
  helpsource.name        = tr("News").latin1();
  helpsource.defaultlink = s.doc.newsLink;
  info.src.push_back(helpsource);

  helpdialog= new HelpDialog(this, info);
  helpdialog->hide();

}





void qtsMain::timerEvent(QTimerEvent* e)
{

  if (e->timerId()==updateTimer)
    if(work) {
      work->updateStreams();
      setTimemark(miTime::nowTime());
    }


}


void qtsMain::cleanConnection()
{
  dianaconnected = false;
  cout << ttc::color(ttc::Red)
       << "< DISCONNECTING >"
       << ttc::reset << endl;

  setRemoteParameters();
  setDianaTimemark(miTime::nowTime());
}



void qtsMain::manageFilter()
{

  set <miString> p  = work->fullPosList();
  set <miString> f  = work->Filter();
  set <miString> o  = work->createFilter(true);

  qtsFilterManager * fm = new qtsFilterManager(p,f,o,this);

  if (fm->exec()) {
    // clean out the old filters from diana
    if(dianaconnected ) {
      set<miString>::iterator itr = sendModels.begin();
      for(;itr!=sendModels.end();itr++)
	if(itr->contains(TS_MINE)){
	  sendModels.erase(*itr);
	  itr--;
	}
      if(currentModel.contains(TS_MINE))
	currentModel=NOMODEL_TSERIES;
    }
    // new filter
    work->newFilter(fm->result());

  }


}

void qtsMain::chooseFont()
{
  bool ok;
  QFont font = QFontDialog::getFont( &ok,qApp->font(),this );
  if ( ok ) {
    // font is set to the font the user selected
    qApp->setFont(font, true);
  } else {
    // the user cancelled the dialog; font is set to the initial
    // value: do nothing
  }
}

void qtsMain::findLanguages()
{
  QDir d;
  tsSetup setup;
  miString dname = ( setup.path.lang.empty() ? "./" : setup.path.lang[0]);

  d.setPath(dname.cStr() );
  QStringList f=d.entryList("tseries_??.qm");


  for ( QStringList::Iterator it = f.begin(); it != f.end(); ++it ) {
    QString s = *it;
    s.replace("tseries_","");
    s.replace(".qm","");

    int  id = menu_lang->insertItem( s,this,SLOT(toggleLang(int)));

    langID[id] = s.latin1();

    menu_lang->setItemChecked(id, (s.latin1() == lang)  );

  }

  int id = menu_lang->insertItem("en",this,SLOT(toggleLang( int)));
  menu_lang->setItemChecked(id, (!lang.exists() || lang=="en") );
  langID[id]="en";

}


void qtsMain::toggleLang(int id)
{
  lang=langID[id];
  map<int,miString>::iterator itr=langID.begin();
  for(;itr!=langID.end();itr++)
    menu_lang->setItemChecked(itr->first, ( itr->second == lang ) );



  switch(QMessageBox::warning( this, tr("Language Changed"),
			       tr("T-series must be restarted to reset the Language to: [%1] ")
			       .arg(lang.cStr()),
			       tr("Ok"),
			       tr("Quit T-series"),
			       0,0,1) ) {
  case 0:
    break;
  case 1:
    quit();
  }
}
