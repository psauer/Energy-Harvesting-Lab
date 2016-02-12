/*
 * tmp102.c
 *
 *  Created on: Jan 29, 2016
 *      Author: paul
 */

#include "tmp102.h"
#include "i2c.h"
#include "config.h"
#include <stdio.h>
#include "uart.h"

static uint8_t buff[3];

static void set_OS_mode(void) {
  //configuring the temperature sensor to use OS: one shot mode & shutdown
  buff[0] = TMP102_CONFIG_REG;
  buff[1] = TMP102_OS | TMP102_SD;
  buff[2] = 0x00;

  //configuring temperature sensor
  i2c_write(buff, 3);
}
void init_tmp102(void) {

  init_i2c(TMP102_ADDRESS);
  //configuring the temperature sensor to shutdown mode
  buff[0] = TMP102_CONFIG_REG;
  buff[1] = TMP102_SD;
  buff[2] = 0x00;

  //configuring temperature sensor
  i2c_write(buff, 3);
}

int16_t tmp102_get_temp(void) {
  int16_t temperature;

  //set_OS_mode();
  i2c_read(TMP102_TEMP_REG, buff, 2);
  //note: temperature is only 12 bits and 2's compliment
  temperature = (buff[0] << 8) | buff[1];
  // temperature register is left justified so need to shift by 4 to compensate
  temperature >>= 4;
  if(temperature & (1 << 11)) {
    //negative temperature so filling the first 4 bits with 1's for 2's complement
    temperature |= 0xF000;
    temperature /= 16;
  } else {
    //quick way to divide by 16 but only works for positive numbers
    temperature >>= 4;
  }

  return temperature;
}

