/* Gear_VideoLoop.cpp
 * Copyright (C) 2004 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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

#include "Gear_VideoLoop.h"
#include "Engine.h"
#include "CircularBuffer.h"

#include <iostream>

#include "GearMaker.h"


extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_VideoLoop(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "VideoLoop";
  gearInfo.classification = GearClassifications::video().time().instance();
  return gearInfo;
}
}

Gear_VideoLoop::Gear_VideoLoop(Schema *schema, std::string uniqueName) : Gear(schema, "VideoLoop", uniqueName)
{
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN", true));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT", true));
  addPlug(_PUNCH_IN = new PlugIn<ValueType>(this, "PunchIn", false, new ValueType(0, 0, 1)));
  addPlug(_PUNCH_OUT = new PlugIn<ValueType>(this, "PunchOut", false, new ValueType(0, 0, 1)));
  addPlug(_MEMORY = new PlugIn<ValueType>(this, "Memory", false, new ValueType(125, 0, 125)));

  EnumType *playbackMode = new EnumType(N_PLAYBACK_MODE, FORWARD);
  playbackMode->setLabel(FORWARD,"Foward");
  playbackMode->setLabel(BACKWARD,"Backward");
  playbackMode->setLabel(PING_PONG,"Ping pong");
  addPlug(_MODE_IN = new PlugIn<EnumType>(this, "Mode", playbackMode));

  _circbuf = new CircularBuffer<RGBA>(BLACK_RGBA);

}

Gear_VideoLoop::~Gear_VideoLoop()
{
}

void Gear_VideoLoop::internalInit()
{
  _recording = true;
  _currentLoopFrame = 0;
  _nLoopFrames = 0;
  _pingpongDir = 1;
}

void Gear_VideoLoop::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;

  
  _sizeY = _image->height();
  _sizeX = _image->width();

  _outImage = _VIDEO_OUT->type();

  _outImage->resize(_sizeX, _sizeY);

  _playbackMode = CLAMP((ePlaybackMode)_MODE_IN->type()->value(), FORWARD, PING_PONG);

  _memory = MAX(_MEMORY->type()->intValue(), 0);
  _circbuf->resize(_sizeY*_sizeX, _memory);

  // Punch in!
  if ((int)_PUNCH_IN->type()->value() == 1)
  {
    NOTICE("Punch in");
    // Start recording.
    _recording = true;
    _nLoopFrames = 0;
  }
  
  if (_recording)
  {
    // Now recording.
    _circbuf->append(_image->data()); // append current image
    _nLoopFrames = MIN(_nLoopFrames+1,_memory); // update number of frames
    
    // Punch out!
    if ((int)_PUNCH_OUT->type()->value() == 1)
    {
      NOTICE("Punch out");
      // Stop recording.
      _recording = false;
      _currentLoopFrame = 0;
    }
    
    _circbuf->fillVectorFromBlock(_outImage, 0);
  }
  else
  {
    ASSERT_ERROR (_nLoopFrames > 0);
    
    switch (_playbackMode)
    {
    case FORWARD:
        _currentLoopFrame = (_currentLoopFrame + 1) % _nLoopFrames;
      break;
    case BACKWARD:
        if (_currentLoopFrame < 0)
           _currentLoopFrame = _nLoopFrames-1;
         _currentLoopFrame--;
      break;
    case PING_PONG:
      if (_nLoopFrames > 1)
      {
        if (_currentLoopFrame < 0)
          _pingpongDir=1;
        else if (_currentLoopFrame >= _nLoopFrames)
          _pingpongDir=-1;
        _currentLoopFrame+=_pingpongDir;
      }
      break;
    }

    
    _circbuf->fillVectorFromBlock(_outImage, _currentLoopFrame -_nLoopFrames + 1);
  }
 
}
