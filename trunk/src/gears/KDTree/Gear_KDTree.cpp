/* Gear_KDTree.cpp
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

#include "Gear_KDTree.h"
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_KDTree(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "KDTree";
  gearInfo.classification = GearClassifications::video().distortion().instance();
  return gearInfo;
}
}

Gear_KDTree::Gear_KDTree(Schema *schema, std::string uniqueName)
: Gear(schema, "KDTree", uniqueName)
{
  // Inputs.
  addPlug(_VIDEO_IN = new PlugIn<VideoRGBAType>(this, "ImgIN"));
  addPlug(_DEPTH_IN = new PlugIn<ValueType>(this, "Depth", new ValueType(4, 0, 16)));
  addPlug(_H_FIRST_IN = new PlugIn<ValueType>(this, "HFirst", new ValueType(0, 0, 1)));
  addPlug(_H_CELLS_IN = new PlugIn<ValueType>(this, "HCells", new ValueType(2, 2, 16)));
  addPlug(_V_CELLS_IN = new PlugIn<ValueType>(this, "VCells", new ValueType(2, 2, 16)));

  // Outputs.
  addPlug(_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ImgOUT"));
  addPlug(_AREA_OUT = new PlugOut<AreaArrayType>(this, "Segm"));
  
  // Internal objects.
  _rasterer = new Rasterer();
  //_intensitiesTable = new SummedAreaTable<unsigned char, int, 1>();
  _table = new SummedAreaTable<>();
}

Gear_KDTree::~Gear_KDTree()
{
  delete _rasterer;
  delete _table;
  //delete _intensitiesTable;
}

bool Gear_KDTree::ready()
{
  return(_VIDEO_IN->connected() && (_VIDEO_OUT->connected() || _AREA_OUT->connected()));
}

void Gear_KDTree::init()
{
  _rasterer->setImage(_VIDEO_OUT->type());
  _rasterer->setColor(0, 0, 0); // black lines only
  _minCellSize = 2; // XXX temporaire (should be an input)
}

void Gear_KDTree::runVideo()
{
  _image = _VIDEO_IN->type();
  if (_image->isNull())
    return;

  _outImage = _VIDEO_OUT->type();
  _outImage->resize(_image->width(), _image->height());

  //_intensities->resize(_image->width(), _image->height());
  //rgba2grayscale(_intensities.data(), _image->data(), _intensities.size());
  
  _sizeX = _image->width();
  _sizeY = _image->height();

  // initialize
  _maxDepth = MAX(_DEPTH_IN->type()->intValue(),   0);
  _nHCells  = CLAMP(_H_CELLS_IN->type()->intValue(), 2, 1024);
  _nVCells  = CLAMP(_V_CELLS_IN->type()->intValue(), 2, 1024);

  _rasterer->setImage(_outImage);

  // build accumulation buffers
  _table->reset((unsigned char*)_image->data(), _image->width(), _image->height());
  //_intensitiesTable->reset((unsigned char*)_intensities.data(), _intensities.width(), _intensities.height());

  // clear areas
  _AREA_OUT->type()->resize(0);
  
  // create splits
  split(0, _sizeX-1, 0, _sizeY-1, 0, ( _H_FIRST_IN->type()->value() > 0.5) );
}

void Gear_KDTree::split(int x0, int x1, int y0, int y1, int depth, bool hSplit)
{
  int x0minus1 = x0-1;
  int y0minus1 = y0-1;
  
  // Get the total values in the area.
  int rgba[SIZE_RGBA];
  int intensity;
  int area;
  _table->getSum(rgba, area, x0minus1, y0minus1, x1, y1);
  //_intensitiesTable->getSum(&intensity, area, x0minus1, y0minus1, x1, y1);
  
  if (depth == _maxDepth)
  {
    // Draw a rectangle around the area and paint it with the average color.
    if (_VIDEO_OUT->connected())
    {
      _rasterer->setColor(rgba[0] / area, rgba[1] / area, rgba[2] / area);
      _rasterer->rect(x0, y0, x1, y1, true);
      _rasterer->setColor(0,0,0);
    }
    
    // Add area to list.
    if (_AREA_OUT->connected())
    {
      Area a;
      a.x0 = x0; a.y0 = y0;
      a.x1 = x1; a.y1 = y1;
      _AREA_OUT->type()->push_back(a);
    }

    return;
  }

  if (abs(x1 - x0) < _minCellSize || abs(y1 - y0) < _minCellSize) // XXX faut faire la v�rif avant (l� c'est trop tard...)
    return;

  // Now split.
  if (hSplit)
  {
    int cellValue = sum(rgba, SIZE_RGB) / _nHCells; // XXX should calculate luminance for better effect

    // horizontal split
    int mid;
    for (int i=1; i<_nHCells; ++i)
    {
      int cut = i*cellValue;
      int upper = x1;
      int lower = x0;
      mid = lower;
      // binary search
      while (lower != upper)
      {
        mid = (lower+upper) / 2; // take the mean
        /// XXX pas besoin de "area" ici, il faut du code separe getSumAndArea dans SummedAreaTable...
        _table->getSum(rgba, area, x0minus1, y0minus1, mid, y1);
        if (sum(rgba, SIZE_RGB) < cut)
          lower = mid+1; // look right //*** attention risque d'erreur : v�rifier
        else
          upper = mid;  // look left
      }
      // split the area in two
      split(x0, mid, y0, y1, depth+1, false);
      x0 = mid;
    }
    split(mid, x1, y0, y1, depth+1, false);
  }
  else
  {
    int cellValue = sum(rgba, SIZE_RGB) / _nVCells; // XXX should calculate luminance for better effect

    // vertical split
    int mid;
    
    for (int i=1; i<_nVCells; ++i)
    {
      int cut = i*cellValue;
      int upper = y1;
      int lower = y0;
      mid = lower;
      // binary search
      while (lower != upper)
      {
        mid = (lower+upper) / 2; // take the mean
        /// XXX pas besoin de "area" ici, il faut du code separe getSumAndArea dans SummedAreaTable...
        _table->getSum(rgba, area, x0minus1, y0minus1, x1, mid);
        if (sum(rgba, SIZE_RGB) < cut)
          lower = mid+1; // look up //*** attention risque d'erreur : v�rifier
        else
          upper = mid;  // look down
      }
      // split the area in two
      split(x0, x1, y0, mid, depth+1, true);
      y0 = mid;
    }
    split(x0, x1, mid, y1, depth+1, true);
  }
}
