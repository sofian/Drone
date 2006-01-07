/* PaletteWidget.cpp
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable
 * This file is part of Drone.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "PaletteWidget.h"
#include "PaletteManager.h"
#include "MainWindow.h"
#include <qstring.h>
#include <iostream>
#include <qsettings.h>
#include <qpoint.h>
#include <qbutton.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qpainter.h>
#include <qstyle.h>
#include <qaccel.h>
#include <qapplication.h>

extern QSettings globalSettings;

PaletteWidget::PaletteWidget(PaletteManager* pman,MainWindow * mw,QString name):
    QFrame(mw,name.latin1(),WStyle_SysMenu|Qt::WType_TopLevel|Qt::WStyle_Customize | Qt::WStyle_NoBorder | Qt::WStyle_StaysOnTop | WStyle_Tool),
    _pMan(pman),_mainWindow(mw),_name(name),_firstMove(false),_shadeMode(false),_childWidget(false)
{
  _titlebar = new ToolTitleBar( this,this);
  _titlebar->setCaption(_name);
  _titlebar->setActive(true);
  _titlebar->show();
  updateLayout();
  
  setFocusPolicy(QWidget::NoFocus);
  setFrameStyle( QFrame::StyledPanel | QFrame::Raised );
  setLineWidth( style().pixelMetric(QStyle::PM_MDIFrameWidth, this) );

  connect( _titlebar, SIGNAL( doClose() ),
           this, SLOT( close() ) );
  connect( _titlebar, SIGNAL( doubleClicked() ),
           this, SLOT( close() ) );
  QAccel *a = new QAccel( mw );
  a->connectItem( a->insertItem( ALT + Key_1 + _pMan->numPalettes() ),this , SLOT( close() ) );
  _titlebar->setCaption(_titlebar->caption() + QString(" (Alt-") + QString().sprintf("%i",_pMan->numPalettes()+1) + QString(")"));
  _titlebar->setFixedHeight(20);
  _shadedWidth=300;
  _shadedHeight = _titlebar->sizeHint().height()+(frameWidth() * 4);
  setMinimumHeight(_shadedHeight);

  
  //???addToolBar( dw, Qt::DockRight );
  //setCaption( tr( name.latin1() ) );
  //setFixedExtentHeight( 100 );


  //_container=new QWidget(this);
 // _lay = new QVBoxLayout();
  //_lay->addWidget(_titlebar);
  //  _lay->addWidget(_container);
  //  QDockWindow::setWidget(_container);
  //  _containerLayout = new QHBoxLayout();

  //setMinimumHeight(12);
  //??????setFocusPolicy(QWidget::NoFocus);

//  updateLayout();

}

void PaletteWidget::close()
{
  _shadeMode=!_shadeMode;
  if(_shadeMode)
    _windowGoalSize=QSize(_shadedWidth,_shadedHeight);
  else
  {
    _windowGoalSize=_lastUnshadedWindowSize;
    setMaximumSize(800,700);

  }

}

void PaletteWidget::doMovingGeometry()
{
  _windowSize=size();
  if(_windowSize!=_windowGoalSize)
  {

    _windowSize = QSize(0.6*(float)_windowSize.width() + 0.4 * (float)_windowGoalSize.width(),
                        0.6*(float)_windowSize.height() + 0.4 * (float)_windowGoalSize.height());
    resize(_windowSize);
  }
  else
  {
    if(_shadeMode)
      setMaximumSize(_shadedWidth,_shadedHeight);
  }

}

void PaletteWidget::setWidget(QWidget*w)
{
  //_lay->addWidget(w);
  _childWidget=w;
  w->show();
 _titlebar->raise();
  std::cerr<<"widget added"<<std::endl;
  updateLayout();
}

PaletteWidget::~PaletteWidget()
{}

bool PaletteWidget::sizeMoving()
{
  return _windowSize!=_windowGoalSize;
}

void PaletteWidget::saveGeometry()
{
  int gx = this->x();
  int gy = this->y();
  int gwidth = _lastUnshadedWindowSize.width();
  int gheight = _lastUnshadedWindowSize.height();
  //std::cerr<<"setgeo of "<<_name<<"  "<<gx<<","<<gy<<","<<gwidth<<","<<gheight<<","<<std::endl;

  globalSettings.writeEntry(QString("/drone/palettes/")+_name+QString("/xpos"),gx);
  globalSettings.writeEntry(QString("/drone/palettes/")+_name+QString("/ypos"),gy);
  globalSettings.writeEntry(QString("/drone/palettes/")+_name+QString("/width"),gwidth);
  globalSettings.writeEntry(QString("/drone/palettes/")+_name+QString("/height"),gheight);
  globalSettings.writeEntry(QString("/drone/palettes/")+_name+QString("/shaded"),(int)_shadeMode);
}

void PaletteWidget::loadGeometry()
{
	int gx;
	int gy;
	int gwidth;
	int gheight;
	
	QStringList keys = globalSettings.entryList("/drone/palettes"+_name);
	//positionning for the first load
// marche pas sur linux ?
	//if (!keys.empty()) 
	{			
      int gx = globalSettings.readEntry( QString("/drone/palettes/")+_name+QString("/xpos") ).toInt();
      int gy = globalSettings.readEntry( QString("/drone/palettes/")+_name+QString("/ypos") ).toInt();
      int gwidth = globalSettings.readEntry( QString("/drone/palettes/")+_name+QString("/width") ).toInt();
      int gheight= globalSettings.readEntry( QString("/drone/palettes/")+_name+QString("/height") ).toInt();
      _shadeMode= (bool)globalSettings.readEntry( QString("/drone/palettes/")+_name+QString("/shaded") ).toInt();
      
      std::cerr<<"loadgeo of "<<_name<<"  "<<gx<<","<<gy<<","<<gwidth<<","<<gheight<<","<<std::endl;
      _lastUnshadedWindowSize = QSize(gwidth,gheight);
      _windowGoalSize = _windowSize = QSize(_shadeMode?_shadedWidth:gwidth,_shadeMode?_shadedHeight:gheight);
      move(gx,gy);
      setMinimumWidth(100);
      resize(_windowSize);
      updateLayout();
	}

}


void PaletteWidget::updateLayout()
{
  QRect r = contentsRect();
  QRect cr;

  int th = _shadedHeight;
	
  QRect tbrect( 0, 0, width(), th );
  if ( !style().styleHint( QStyle::SH_TitleBar_NoBorder ) )
    tbrect = QRect( r.x(), r.y(), r.width(), th );
  _titlebar->setGeometry( tbrect );

  if ( style().styleHint( QStyle::SH_TitleBar_NoBorder, _titlebar ) )
    th -= r.y();
  cr = QRect( r.x(), r.y() + th,
              r.width(), r.height() - th );
  if (!_childWidget)
    return;

  //_clientWindowSize = cr.size();
  _childWidget->setGeometry( cr );
  _titlebar->raise();
}

void PaletteWidget::resizeEvent(QResizeEvent*e)
{
  if(e->spontaneous())
  {
    _windowSize=size();
    if(!_shadeMode)
      _windowGoalSize=_windowSize;
    else
      _windowGoalSize.setWidth(_windowSize.width());
  }

  if(!_shadeMode && !sizeMoving())
  	_lastUnshadedWindowSize=size();

  _pMan->regroup();
  updateLayout();


  //std::cerr<<"resize of "<<_name<<" minimized :"<<(_shadeMode?"oui":"non")<<std::endl;
  if(_firstMove)
    saveGeometry();

  QFrame::resizeEvent(e);
}

void PaletteWidget::moveEvent(QMoveEvent*e)
{
  //std::cerr<<"move of "<<_name<<" minimized :"<<(_shadeMode?"oui":"non")<<std::endl;

  if(_firstMove)
  {
    saveGeometry();
  }

  QFrame::moveEvent(e);
}

void PaletteWidget::showEvent(QShowEvent*e)
{//std::cerr<<"show of "<<_name<<std::endl;
  loadGeometry();

  _firstMove=true;
  QFrame::showEvent(e);
  updateLayout();


}

void PaletteWidget::closeEvent(QCloseEvent*e)
{
	_mainWindow->close();
}

void PaletteWidget::keyPressEvent ( QKeyEvent * e ){
std::cerr<<"keypress!"<<e->key()<<std::endl;
qApp->sendEvent(_mainWindow,e);
}
 

void PaletteWidget::keyReleaseEvent ( QKeyEvent * e ){
std::cerr<<"keyrel!"<<e->key()<<std::endl;
qApp->sendEvent(_mainWindow,e);
}