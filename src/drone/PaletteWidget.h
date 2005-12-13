/* PaletteWidget.h
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

#ifndef PaletteWidget_INCLUDED
#define PaletteWidget_INCLUDED

#include "qdockwindow.h"
#include <qstring.h>
#include <qbutton.h>
#include <qpixmap.h>
#include <qsize.h>
#include <qlayout.h>
#include "ToolTitleBar.h"

class MainWindow;
class PaletteManager;

class PaletteWidget: public QFrame
{
Q_OBJECT
  public:
  PaletteWidget(PaletteManager*,MainWindow *mw,QString name);
  virtual ~PaletteWidget();

  void setWidget(QWidget*);
  void doMovingGeometry();
  QString name(){return _name;}
public slots:
  void close();
protected:
  virtual void resizeEvent(QResizeEvent*);
  virtual void moveEvent(QMoveEvent*);
  virtual void showEvent(QShowEvent*);
  virtual void closeEvent(QCloseEvent*);
  virtual void keyPressEvent ( QKeyEvent * e );
  virtual void keyReleaseEvent ( QKeyEvent * e );

  bool sizeMoving();
  void updateLayout();
  void loadGeometry();
  void saveGeometry();

  PaletteManager* _pMan;
  MainWindow* _mainWindow;
  QString _name; 
  bool _firstMove;
  QVBoxLayout *_lay;
  bool _shadeMode;
  ToolTitleBar *_titlebar;
  QWidget* _childWidget;
  int _shadedHeight,_shadedWidth;

  QSize _windowSize;
  QSize _windowGoalSize;
  QSize _lastUnshadedWindowSize;

};

#endif
