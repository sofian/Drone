#ifndef GEAR_PATTERNGEN_INCLUDED
#define GEAR_PATTERNGEN_INCLUDED


#include "Gear.h"
#include "SignalType.h"
#include "VideoType.h"
#include <string>

class Gear_PatternGen : public Gear
{
public:

  static const std::string SETTING_SIZE_X;
  static const std::string SETTING_SIZE_Y;

  Gear_PatternGen(Engine *engine, std::string name);
  virtual ~Gear_PatternGen();

  void runVideo();

  bool ready();

protected:
  void onUpdateSettings();

private:

  PlugOut<VideoTypeRGBA> *_VIDEO_OUT;
  PlugIn<ValueType> *_PARAM_IN;

  MatrixType<RGBA> *_outImage; 
  RGBA *_outData;

  int _iterSizeY;
  int _iterSizeX;    

  unsigned char *_imageOut;

};

#endif