/* VideoOutputX11Base.h
 * Copyright (C) 2004 Mathieu Guindon
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

#ifndef VIDEOOUTPUTX11BASE_INCLUDED
#define VIDEOOUTPUTX11BASE_INCLUDED

#include "VideoOutput.h"

namespace X11
{

extern "C"{
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/xf86vmode.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <inttypes.h>
#include <X11/extensions/Xinerama.h>
#define MWM_HINTS_DECORATIONS (1L << 1)
typedef struct
{
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long          inputMode;
    unsigned long status;
} PropMotifWmHints;
#define PROP_MOTIF_WM_HINTS_ELEMENTS 5
}



// Clean up the mess caused by the X headers
/* #undef Bool       */
/* #undef Above      */
/* #undef Below      */
/* #undef KeyPress   */
/* #undef KeyRelease */
/* #undef FocusOut   */
/* #undef FocusIn    */
//#undef None

struct XVisualInfo_fixed
{
  Visual *visual;
  VisualID visualid;
  int screen;
  unsigned int depth;
  int class_fixed;
  unsigned long red_mask;
  unsigned long green_mask;
  unsigned long blue_mask;
  int colormap_size;
  int bits_per_rgb;
};

class VideoOutputX11Base : public VideoOutput
{
public:
  VideoOutputX11Base();
  virtual ~VideoOutputX11Base();

  bool toggleFullscreen(bool fs, int xRes, int yRes, int xPos, int yPos);

protected:
  
  bool openXDisplay();
  bool checkXShmExtension();
  bool createXWindow(int xRes, int yRes);
  void destroyXWindow();
  bool mapWindow();
  bool unMapWindow();
  XShmSegmentInfo *createShmSegment(int size);//assign segment to _shmInfo and return the segment info
  void waitForShmCompletion();
  void destroyShm();
  bool createGC();
  void processX11Events();
  virtual void onResize(int , int ){}
  void resizeWindow(int sizeX, int sizeY);

  bool findHighestDepthVisual(XVisualInfo &visualInfo);

  void* _display;//! handle on the X11 display
  Window _window;//! handle on the X11 window
  XF86VidModeModeInfo _desktopMode;//! The Desktop videomode
  XVisualInfo _visualInfo;

  XShmSegmentInfo _shmInfo;//! the current used shm segment
  GC _gc;
  int _currentWindowWidth, _currentWindowHeight;

};

}

#endif
