/* Gear_OscOutput.cpp
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


#include "Gear_OscOutput.h"
#include "Engine.h"

#include "GearMaker.h"

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_OscOutput(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "OscOutput";
  gearInfo.classification = GearClassifications::protocol().osc().instance();
  return gearInfo;
}
}

Gear_OscOutput::Gear_OscOutput(Schema *schema, std::string uniqueName) : 
	Gear(schema, "OscOutput", uniqueName)
{

  addPlug(_PORT = new PlugIn<StringType>(this, "Port", false, new StringType("7770")));
  addPlug(_IP = new PlugIn<StringType>(this, "IP", false, new StringType("127.0.0.1")));	

  addPlug(_OSC_IN = new PlugIn<OscMessageType>(this, "Osc In", true));
}

Gear_OscOutput::~Gear_OscOutput()
{

}

void Gear_OscOutput::runVideo()
{
  if ((_IP->type()->value().size()<=0) || (_PORT->type()->value().size()<=0))		
		return;

  		
  lo_address t = lo_address_new(_IP->type()->value().c_str(), _PORT->type()->value().c_str());
  lo_message msg = lo_message_new();
  OscMessageType* oscMessage = _OSC_IN->type();
  ListType args = oscMessage->args();


  for(ListType::iterator it=args.begin();it!=args.end();++it)
  {
    if ((*it)->typeName() == StringType::TYPENAME)
    	lo_message_add_string(msg, ((StringType*)(*it))->value().c_str());
    else if ((*it)->typeName() == ValueType::TYPENAME)
    	lo_message_add_float(msg, ((ValueType*)(*it))->value());
  }

  if (lo_send_message(t, oscMessage->path().value().c_str(), msg)== -1) 
  {
     std::cout << "OSC error " << lo_address_errno(t) << " " << lo_address_errstr(t);
  }
  
  lo_address_free(t);
  lo_message_free(msg);

}

