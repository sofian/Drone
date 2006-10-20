/* Gear_StringOutput.h
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

#ifndef GEAR_StringOutput_INCLUDED
#define GEAR_StringOutput_INCLUDED

#include "Gear.h"
#include "StringType.h"
#include "ValueType.h"

class GearGui;

class Gear_StringOutput : public Gear
{
public:

  Gear_StringOutput(Schema *schema, std::string uniqueName);
  virtual ~Gear_StringOutput();

  void runVideo();
	std::string getString();

protected:
	GearGui *createGearGui(QCanvas *canvas);
	
private:

  PlugIn<StringType> *_STRING_IN;
  PlugIn<ValueType> *_VALUE_IN;



};

#endif
