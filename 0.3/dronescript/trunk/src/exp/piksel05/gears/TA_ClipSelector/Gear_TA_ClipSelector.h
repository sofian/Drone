/* Gear_TA_ClipSelector.h
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

#ifndef GEAR_TA_CLIPSELECTOR_INCLUDED
#define GEAR_TA_CLIPSELECTOR_INCLUDED


#include "Gear.h"
#include "ValueType.h"
#include "StringType.h"
#include "TA_DataType.h"

class Gear_TA_ClipSelector : public Gear
{
public:

  Gear_TA_ClipSelector(Schema *schema, std::string uniqueName);
  virtual ~Gear_TA_ClipSelector();

  void runVideo();

public:
  PlugIn<TA_DataType> *_DATA_IN;
  PlugIn<ValueType> *_HOTSPOT;

  PlugOut<StringType> *_CURRENT_CLIP;
  PlugOut<StringType> *_PREVIOUS_CLIP;
  PlugOut<ValueType> *_CLIP_CHANGED;
  
protected:
  int _currentSpot;
  int _previousSpot;
};

#endif
