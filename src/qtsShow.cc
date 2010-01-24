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
#include <qtsShow.h>
#include <iostream>


const float gl_width =  1500.0;
const float gl_height = 1000.0;

qtsShow::qtsShow(QWidget* parent, const QGLFormat fmt,
		 tsRequest* tsr,
		 DatafileColl* tsd,
		 SessionManager* ses)
  : QGLWidget(fmt,parent), plotw(1), ploth(1), drawArea(tsr,tsd,ses), initialised(false)
{
}


void qtsShow::paintGL()
{
  static int paintw=0, painth=0;
  if ( paintw != plotw || painth != ploth ){
    glViewport( 0, 0, plotw, ploth );
    paintw= plotw;
    painth= ploth;
  }

  drawArea.prepare(false);
  drawArea.plot();
  swapBuffers();

}

//  Set up the OpenGL rendering state
void qtsShow::initializeGL()
{
  glClearColor( 1.0,1.0,1.0,1.0 ); // Let OpenGL clear to white
  glShadeModel( GL_FLAT );

  setAutoBufferSwap(false);
  glDrawBuffer(GL_BACK);

  glLoadIdentity();
  glOrtho(0,gl_width,0,gl_height,-1,1);
  initialised = true;
}


//  Set up the OpenGL view port, matrix mode, etc.
void qtsShow::resizeGL( int w, int h )
{
  glViewport( 0, 0, (GLint)w, (GLint)h );
  plotw= w;
  ploth= h;

  float pw = 1;
  float ph = 1;
  if ( plotw!=0 && ploth!=0 ){
    pw = gl_width/float(plotw);
    ph = gl_height/float(ploth);
  }

  drawArea.setViewport(w,h,pw,ph);
}

void qtsShow::refresh(bool readData)
{
  if (!initialised) return;
  makeCurrent(); // set current OpenGL context
  drawArea.prepare(readData);
  updateGL();
}


void qtsShow::hardcopy(const printOptions& p)
{
  makeCurrent(); // set current OpenGL context
  drawArea.prepare();

  drawArea.startHardcopy(p);
  updateGL();
  drawArea.endHardcopy();
}

void qtsShow::setTimemark(miTime tim, miString nam)
{
  drawArea.setTimemark(tim,nam);
  refresh(false);
}

void qtsShow::clearTimemarks(miString nam)
{
  drawArea.clearTimemarks(nam);
  refresh(false);
}



