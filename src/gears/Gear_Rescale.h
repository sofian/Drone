/* Gear_Rescale.h
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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

#ifndef GEAR_RESCALE_INCLUDED
#define GEAR_RESCALE_INCLUDED


#include "Gear.h"
#include "VideoType.h"
#include "ValueType.h"
#include "Utils.h"

class Gear_Rescale : public Gear
{
public:

  Gear_Rescale(Engine *engine, std::string name);
  virtual ~Gear_Rescale();

  void runVideo();

  bool ready();

private:

  PlugIn<VideoTypeRGBA> *_VIDEO_IN;
  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<ValueType> *_WIDTH_IN;
  PlugIn<ValueType> *_HEIGHT_IN;

  //local var
  const VideoTypeRGBA *_image; 
  VideoTypeRGBA *_outImage; 

  int _newWidth;
  int _newHeight;
};

#endif
