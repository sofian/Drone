#include "Gear_VideoMix.h"                       
#include "Engine.h"

#include <iostream>

#include "GearMaker.h"


Register_Gear(MAKERGear_VideoMix, Gear_VideoMix, "VideoMix")
const std::string Gear_VideoMix::SETTING_MIX_FUNCTION = "MIX FUNCTION : See the code or just have fun";

Gear_VideoMix::Gear_VideoMix(Engine *engine, std::string name) : Gear(engine, "VideoMix", name)
{
  addPlug(_VIDEO_IN_A = new PlugIn<VideoTypeRGBA>(this, "ImgA"));
  addPlug(_VIDEO_IN_B = new PlugIn<VideoTypeRGBA>(this, "ImgB"));
  addPlug(_VIDEO_OUT = new PlugOut<VideoTypeRGBA>(this, "ImgO"));
  addPlug(_AMOUNT_IN = new PlugIn<ValueType>(this, "ArgA", new ValueType(127)));
  addPlug(_MIXFUNC_IN = new PlugIn<ValueType>(this, "MixFunc", new ValueType(0)));

  _settings.add(Property::INT, SETTING_MIX_FUNCTION)->valueInt((int)BLEND);
}

Gear_VideoMix::~Gear_VideoMix()
{

}

bool Gear_VideoMix::ready()
{
  return(_VIDEO_IN_A->connected() && _VIDEO_IN_B->connected() && _VIDEO_OUT->connected());
}

void Gear_VideoMix::runVideo()
{
  _imageA = _VIDEO_IN_A->type()->image();
  _imageB = _VIDEO_IN_B->type()->image();
  _outImage = _VIDEO_OUT->type()->image();
  _mixType = (eVideoMixType)_MIXFUNC_IN->type()->value();

  if (_imageA->width() != _imageB->width() ||
      _imageA->height() != _imageB->height())
  {
    std::cerr << "images have to be of the same size for now" << std::endl;
    exit(0);
  }

  int size = _imageA->size();

  _outImage->resize(_imageA->width(), _imageA->height());

  _dataA = _imageA->data();    
  _dataB = _imageB->data();    
  _outData = _outImage->data();

  switch (_mixType)
  {
  case BLEND:
    blend_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                 (int) _AMOUNT_IN->type()->value(), size, SIZE_RGB);
    break;
  case SHADE:
    shade_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                 (int) _AMOUNT_IN->type()->value(), size, SIZE_RGB, 0);
    break;
  case DARKEN:
    darken_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                  size, SIZE_RGB, SIZE_RGB);
    break;
  case LIGHTEN:
    lighten_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                   size, SIZE_RGB, SIZE_RGB);
    break;
  case HUE_ONLY:
    hue_only_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                    size, SIZE_RGB, SIZE_RGB);
    break;
  case SATURATION_ONLY:
    saturation_only_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                           size, SIZE_RGB, SIZE_RGB);
    break;
  case VALUE_ONLY:
    value_only_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                      size, SIZE_RGB, SIZE_RGB);
    break;
  case COLOR_ONLY:
    color_only_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                      size, SIZE_RGB, SIZE_RGB);
    break;
  case MULTIPLY:
    multiply_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                    size, SIZE_RGB, SIZE_RGB);
    break;
  case DIVIDE:
    divide_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                  size, SIZE_RGB, SIZE_RGB);
    break;
  case SCREEN:
    screen_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                  size, SIZE_RGB, SIZE_RGB);
    break;
  case OVERLAY:
    overlay_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                   size, SIZE_RGB, SIZE_RGB);
    break;
  case DODGE:
    dodge_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                 size, SIZE_RGB, SIZE_RGB);
    break;
  case BURN:
    burn_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                size, SIZE_RGB, SIZE_RGB);
    break;
  case HARDLIGHT:
    hardlight_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                     size, SIZE_RGB, SIZE_RGB);
    break;
  case SOFTLIGHT:
    softlight_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                     size, SIZE_RGB, SIZE_RGB);
    break;
  case GRAIN_EXTRACT:
    grain_extract_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                         size, SIZE_RGB, SIZE_RGB);
    break;
  case GRAIN_MERGE:
    grain_merge_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                       size, SIZE_RGB, SIZE_RGB);
    break;
  case ADD:
    add_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
               size, SIZE_RGB, SIZE_RGB);
    break;
  case SUBTRACT:
    subtract_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                    size, SIZE_RGB, SIZE_RGB);
    break;
  case DIFFERENCE:
    difference_pixels((unsigned char*)_dataA, (unsigned char *)_dataB, (unsigned char*)_outData,
                      size, SIZE_RGB, SIZE_RGB);
    break;
  default:;
    memcpy(_outData, _dataA, size * sizeof(RGBA));
  }

}
