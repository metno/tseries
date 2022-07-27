/*
  Tseries - A Free Meteorological Timeseries Viewer

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

#include "qtsSidebar.h"

#include "tsSetup.h"

#include <coserver/ClientSelection.h>

#include <QPixmap>
#include <QHBoxLayout>
#include <QRegExp>
#include <QVBoxLayout>

#include "ts_find.xpm"
#include "ts_filter.xpm"
#include "view-refresh.xpm"
#include "list-add.xpm"
#include "synop.xpm"
#include "media-record.xpm"
#include "expand.xpm"
#include "collapse.xpm"

#include <iostream>

using namespace miutil;
using namespace std;

qtsSidebar::qtsSidebar(QString language)
: QWidget()
{
  fimexRexordToggled = false;

  tabs       = new QTabWidget(this);

  fimextab = new FimexTab(this, language);
  connect(fimextab,SIGNAL(changestyle( const QString&)), this, SIGNAL(changeFimexStyle(const QString& )));
  connect(fimextab,SIGNAL(changemodel( const QString&)), this, SIGNAL(changeFimexModel(const QString& )));
  connect(fimextab,SIGNAL(changerun(   const QString&)), this, SIGNAL(changeFimexRun(  const QString& )));
  connect(fimextab,SIGNAL(changeCoordinates(float, float,QString)), this, SIGNAL(changeFimexCoordinates(float,float,QString)));
  connect(fimextab,SIGNAL(changePoslist()), this, SIGNAL(changeFimexPoslist()));
  connect(fimextab,SIGNAL(newPoslist()), this, SIGNAL(newFimexPoslist()));

  progressHeader = new QLabel(this);
  progressHeader->hide();

  progress = new QProgressBar(this);
  progress->setRange(0,100);
  progress->setTextVisible(true);
  QFont progressfont=progress->font();
  progressfont.setPointSize(progressfont.pointSize()-3);
  progress->setFont(progressfont);

  connect(tabs,SIGNAL(currentChanged(int)), this,SLOT(tabChanged(int)));

  // Control the start and length

  timecontrol= new TimeControl(this);
  connect(timecontrol,SIGNAL(minmaxProg(int,int)),this, SIGNAL(minmaxProg(int,int)));


  obsInfo = new QLabel(this);
  obsInfo->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  obsInfo->hide();
  QFont obsfont=obsInfo->font();
  obsfont.setPointSize(obsfont.pointSize()-3);
  obsInfo->setFont(obsfont);


  // connectbuttons are hosted here... but are used and
  // connected in qtsMain!

  QPixmap synop_pix(synop_xpm);
  QPixmap find_pix(ts_find_xpm);
  QPixmap filter_pix(ts_filter_xpm);
  QPixmap refresh_pix(view_refresh_xpm);
  QPixmap add_pix(list_add_xpm);
  QPixmap record_pix(media_record_xpm);
  QPixmap expand_pix(expand_xpm);
  QPixmap collapse_pix(collapse_xpm);

  pluginB = new ClientSelection("TSeries", this);
  tsSetup s;
  pluginB->client()->setServerCommand(QString::fromStdString(s.server.command));
  pluginB->setClientName(QString::fromStdString(s.server.name));

  observationB = new QPushButton(synop_pix,"",this);
  observationB->setMaximumWidth(synop_pix.width());
  observationB->setCheckable(true);
  observationB->setToolTip(  tr("enable/disable observations") );

  connect(observationB,SIGNAL(toggled(bool)), this, SIGNAL(observationToggled(bool)));

  targetB = new QPushButton(find_pix,"",this);
  targetB->setMaximumWidth(find_pix.width());
  targetB->setToolTip(tr("Show position (DIANA)") );

  addFimexBookmarkButton =  new QPushButton(add_pix, "",this);
  connect(addFimexBookmarkButton,SIGNAL(clicked()),fimextab, SLOT(addBookmarkFolder()));

  recordFimexButton =  new QPushButton(record_pix, "",this);
  recordFimexButton->setCheckable(true);
  connect(recordFimexButton,SIGNAL(toggled(bool)),fimextab, SLOT(recordToggled(bool)));
  connect(recordFimexButton,SIGNAL(toggled(bool)),this, SLOT(recordToggled(bool)));

  expandFimexButton   = new QPushButton(expand_pix, "",this);
  expandFimexButton->setToolTip(  tr("expand all") );
  collapseFimexButton = new QPushButton(collapse_pix, "",this);
  collapseFimexButton->setToolTip(  tr("collapse all") );

  connect(expandFimexButton,SIGNAL(clicked()),fimextab, SLOT(expandAll()));
  connect(collapseFimexButton,SIGNAL(clicked()),fimextab, SLOT(collapseAll()));


  // LAYOUT ---------------------------

  QVBoxLayout * vlayout = new QVBoxLayout(this);


  vlayout->addWidget(tabs);

  vlayout->addWidget(timecontrol);
  vlayout->addWidget(progressHeader);
  vlayout->addWidget(progress);
  vlayout->addWidget(obsInfo);

  // Buttons -------------------

  connectStatus = new QLabel(this);
  connectStatus->setMinimumSize(50,32);

  QHBoxLayout * blayout = new QHBoxLayout();
  blayout->addWidget(addFimexBookmarkButton);
  blayout->addWidget(recordFimexButton);

  blayout->addWidget(collapseFimexButton);
  blayout->addWidget(expandFimexButton);
  blayout->addWidget(connectStatus);
  blayout->addStretch(2);
  blayout->addWidget(observationB);
  blayout->addWidget(targetB);
  QToolButton* clientbutton = new QToolButton(this);
  clientbutton->setDefaultAction(pluginB->getToolButtonAction());
  blayout->addWidget(clientbutton);
  vlayout->addLayout(blayout);

  progress->hide();
}

void qtsSidebar::recordToggled(bool record)
{
  fimexRexordToggled = record;
  if(record)
    setObsInfo("<b><font color=red>Recording Positions</font></b>");
  else
    setObsInfo("");
}


void qtsSidebar::setTab(int tabIdx)
{
  if(tabIdx < tabs->count() && tabIdx >= 0 ) {
    tabs->setCurrentIndex(tabIdx);
  }
}

void qtsSidebar::setCoordinates(float lon, float lat)
{
  fimextab->setCoordinates(lon, lat);
}

void qtsSidebar::newTimeRange(int total,int fcast)
{
  timecontrol->setTimeRange(total,fcast);
}

void qtsSidebar::setObsInfo(QString s)
{
  obsInfo->setText(s);
  if(s.isEmpty())
    obsInfo->hide();
  else
    obsInfo->show();
}

QString qtsSidebar::fillList(const vector<std::string>& v, const StationTab_lEntry l)
{
  QStringList qlist;
  for(unsigned int i=0;i<v.size();i++) {
    qlist << QString::fromStdString(v[i]);
  }

  switch (l) {
  case CMFIMEXSTYLE:
    return fimextab->setStyles(qlist);
  case CMFIMEXMODEL:
    fimextab->setModels(qlist);
    break;
  case CMFIMEXRUN:
    qSort(qlist.begin(), qlist.end(), qGreater<QString>());
    fimextab->setRuns(qlist);
    break;
  }
  return QString("");
}

void qtsSidebar::tabChanged(int /*idx*/)
{
  addFimexBookmarkButton->show();
  recordFimexButton->show();
  collapseFimexButton->show();
  expandFimexButton->show();
  targetB->show();
  recordToggled(fimexRexordToggled);
}
bool qtsSidebar::restoreFimexFromLog(std::string mod, std::string sty, std::string expanded)
{
  fimextab->setStyle(QString::fromStdString(sty));
  fimextab->setModel(QString::fromStdString(mod));
  fimextab->setExpandedDirs(expanded);
  return true;
}

void qtsSidebar::writeBookmarks()
{
  fimextab->writeBookmarks();
}

void qtsSidebar::setProgress(int progr, std::string text)
{
  QString txt = QString::fromStdString(text);

  progressHeader->setText(QString("<b>Leser Data for:  %1</b>").arg(txt.section(':',0,0)));
  progressHeader->show();

  progress->show();
  progress->setValue(progr);

  progress->setFormat(txt.section(':',1,1));
}

void qtsSidebar::endProgress()
{
  progress->reset();
  progress->hide();
  progressHeader->hide();
}
