/*
 * spi.h
 *
 *  Created on: Jan 29, 2016
 *      Author: paul
 */

#ifndef SPI_H_
#define SPI_H_

#include <inttypes.h>

void init_spi(void);
uint8_t SPI_transfer(uint8_t data);

#endif /* SPI_H_ */
