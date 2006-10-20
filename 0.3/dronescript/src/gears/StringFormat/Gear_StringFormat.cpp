/* Gear_StringFormat.cpp
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


#include "Gear_StringFormat.h"
#include "Engine.h"

#include "GearMaker.h"

#include <stdio.h>

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_StringFormat(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "StringFormat";
  gearInfo.classification = GearClassifications::protocol().osc().instance();
  return gearInfo;
}
}

Gear_StringFormat::Gear_StringFormat(Schema *schema, std::string uniqueName) : 
  Gear(schema, "StringFormat", uniqueName)
{
  addPlug(_FORMAT = new PlugIn<StringType>(this, "Format", true));
  addPlug(_PARAMS = new PlugIn<ListType>(this, "Params", false));
  
  addPlug(_STRING = new PlugOut<StringType>(this, "String", true));
}

Gear_StringFormat::~Gear_StringFormat()
{

}

void Gear_StringFormat::runVideo()
{
  const ListType *listType = _PARAMS->type();
  std::string format = _FORMAT->type()->value();
  
  std::string outputString = "";

  for (ListType::const_iterator it = listType->begin(); it != listType->end(); ++it)
  {
    int pos1, pos2;
    if ((*it)->typeName() == StringType::TYPENAME)
    {
      pos1 = format.find('%');
      pos2 = format.find('s', pos1);
      StringType *type = (StringType*)*it;
      sprintf(_buffer, format.c_str(), type->value().c_str());
    }
    else if ((*it)->typeName() == ValueType::TYPENAME)
    {
      pos1 = format.find('%');
      pos2 = format.find('d', pos1);
      ValueType *type = (ValueType*)*it;
      sprintf(_buffer, format.c_str(), type->intValue());
    }

    outputString += _buffer;
    format = format.substr(pos2+1);

    NOTICE("Outputstring: %s, format: %s.", outputString.c_str(), format.c_str());
  }

  outputString += format;
  
  _STRING->type()->setValue(outputString);
}

