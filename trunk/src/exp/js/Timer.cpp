/* Timer.cpp
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

// Copyright (C) 2004 Ronan Collobert (collober@idiap.ch)
//                
// This file is part of Torch 3.
//
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "Timer.h"
#ifndef _MSC_VER
  #include <sys/times.h>
  #include <unistd.h>
#endif

#ifdef _MSC_VER
time_t Timer::_base_time = 0;
#endif

float Timer::getRunTime()
{
#ifdef _MSC_VER
  time_t truc_foireux;
  time(&truc_foireux);
  return(difftime(truc_foireux, base_time));
#else
  struct tms current;
  times(&current);

  float norm = (float)sysconf(_SC_CLK_TCK);
  return(((float)current.tms_utime)/norm);
#endif
}

Timer::Timer()
{
#ifdef _MSC_VER
  while (!base_time)
    time(&base_time);
#endif
  _totalTime = 0;
  _isRunning = true;
  _startTime = getRunTime();
}

void Timer::reset()
{
  _totalTime = 0;
  _startTime = getRunTime();
}

void Timer::stop()
{
  if (!_isRunning)
    return;

  float _currentTime = getRunTime() - _startTime;
  _totalTime += _currentTime;
  _isRunning = false;
}

void Timer::resume()
{
  if (_isRunning)
    return;

  _startTime = getRunTime();
  _isRunning = true;
}

float Timer::getTime()
{
  if (_isRunning)
  {
    float _currentTime = getRunTime() - _startTime;
    return(_totalTime+_currentTime);
  } else
    return _totalTime;
}

Timer::~Timer()
{
}
