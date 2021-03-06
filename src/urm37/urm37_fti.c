/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Thanks to Adafruit for supplying a google translated version of the
 * Chinese datasheet and some clues in their code.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "urm37.h"
#include "upm_fti.h"

/** 
 * This file implements the Function Table Interface (FTI) for this sensor
 */

const char upm_light_name[] = "URM37";
const char upm_light_description[] = "Ultrasonic Ranger";
// problem here is it's an either/or analog vs. uart.  So we will just
// only support analog for now
// 1st gpio is reset, 2nd is trigger
const upm_protocol_t upm_light_protocol[] = {UPM_ANALOG, UPM_GPIO, UPM_GPIO};
const upm_sensor_t upm_light_category[] = {UPM_DISTANCE};

// forward declarations
const void* upm_urm37_get_ft(upm_sensor_t sensor_type);
void* upm_urm37_init_name();
void upm_urm37_close(void* dev);
upm_result_t upm_urm37_get_distance(void* dev, float* distance,
                                    upm_distance_u unit);

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_urm37_init_name,
    .upm_sensor_close = &upm_urm37_close,
};

static const upm_distance_ft dft =
{
    .upm_distance_get_value = &upm_urm37_get_distance
};

const void* upm_urm37_get_ft(upm_sensor_t sensor_type)
{
  switch(sensor_type)
    {
    case UPM_SENSOR:
      return &ft;
      
    case UPM_DISTANCE:
      return &dft;
      
    default:
      return NULL;
    }
}

void* upm_urm37_init_name()
{
    return NULL;
}


void upm_urm37_close(void* dev)
{
    urm37_close((urm37_context)dev);
}

upm_result_t upm_urm37_get_distance(void* dev, float* distance,
                                    upm_distance_u unit)
{
  // only cm returned by sensor
  float dist;
  urm37_get_distance((urm37_context)dev, &dist, 0);

  switch(unit)
    {
    case CENTIMETER:
      *distance = dist;
      return UPM_SUCCESS;

    case INCH:
      *distance = dist / 2.54;
      return UPM_SUCCESS;

    default:
      return UPM_ERROR_INVALID_PARAMETER;
    }
}

