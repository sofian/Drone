/* Gear_BeatDetector.cpp
 * Copyright (C) 2005 Norman Casagrande, Jean-Sebastien Senecal
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

#include <cmath>

#include "Gear_BeatDetector.h"
#include "Math.h"
#include "Engine.h"

#include "GearMaker.h"

// the buffer of the DOWNSAMPLED data
static const unsigned int SECS_BUFFERED = 5; // 5 secs

static const unsigned int MIN_RANGE = 200; // equiv to min lag
static const unsigned int MAX_RANGE = 1000; // max lag is MAX_RANGE * N_REP
static const unsigned int N_REP = 2;

//static const unsigned int MIN_LAG = 100; // 200 msec
//static const unsigned int MAX_LAG = 3200; // 3 sec
static const float WORKING_FREQ = 1000; // 1KHz
static const float DEFAULT_SMOOTH_DECAY = 0.5;

static const float DEFAULT_ACORR_DECAY = 1;//0.95;
static const unsigned int ACORR_DECAY_STEP = 30;

extern "C" {
Gear* makeGear(Schema *schema, std::string uniqueName)
{
  return new Gear_BeatDetector(schema, uniqueName);
}

GearInfo getGearInfo()
{
  GearInfo gearInfo;
  gearInfo.name = "BeatDetector";
  gearInfo.classification = GearClassifications::signal().transform().instance();
  return gearInfo;
}
}

Gear_BeatDetector::Gear_BeatDetector(Schema *schema, std::string uniqueName)
  : Gear(schema, "BeatDetector", uniqueName)
{
  addPlug(_AUDIO_IN = new PlugIn<SignalType>(this, "AudioIn") );
  addPlug(_ACORR_VIDEO_OUT = new PlugOut<VideoRGBAType>(this, "ACorrOut") );

}

void Gear_BeatDetector::init()
{
	_minLag = MIN_RANGE;
	_maxLag = MAX_RANGE * N_REP;
	_acorrSize = _maxLag - _minLag + 1;
  _rangeSize = MAX_RANGE - MIN_RANGE + 1;

	_cirDownBuffer.resize( SECS_BUFFERED * (int)WORKING_FREQ );
	_cirDownBuffer.zero_fill();
	
	_dSampleIt = _cirDownBuffer.head();
	_dSampleLaggedIt = _dSampleIt - _minLag;
	
	_autoCorrelation.resize(_acorrSize, 0);
	_pAutoCorrelation = _autoCorrelation.data();
	
	size_t dataSize = _AUDIO_IN->type()->size();
  float sampleRate = (float)Engine::signalInfo().sampleRate();
  float downsampledSize = ((float)dataSize / sampleRate) * WORKING_FREQ;
    
  //_downSampledSig.resize( (int)(downsampledSize + 0.5) );
  _downSampledSig.resize( (int)(downsampledSize + 0.5), 0 );
  _pDownSampledSig = _downSampledSig.data();
  
  _smoothSampleAverage = 0.0f;

  _smoothDecay = DEFAULT_SMOOTH_DECAY;
  _acorrDecay = DEFAULT_ACORR_DECAY;
  _acorrDecayCounter = 0;
  _acorrDecayStep = ACORR_DECAY_STEP;
  
  _whenChkStep = 0;
  _whenChkSize = (unsigned int)(sampleRate/ (float)Engine::signalInfo().blockSize() + 0.5) / 2 ; // 0.5 sec..

  _dbgStep = 0;
  _dbgSize = _whenChkSize;
  
  //_outTestFile.open("sampledata.dat");
  
  std::cout << "Input Sample Rate: " << sampleRate << " KHz" << std::endl;
  std::cout << "Working Sample Rate: " << WORKING_FREQ << " KHz" << std::endl;
  std::cout << "Auto Correlation Size: " << _acorrSize << " (" << _minLag << "->" << _maxLag << ")" << std::endl;
  std::cout << "     --> Allocated Size: " << _autoCorrelation.size() << std::endl;
  std::cout << "Input samples: " << dataSize << " -> " << downsampledSize << std::endl;
  std::cout << "Samples in buffer: " << (int)downsampledSize << std::endl;
  std::cout << "Checking every " << _whenChkSize+1 << " calls of runAudio" << std::endl;
}

bool Gear_BeatDetector::ready()
{
  return (_AUDIO_IN->connected() /* && _ACORR_VIDEO_OUT->connected()*/);
}

void Gear_BeatDetector::runAudio()
{
  float* tmpNewData = _AUDIO_IN->type()->data();
	unsigned int dataSize = _AUDIO_IN->type()->size();

  for (unsigned int i = 0; i < dataSize; ++i)
    tmpNewData[i] = fabs(tmpNewData[i]);

  downsample<float>( _pDownSampledSig, _downSampledSig.size(), 
                     tmpNewData, dataSize);
  
  //create_histogram<float>(_pDownSampledSig, _downSampledSig.size(), 
  //                        tmpNewData, dataSize);
  
	smooth<float>( _pDownSampledSig, _downSampledSig.size(), _smoothDecay,
                _smoothSampleAverage);
  
/*  _downSampledSig.fill(0);
  if (_dbgStep >= _dbgSize)
  {
    _downSampledSig[0] = 1;
    _dbgStep = 0;
    //std::cout << "Added one.." << std::endl;
  }
  ++_dbgStep;
*/    
	// add new data to buffer and autocorrelation
	for (size_t i = 0; i < _downSampledSig.size(); ++i)
  {
    //_outTestFile << _pDownSampledSig[i] << "\n";
		addSample( _pDownSampledSig[i] );
  }
	
  float maxVal = 0;
  float tmpSum;
  unsigned int idx = 0;
  drawACorr();

  ++_whenChkStep;
  if (_whenChkStep > _whenChkSize)
  {

    for (unsigned int i = 0; i < _rangeSize; ++i)
    {
      tmpSum = 0;
     for (unsigned int j = 1; j < N_REP; j *= 2)
     {
       tmpSum += _pAutoCorrelation[i*j];
     }
    
     if (tmpSum > maxVal)
     {
       maxVal = tmpSum;
       idx = i;   
     }
   }
  
  
	  //float* it = std::max_element(_pAutoCorrelation, _pAutoCorrelation + _acorrSize);
	  //std::cout << "max[" << idx << "] =" << maxVal << std::endl;
    std::cout << "max every " << idx + _minLag << " msecs" << std::endl;
    _whenChkStep = 0;
  }

    
  ++_acorrDecayCounter;
  if (_acorrDecayCounter > _acorrDecayStep)
  {
    //std::cout << "Decaying.." << std::endl;
    applyDecay();
    _acorrDecayCounter = 0;
  }
  
}


// performs the autocorrelation with one given sample
// Note: the direction of updating is to the right of the
// circle (rigth == newer data)
//
//        **    \
//     *      *  \
//    *        *  v
//    *        *
//     *      *
//        ** 
inline void Gear_BeatDetector::addSample(float sample)
{
  // norman: with random access circular array a samplelagged is
  // useless! We can get it in no time everytime!
	++_dSampleIt;
  ++_dSampleLaggedIt;
  *_dSampleIt = sample;
	
  CircularArray<float>::iterator sIt = _dSampleLaggedIt;
  
	// Autocorrelation of lag is:
	// A(lag) = sum_n x_n * x_(n-lag)
	// Where:
	// x_n = sample
	// x_(n-lag) is sIt which is moving into the past
	  
  // note that maxLag - minLag + 1 = size of pArray!
  for (unsigned int i = 0; i < _acorrSize; ++i)
  {
    _pAutoCorrelation[i] += ( sample * (*sIt) );
    --sIt;
  }
  
}

inline void Gear_BeatDetector::applyDecay()
{
  for (unsigned int i = 0; i < _acorrSize; ++i)
    _pAutoCorrelation[i] *= 0.98;//_acorrDecay;
}

void Gear_BeatDetector::drawACorr()
{
  int div = 4;
  int sizey = 200;
  int smallerSize = _acorrSize/div;
  Array<float> smallerACorr(smallerSize);
  float max;
  unsigned int aIdx = 0;
  for (int i = 0; i < smallerSize; ++i)
  {
    max = 0;
    for (int j = 0; j < div; ++j)
    {
//      if (aIdx >= _acorrSize)
//        break;
      if ( _pAutoCorrelation[aIdx] > max)
        max = _pAutoCorrelation[aIdx];
      ++aIdx;
    }
    smallerACorr[i] = max;
  }

  _pOutImage = _ACORR_VIDEO_OUT->type();
  _pOutImage->resize(smallerSize,sizey+1);
  _pOutImage->fill(WHITE_RGBA);  

	float maxVal = *std::max_element(smallerACorr.begin(), smallerACorr.end());
  
  if (maxVal <= 0.00000001)
    return; 

  float yRatio = (float)sizey / maxVal;
 
  //std::cout << "smallerSize: " << smallerSize << " - sizey+1: " << sizey+1 << std::endl;
  int yRatePos = std::max( sizey-(int)(yRatio+0.5), 0);

  for (int x = 0; x < smallerSize; ++x)
  {
  
    _pOutImage->operator()(x, yRatePos) = BLACK_RGBA;
    int yLineSize = (int)(smallerACorr[x]*yRatio+0.5);
    for (int y = 0; y < yLineSize; ++y)
      _pOutImage->operator()(x, sizey-y)=BLACK_RGBA;
      
  }
  
}

