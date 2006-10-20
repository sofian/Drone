/* Gear_ImageCapture.cpp
 * Copyright (C) 2004--2005 Jean-Sebastien Senecal
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

#include <iostream>
#include <sstream>
#include <stdio.h>

#include "Gear_ImageCapture.h"
#include "Engine.h"

#include "GearMaker.h"
#include "Math.h"

#include <qfile.h>


const std::string Gear_ImageCapture::SETTING_FILENAME = "Filename";
const std::string Gear_ImageCapture::FORMAT_EXTENSION = "png";
const std::string Gear_ImageCapture::DEFAULT_FILENAME = "droneImgCapture";

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_ImageCapture(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "ImageCapture";
  gearInfo.classification = GearClassifications::video().IO().instance();
  return gearInfo;
}
}

Gear_ImageCapture::Gear_ImageCapture(Schema *schema, std::string uniqueName) : 
  Gear(schema, "ImageCapture", uniqueName)  
{
  // Inputs.
  addPlug(_CAPTURE_IN = new PlugIn<ValueType>(this, "Go", false, new ValueType(0, 0, 0)));
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIn", true));
  

  _settings.add(Property::FILENAME, SETTING_FILENAME)->valueStr(DEFAULT_FILENAME);
}

Gear_ImageCapture::~Gear_ImageCapture()
{
}

void Gear_ImageCapture::saveImage(const std::string& filename, const VideoRGBAType *image)
{  
  QImage img(QSize(image->width(), image->height()), 32);
  
  unsigned char *data = (unsigned char*)image->data();
  for (int y = 0; y < img.height(); ++y)
    for (int x = 0; x < img.width(); ++x)
    {            
      img.setPixel(x, y, qRgba(*data, *(data+1), *(data+2), *(data+3)));      
      data+=4;
    }

  
  std::string baseFilename = filename;
  std::string filenameWithExtension;  
  //empty filename
  if (baseFilename.size()<=0)  
    baseFilename = DEFAULT_FILENAME;
   
  //remove extension from basefilename if already there
  if (baseFilename.find(FORMAT_EXTENSION.c_str(), baseFilename.size()-FORMAT_EXTENSION.size())!=std::string::npos)  
    baseFilename = baseFilename.substr(baseFilename.size()-FORMAT_EXTENSION.size(), baseFilename.size());
        
  //already exist? find unique name
  int counter=1;
  
  std::ostringstream oss;
  oss << baseFilename << "." << FORMAT_EXTENSION;
  while(QFile::exists(oss.str().c_str()))
  {    
    oss.seekp(0);
    oss.str("");
    oss << baseFilename << counter << "." << FORMAT_EXTENSION;
    counter++;
  }
    
  //save at best quality
  std::string extensionUpperCase = FORMAT_EXTENSION;
  std::transform(extensionUpperCase.begin(), extensionUpperCase.end(), extensionUpperCase.begin(), toupper);
  img.save(oss.str().c_str(), extensionUpperCase.c_str(), 100);
}

void Gear_ImageCapture::runVideo()
{
  if (_CAPTURE_IN->type()->value()!=0.0f)
  {
    saveImage(_settings.get(SETTING_FILENAME)->valueStr(), _VIDEO_IN->type());
  }
}



