/*
 * config.h
 *
 *  Created on: Jan 27, 2016
 *      Author: paul
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0)

//board config

//launchpad led
#define LED1 BIT0
#define LED2 BIT6

// spi pin def
#define SCLK    BIT4 // P1.4: SPI Clock
#define SOMI    BIT1 // P1.1: SPI SOMI (Slave Out, Master In)
#define SIMO    BIT2 // P1.2: SPI SIMO (Slave In, Master Out)
#define CE      BIT0 // P1.0: CE (Chip Enable)
#define CS      BIT3 // P1.3: CS (Chip Select)

// I2C pin def
#define SCL BIT6     //P1.6 SCL
#define SDA BIT7     //p1.7 SDA

//macros to toggle gpio pins
#define SET_CS_HIGH P1OUT |= CS;
#define SET_CS_LOW P1OUT &= ~CS;

#define SET_CE_HIGH P1OUT |= CE;
#define SET_CE_LOW P1OUT &= ~CE;

//rf config
#define CHANNEL 76

//temp sensor config
#define TMP102_ADDRESS 0x48 // Connect A0 to Ground



#endif /* CONFIG_H_ */
