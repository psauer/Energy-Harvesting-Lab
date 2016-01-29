/*
 * config.h
 *
 *  Created on: Jan 27, 2016
 *      Author: paul
 */

#ifndef CONFIG_H_
#define CONFIG_H_

//board config

//launchpad led
#define LED1 BIT0
#define LED2 BIT6

#define SCLK    BIT5 // P1.5: SPI Clock
#define SOMI    BIT6 // P1.6: SPI SOMI (Slave Out, Master In)
#define SIMO    BIT7 // P1.7: SPI SIMO (Slave In, Master Out)
#define CE      BIT4 // P1.4: CE (Chip Enable)
#define CS      BIT3 // P1.3: CS (Chip Select)

//macros to toggle gpio pins
#define SET_CS_HIGH P1OUT |= CS;
#define SET_CS_LOW P1OUT &= ~CS;

#define SET_CE_HIGH P1OUT |= CE;
#define SET_CE_LOW P1OUT &= ~CE;

//rf config
#define CHANNEL 76



#endif /* CONFIG_H_ */
