/* Gear_VideoRecord.h
 * Copyright (C) 2004--2005 Mathieu Guindon, Julien Keable, Jean-Sebastien Senecal
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

#ifndef GEAR_VIDEORECORD_INCLUDED
#define GEAR_VIDEORECORD_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"

template <class T>
class CircularBuffer;

class Gear_VideoRecord : public Gear
{
public:
  Gear_VideoRecord(Schema *schema, std::string uniqueName);
  virtual ~Gear_VideoRecord();

  void runVideo();

protected:
  void internalInit();

private:

  // Inputs.
  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugIn<ValueType> *_RECORD_IN;
  PlugIn<ValueType> *_MEMORY_IN;
  PlugIn<ValueType> *_FRAME_IN;

  // Outputs.
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  PlugOut<ValueType> *_N_FRAMES_OUT;

  // Functions.
  void init();

  //local var
  const VideoRGBAType *_image;
  VideoRGBAType *_outImage;

  CircularBuffer<RGBA> * _circbuf;

  int _nFrames;
  int _memory;
};

#endif
