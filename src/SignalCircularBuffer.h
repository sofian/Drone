/* SignalCircularBuffer.h
 * Copyright (C) 2003 Mathieu Guindon, Julien Keable
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

#ifndef SIGNALCIRCULARBUFFER_INCLUDED
#define SIGNALCIRCULARBUFFER_INCLUDED

#include "Plug.h"

class SignalCircularBuffer
{
public:

  SignalCircularBuffer(int size);
  ~SignalCircularBuffer();

  void init(int size);

  void write(const float *buf, int size);
  void read(float *buf, int &size);

private:

  float *_buffer;
  int _readPos;
  int _writePos;
  int _size;
  int _availableData;
  pthread_mutex_t *_mutex;


};


#endif
