/*
 * spi.c
 *
 *  Created on: Jan 29, 2016
 *      Author: paul
 */
#include <msp430g2553.h>
#include "spi.h"
#include "config.h"

void init_spi(void) {

  // Set UCSWRST
  UCB0CTL1 = UCSWRST;

  // configuring GPIO pins
  P1DIR  |= CS | CE; //setting GPIO CS and CE pins to output
  P1OUT  |= CS; //setting CS to high
  P1SEL  |= SOMI | SIMO | SCLK;
  P1SEL2 |= SOMI | SIMO | SCLK;

  // configuring SPI: 3-pin SPI Mode, 8-bit data, SPI master,
  UCB0CTL0 |= UCCKPH | UCMSB | UCMST | UCSYNC;
  UCB0CTL1 |= UCSSEL_2;   // SMCLK

  // starting state machine
  UCB0CTL1 &= ~UCSWRST;
}

uint8_t SPI_transfer(uint8_t data) {
  UCB0TXBUF = data;

  // wait for TX
  while (!(IFG2 & UCB0TXIFG));

  return UCB0RXBUF;
}

