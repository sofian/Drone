/* threshold0r.c
 * Copyright (C) 2005 Jean-Sebastien Senecal
 * This file is a Frei0r plugin.
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

#include <stdlib.h>
#include <assert.h>

#include "frei0r.h"

typedef struct thresholder_instance
{
  unsigned int width;
  unsigned int height;
  unsigned char threshold; /* the threshold [0, 255] */
  unsigned char lut[256]; /* look-up table */
} thresholder_instance_t;

/* Clamps a int32-range int between 0 and 255 inclusive. */
unsigned char CLAMP0255(int32_t a)
{
  return (unsigned char)
    ( (((-a) >> 31) & a)  // 0 if the number was negative
      | (255 - a) >> 31); // -1 if the number was greater than 255
}

/* Updates the look-up-table. */
void update_lut(thresholder_instance_t *inst)
{
  int i;
  unsigned char *lut = inst->lut;
  unsigned char thresh = inst->threshold;
  if (thresh == 0xff)
    memset(lut, 0x00, 256*sizeof(unsigned char));
  else if (thresh == 0x00)
    memset(lut, 0xff, 256*sizeof(unsigned char));
  else
  {
    for (i=0; i<thresh; ++i)
      lut[i] = 0x00;
    for (i=thresh; i<256; ++i)
      lut[i] = 0xff;
  }
}

int f0r_init()
{
  return 1;
}

void f0r_deinit()
{ /* no initialization required */ }

void f0r_get_plugin_info(f0r_plugin_info_t* thresholder)
{
  thresholder->name = "Threshold0r";
  thresholder->author = "Jean-Sebastien Senecal";
  thresholder->plugin_type = F0R_PLUGIN_TYPE_FILTER;
  thresholder->color_model = F0R_COLOR_MODEL_BGRA8888;
  thresholder->frei0r_version = FREI0R_MAJOR_VERSION;
  thresholder->major_version = 0; 
  thresholder->minor_version = 2; 
  thresholder->num_params =  1; 
  thresholder->explanation = "Thresholds a source image";
}

void f0r_get_param_info(f0r_param_info_t* info, int param_index)
{
  switch(param_index)
  {
  case 0:
    info->name = "Threshold";
    info->type = F0R_PARAM_DOUBLE;
    info->explanation = "The threshold";
    break;
  }
}

f0r_instance_t f0r_construct(unsigned int width, unsigned int height)
{
  thresholder_instance_t* inst = 
    (thresholder_instance_t*)malloc(sizeof(thresholder_instance_t));
  inst->width = width; inst->height = height;
  /* init look-up-table */
  inst->threshold = 0;
  update_lut(inst);
  return (f0r_instance_t)inst;
}

void f0r_destruct(f0r_instance_t instance)
{
  free(instance);
}

void f0r_set_param_value(f0r_instance_t instance, 
                         f0r_param_t param, int param_index)
{
  assert(instance);
  thresholder_instance_t* inst = (thresholder_instance_t*)instance;

  switch(param_index)
  {
    unsigned char val;
  case 0:
    /* threshold */
    val = (unsigned char) (255.0 * *((double*)param));
    if (val != inst->threshold)
    {
      //      printf("value changed from %d to %d\n", (int)inst->threshold, (int)val);
      inst->threshold = val;
      update_lut(inst);
    }
    break;
  }
}

void f0r_get_param_value(f0r_instance_t instance,
                         f0r_param_t param, int param_index)
{
  assert(instance);
  thresholder_instance_t* inst = (thresholder_instance_t*)instance;
  
  switch(param_index)
  {
  case 0:
    *((double*)param) = (double)(inst->threshold) / 255.0;
    break;
  }
}

void f0r_update(f0r_instance_t instance, double time,
                const uint32_t* inframe, uint32_t* outframe)
{
  assert(instance);
  thresholder_instance_t* inst = (thresholder_instance_t*)instance;
  unsigned int len = inst->width * inst->height;
  int p;

  unsigned char* lut = inst->lut;
  unsigned char* dst = (unsigned char*)outframe;
  const unsigned char* src = (unsigned char*)inframe;
  for (p=0; p<len; ++p)
  {
    *dst++ = lut[*src++];
    *dst++ = lut[*src++];
    *dst++ = lut[*src++];
    dst++; src++;
  }
}

