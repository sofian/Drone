/* Gear_VideoAdd.cpp
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

#include "Gear_VideoAdd.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_VideoAdd, Gear_VideoAdd, "VideoAdd")

Gear_VideoAdd::Gear_VideoAdd(Schema *schema, std::string uniqueName) : Gear(schema, "VideoAdd", uniqueName)
{
  addPlug(_VIDEO_IN_A = new PlugIn<VideoRGBAType>(this, "ImgA"));
  addPlug(_VIDEO_IN_B = new PlugIn<VideoRGBAType>(this, "ImgB"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgO"));
}

Gear_VideoAdd::~Gear_VideoAdd()
{

}

bool Gear_VideoAdd::ready()
{
  return(_VIDEO_IN_A->connected() && _VIDEO_IN_B->connected() && _VIDEO_OUT->connected());
}



void Gear_VideoAdd::runVideo()
{
  _imageA = _VIDEO_IN_A->type();
  _imageB = _VIDEO_IN_B->type();
  _outImage = _VIDEO_OUT->type();

  _imageOutSizeX = MAX(_imageA->width(), _imageB->width());
  _imageOutSizeY = MAX(_imageA->height(), _imageB->height());
  _outImage->resize(_imageOutSizeX, _imageOutSizeY);

  if (_imageA->isNull())
  {
    if (_imageB->isNull())
      return;
      
    memcpy(_outImage->data(), _imageB->data(), _imageA->size()*sizeof(RGBA));
    return;
  }
  else if (_imageB->isNull())
  {  
    memcpy(_outImage->data(), _imageA->data(), _imageA->size()*sizeof(RGBA));
    return;
  }

  _imageASizeX = _imageA->width();
  _imageASizeY = _imageA->height();
  _imageBSizeX = _imageB->width();
  _imageBSizeY = _imageB->height();
  _iterSizeX = MIN(_imageASizeX,_imageBSizeX);
  _iterSizeY = MIN(_imageASizeY,_imageBSizeY);

  _dataA = _imageA->data();    
  _dataB = _imageB->data();    
  _outData = _outImage->data();

  register int mmxCols=_iterSizeX/2;
  register int index;    

  _largerImage = _imageASizeX > _imageBSizeX ? _dataA : _dataB;
  _tallerImage = _imageASizeY > _imageBSizeY ? _dataA : _dataB;

  for (int y=0;y<_iterSizeY;y++)
  {
    _mmxImageInA = (double*)&_dataA[y*_imageASizeX];
    _mmxImageInB = (double*)&_dataB[y*_imageBSizeX];
    _mmxImageOut = (double*)&_outData[y*_imageOutSizeX];

    for (index=0;index<mmxCols;index++)
    {
      __asm__ volatile (
                       "\n\t movq %1,%%mm0        \t# (u) load imageA"
                       "\n\t movq %2,%%mm1        \t# (u) load imageB"
                       "\n\t paddusb %%mm0,%%mm1   \t# (u) L2 = L0 - L2"
                       "\n\t movq %%mm1,%0        \t# (u) store result "
                       : "=m" (_mmxImageOut[index])  // this is %0, output
                       : "m"  (_mmxImageInA[index]), // this is %1, image A
                       "m"  (_mmxImageInB[index]) // this is %1, image A
                       );
    }

  }

  __asm__("emms" : : );

}

