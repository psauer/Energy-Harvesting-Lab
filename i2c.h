/*
 * i2c.h
 *
 *  Created on: Jan 29, 2016
 *      Author: paul
 */

#ifndef I2C_H_
#define I2C_H_

#include <inttypes.h>

void init_i2c(uint8_t address);
void i2c_write(uint8_t * buff, uint8_t len, uint8_t wr);
void i2c_read(uint8_t reg, uint8_t * buff, uint8_t len);

#endif /* I2C_H_ */
