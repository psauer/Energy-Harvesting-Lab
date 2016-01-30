/*
 * tmp102.c
 *
 *  Created on: Jan 29, 2016
 *      Author: paul
 */

#include "tmp102.h"
#include "i2c.h"
#include "config.h"

void init_tmp102(void) {
  uint8_t buffer[3];
  init_i2c(TMP102_ADDRESS);

  buffer[0] = TMP102_CONFIG_REG;
  buffer[1] = TMP102_OS | TMP102_SD;
  buffer[2] = 0x00;

  //configuring temperature sensor
  i2c_write(buffer, 2, 0);
}

int tmp102_get_temp(void) {
  uint8_t buff[2];
  int temperature;
  i2c_read(TMP102_TEMP_REG, buff, 2);
  temperature = (int)(buff[0] << 8) + (int)buff[1];
  return temperature;
}

