/* Gear_Blur.h
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

#ifndef GEAR_BLUR_INCLUDED
#define GEAR_BLUR_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoRGBAType.h"
#include "SummedAreaTable.h"

class Gear_Blur : public Gear
{
public:

  Gear_Blur(Schema *schema, std::string uniqueName);
  virtual ~Gear_Blur();

  void runVideo();

  bool ready();

private:

  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugIn<ValueType> *_AMOUNT_IN;

  //local var
  const VideoRGBAType *_image;     
  VideoRGBAType *_outImage;
  unsigned char *_outData;

  SummedAreaTable<> *_table;
  int _sum[SIZE_RGBA];

  int _sizeY;
  int _sizeX;

  int _x1,_y1,_x2,_y2;

  int _blurSize;
  int _area;
  
  void init();    
};

#endif
