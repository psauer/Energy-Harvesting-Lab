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

#define SET_SIMO_HIGH P1OUT |= SIMO;
#define SET_SIMO_LOW P1OUT &= ~SIMO;

#define SET_SCLK_HIGH P1OUT |= SCLK;
#define SET_SCLK_LOW P1OUT &= ~SCLK;

#define SCLK    BIT5 // P1.5: SPI Clock
#define SOMI    BIT6 // P1.6: SPI SOMI (Slave Out, Master In)
#define SIMO    BIT7 // P1.7: SPI SIMO (Slave In, Master Out)
#define CE      BIT4 // P1.4: CE (Chip Enable)
#define CS      BIT3 // P1.3: CS (Chip Select)

//=============================RF24l01 init config=====================================
char  TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01}; //TX address
char  RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01}; //RX address
char  sta;
char  tf, RxBuf[32],TxBuf[32];

static void inerDelay_us(int n) {
  for(;n>0;n--);
}

void init_spi(void) {
  /**
   * From TIs users manual
   *
     * The recommended USCI initialization/re-configuration process is:
   * 1. Set UCSWRST (BIS.B #UCSWRST,&UCxCTL1)
   * 2. Initialize all USCI registers with UCSWRST=1 (including UCxCTL1)
   * 3. Configure ports
   * 4. Clear UCSWRST via software (BIC.B #UCSWRST,&UCxCTL1)
   * 5. Enable interrupts (optional) via UCxRXIE and/or UCxTXIE
   */

  // (1)
  UCB0CTL1 = UCSWRST;

  // (2)
  P1DIR  |= CS | CE; //setting GPIO CS and CE pins to output
  P1OUT  |= CS | CE; //setting CS to high
  P1SEL  |= SOMI + SIMO + SCLK;
  P1SEL2 |= SOMI + SIMO + SCLK;

  // (3) 3-pin SPI Mode, 8-bit data, SPI master,
  UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;
  UCB0CTL1 |= UCSSEL_2;   // SMCLK
  //UCB0STAT |= UCLISTEN; //enabling loopback mode for testing

  // (4)
  UCB0CTL1 &= ~UCSWRST;

}

/**************************************************
Function: SPI_RW();

Description:
  Writes one byte to nRF24L01, and return the byte read
  from nRF24L01 during write, according to SPI protocol  */
/**************************************************/
uint8_t SPI_RW(uint8_t data) {
  UCB0TXBUF = data;

  // wait for TX
  while (!(IFG2 & UCB0TXIFG));

  return UCB0RXBUF;
}

uint8_t SPI_write_reg(uint8_t reg, uint8_t value) {
  uint8_t status;
  SET_CS_LOW;                   // CSN low, init SPI transaction

  status = SPI_RW(reg);      // select register
  SPI_RW(value);             // ..and write value to it..

  SET_CS_HIGH;                   // CSN high again
  return(status);            // return nRF24L01 status uchar
}

/**************************************************
Function: SPI_Read();

Description:
  Read one byte from nRF24L01 register, 'reg'  */
/**************************************************/
void SPI_Read(uint8_t reg, uint8_t * output_buffer) {
  uint8_t reg_val;

  SET_CS_LOW;           // CSN low, initialize SPI communication...

  output_buffer[0] = SPI_RW(reg);            // Select register to read from..
  output_buffer[1] = SPI_RW(NOP1);    // ..then read registervalue

  SET_CS_HIGH;         // CSN high, terminate SPI communication

}
