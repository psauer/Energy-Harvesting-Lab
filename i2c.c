/*
 * i2c.c
 *
 *  Created on: Jan 29, 2016
 *      Author: paul
 */
#include <msp430g2553.h>
#include "config.h"
#include "i2c.h"
#include "uart.h"

static volatile uint8_t TX_byte_ctr, RX_byte_ctr;
static volatile uint8_t* TX_byte_ptr, *RX_byte_ptr;
static volatile uint8_t read_reg;

void init_i2c(uint8_t address) {
  // Set UCSWRST
  UCB0CTL1 = UCSWRST;
  // configuring GPIO pins
  P1SEL  |= SCL | SDA;
  P1SEL2 |= SCL | SDA;
  // configuring I2C: I2C Master, synchronous mode
  UCB0CTL0 |= UCMST | UCMODE_3 | UCSYNC;
  UCB0CTL1 |= UCSSEL_2;       // SMCLK
  UCB0BR0 = 12;               // fSCL = SMCLK/12 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = address; // Set slave address
  // starting state machine
  UCB0CTL1 &= ~UCSWRST;
  IE2 |= UCB0RXIE;//enable TX & RX interrupts
}

void i2c_write(uint8_t * buff, uint8_t len, uint8_t wr) {
  TX_byte_ctr = len + 1;
  TX_byte_ptr = buff;
  read_reg = wr;

  while (UCB0CTL1 & UCTXSTP);      // Ensure stop condition got sent
  UCB0CTL1 |= UCTR + UCTXSTT;      // I2C TX, start condition
  __disable_interrupt(); // disable
  IE2 |= UCB0TXIE;
  __bis_SR_register(GIE); // Enter LPM0 w/ interrupts
  while(!(IFG2 & UCB0TXIFG)) {
    uartPutChar('z');
  }
}

void i2c_read(uint8_t reg, uint8_t * buff, uint8_t len) {
  uint8_t temp = reg;

  RX_byte_ctr = len;
  RX_byte_ptr = buff;

  //writing register address where we want to read from
  i2c_write(&temp, 0, 1);
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void) {
  uartPutChar('1');
  if (TX_byte_ctr) {                           // Check TX byte counter
    UCB0TXBUF = *TX_byte_ptr++;                     // Load TX buffer
    TX_byte_ctr--;                            // Decrement TX byte counter
    uartPutChar('2');
  } else {
    uartPutChar('3');
    if(read_reg == 1) { // reading data from register so send start condition
      UCB0CTL1 |= UCTXSTT;             // I2C TX, start condition
    } else { // done with the transmission
      UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
      IE2 &= ~UCB0TXIE;
      __bic_SR_register_on_exit(CPUOFF);// Exit LPM0
    }
  }
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void) {
  if(RX_byte_ctr == 1) {
    UCB0CTL1 |= UCTXSTP | UCTXNACK;
    *RX_byte_ptr = UCB0RXBUF;
    IFG2 &= ~(UCB0TXIFG | UCB0RXIFG);
    __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
  } else {
    *RX_byte_ptr++ = UCB0RXBUF;
  }
  RX_byte_ctr--;
}
