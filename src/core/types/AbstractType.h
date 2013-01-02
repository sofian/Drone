/* AbstractType.h
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

#ifndef __ABSTRACTTYPE_INCLUDED
#define __ABSTRACTTYPE_INCLUDED

#include <qcolor.h>
#include <iostream>
#include <string>
#include <vector>

// TYPE_BASE_METHODS(ClassName [, RGB) implementation
//
// Usage:
// TYPE_BASE_METHODS(MyType, (red, green, blue))
// TYPE_BASE_METHODS(MyType) // --> no color() definition, will use that of parent

#define __TYPE_BASE_METHODS_1_ARGS(ClassName) \
    virtual std::string typeName() const { return #ClassName; } \
    virtual void copyFrom(const AbstractType& t) { (*this) = *(static_cast<const ClassName*>(&t)); } \
    virtual AbstractType* clone() const { return new ClassName(*this); }


#define __TYPE_BASE_METHODS_2_ARGS(ClassName, RGB) \
  virtual QColor color() const { return QColor RGB;} \
  __TYPE_BASE_METHODS_1_ARGS(ClassName)

#define __TYPE_BASE_GET_MACRO(arg1, arg2, arg3, ...) arg3
#define __TYPE_BASE_METHODS_CHOOSER(...) \
  __TYPE_BASE_GET_MACRO(__VA_ARGS__, __TYPE_BASE_METHODS_2_ARGS, __TYPE_BASE_METHODS_1_ARGS)

#define TYPE_BASE_METHODS(...) __TYPE_BASE_METHODS_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

class AbstractType
{
public:
  AbstractType(){}
  
  virtual ~AbstractType() {}

  TYPE_BASE_METHODS(AbstractType, (0, 0, 0))

  int nSubTypes() const { return _subTypes.size();}
  const AbstractType* getSubType(int i) const { return _subTypes[i]; }
  const AbstractType* addSubType(const AbstractType &subType)
  {
    _subTypes.push_back(&subType);
    return &subType;
  }
	
  bool typeOf(AbstractType &other) const
  {
    return other.typeName() == typeName();
  }


protected:
  std::vector<const AbstractType*> _subTypes;
	
private:
};


#endif //__ABSTRACTTYPE_INCLUDED

