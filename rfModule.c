/*
 * rfModule.c
 *
 *  Created on: Jan 27, 2016
 *      Author: paul
 */

/*
 * This file is part of the MSP430 SPI example.
 *
 * Copyright (C) 2012 Stefan Wendler <sw@kaltpost.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/******************************************************************************
 * SPI example for MSP430
 *
 * Initialize SPI on USCI_B to communicate with the Nordic NRF24l01+
 *
 * Stefan Wendler
 * sw@kaltpost.de
 * http://gpio.kaltpost.de
 ******************************************************************************/

#include <msp430g2553.h>

#include "rfModule.h"

#define SET_CS_HIGH P1OUT |= CS;
#define SET_CS_LOW P1OUT &= ~CS;

#define SET_CE_HIGH P1OUT |= CE;
#define SET_CE_LOW P1OUT &= ~CE;

#define SCLK    BIT5 // P1.5: SPI Clock
#define SOMI    BIT6 // P1.6: SPI SOMI (Slave Out, Master In)
#define SIMO    BIT7 // P1.7: SPI SIMO (Slave In, Master Out)
#define CE      BIT4 // P1.4: CE (Chip Enable)
#define CS      BIT3 // P1.3: CS (Chip Select)

void init_spi(void) {

  // Set UCSWRST
  UCB0CTL1 = UCSWRST;

  // configuring GPIO pins
  P1DIR  |= CS | CE; //setting GPIO CS and CE pins to output
  P1OUT  |= CS; //setting CS to high
  P1SEL  |= SOMI + SIMO + SCLK;
  P1SEL2 |= SOMI + SIMO + SCLK;

  // configuring SPI: 3-pin SPI Mode, 8-bit data, SPI master,
  UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;
  UCB0CTL1 |= UCSSEL_2;   // SMCLK
  //UCB0STAT |= UCLISTEN; //enabling loopback mode for testing

  // starting state machine
  UCB0CTL1 &= ~UCSWRST;
}

void init_rfModule (void) {
  uint8_t status, byte;
//TODO check status byte to see if == TX_FULL
  //setting power up to 1
  byte = (1 << EN_CRC) | (1 << PWR_UP);
  status = SPI_write_reg(WRITE_REG | CONFIG, byte);
  //setting RF power to be -18dBm, Data rate to 2Mbps
  byte = (1 << RF_PWR_L) | (1 << RF_PWR_H) | (1 << LNA_HCURR) | (1 << RF_DR);
  status = SPI_write_reg(WRITE_REG | RF_SETUP, byte);
  //setting the TX - RX channel
  status = SPI_write_reg(WRITE_REG | RF_CH, 0x08);//choosing channel 0x08
}

uint8_t SPI_RW(uint8_t data) {
  UCB0TXBUF = data;

  // wait for TX
  while (!(IFG2 & UCB0TXIFG));

  return UCB0RXBUF;
}

uint8_t SPI_write_reg(uint8_t reg, uint8_t value) {
  uint8_t status;
  SET_CS_LOW;                   // CS low, init SPI transaction

  status = SPI_RW(reg);      // select register
  SPI_RW(value);             // ..and write value to it..

  SET_CS_HIGH;                   // CS high again
  return(status);            // return nRF24L01 status uchar
}

void SPI_read(uint8_t reg, uint8_t * output_buffer) {

  SET_CS_LOW;           // CSN low, initialize SPI communication...

  output_buffer[0] = SPI_RW(reg);            // Select register to read from..
  output_buffer[1] = SPI_RW(NOP1);    // ..then read registervalue

  SET_CS_HIGH;         // CSN high, terminate SPI communication

}

void transmit_byte (uint8_t data) {
  uint8_t status;
  SET_CE_LOW
  status = SPI_write_reg(WR_TX_PLOAD, data);

  SET_CE_HIGH
  //TODO insert wait here. min 10 us or interupt checking
  SET_CE_LOW

}

