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
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtextbrowser.h>
#include <qmime.h> 
#include <qapplication.h> 

#include <qtHelpDialog.h>
#include <miString.h>
#include <iostream>

#include <tb_close.xpm>
#include <tb_right_arrow.xpm>
#include <tb_left_arrow.xpm>


HelpDialog::HelpDialog( QWidget* parent, const Info& hdi )
  : QDialog(parent), info(hdi), closebutton(0)
{
  miString path= info.path;
  miString src = "";
  if (info.src.size()!=0) src= info.src[0].source;
  ConstructorCernel( path, src );
}


void HelpDialog::ConstructorCernel( const miString& filepath, 
				    const miString& source){  
  setCaption(tr("Help"));

  m_font= qApp->font();

  tb = new QTextBrowser( this ); 
  
  QMimeSourceFactory::defaultFactory()->addFilePath(filepath.c_str() );
 
  tb->setSource( source.c_str());  
   
  pushbackward= new QPushButton( QPixmap(tb_left_arrow_xpm),
				 tr("Backwards"), this );
  connect(pushbackward, SIGNAL( clicked()), tb, SLOT( backward()));

  pushforward= new QPushButton( QPixmap(tb_right_arrow_xpm),
				tr("Forward"), this );
  connect(pushforward, SIGNAL( clicked()), tb, SLOT( forward())); 

  closebutton= new QPushButton( QPixmap(tb_close_xpm),
				tr("Close"), this );
  connect( closebutton, SIGNAL( clicked()), this, SLOT( hideHelp()) );

  hlayout = new QHBoxLayout( 5 );    
  hlayout->addWidget( pushbackward );
  hlayout->addWidget( pushforward );
  hlayout->addWidget( closebutton );
  hlayout->addStretch();
  
  vlayout = new QVBoxLayout( this, 5, 5 );
  vlayout->addLayout( hlayout );
  vlayout->addWidget( tb );
  
  resize( 800, 600 );
}


void HelpDialog::hideHelp(){
  hide();
}


void HelpDialog::addFilePath( const miString& filepath ){
  QMimeSourceFactory::defaultFactory()->addFilePath(filepath.c_str() );
  return;
}


void HelpDialog::setSource( const miString& source ){
  tb->setSource( source.c_str());
  tb->update();
  return;
}


void HelpDialog::showdoc(const int doc, const miString tag ){
  if (info.src.size() <= doc) return;

  setCaption(info.src[doc].name.cStr());
  setSource( info.src[doc].source );


  if(tag.exists())
    tb->scrollToAnchor( tag.cStr() );
  else
    tb->scrollToAnchor( info.src[doc].defaultlink.cStr() );
    
  show();
}


