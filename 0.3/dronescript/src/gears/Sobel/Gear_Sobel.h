/* Gear_Sobel.h
 * Copyright (C) 2005 Jean-Sebastien Senecal
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

#ifndef GEAR_EDGE_INCLUDED
#define GEAR_EDGE_INCLUDED


#include "Gear.h"
#include "VideoRGBAType.h"


class Gear_Sobel : public Gear
{
public:

  Gear_Sobel(Schema *schema, std::string uniqueName);
  virtual ~Gear_Sobel();

  void runVideo();
private:

  PlugIn<VideoRGBAType> *_VIDEO_IN;
  PlugOut<VideoRGBAType> *_VIDEO_OUT;
  
  //local var
  const VideoRGBAType *_image; 
  VideoRGBAType *_outImage; 
  unsigned char *_data;
  unsigned char *_outData;

  int _sizeY;
  int _sizeX;    

  // kernel iterators
  unsigned char
    *_p1, *_p2, *_p3,
    *_p4,       *_p6,
    *_p7, *_p8, *_p9;

  // output iterator
  unsigned char *_iterOutData;

  // computes a single step of the kernel and updates the kernel and output pointers
  inline void singleStep();
};

void Gear_Sobel::singleStep()
{
  for (int z=0; z<SIZE_RGBA; ++z)
  {
    // Compute the kernel function.
    *_iterOutData++ = CLAMP0255(abs((int)(*_p1 + (*_p2 << 1) + *_p3 - *_p7 - (*_p8 << 1) - *_p9)) +
                                abs((int)(*_p3 + (*_p6 << 1) + *_p9 - *_p1 - (*_p4 << 1) - *_p7)));
    
    // Update iterators.
    _p1++; _p2++; _p3++; _p4++; _p6++; _p7++; _p8++; _p9++;
  }
}

#endif
