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
static volatile uint8_t repeat_start;
static volatile uint8_t rx_mode;

void init_i2c(uint8_t address) {
  rx_mode = 0;
  // Set UCSWRST
  UCB0CTL1 = UCSWRST;

  // configuring GPIO pins
  P1SEL  |= SCL | SDA;
  P1SEL2 |= SCL | SDA;

  // configuring I2C: I2C Master, synchronous mode
  UCB0CTL0 |= UCMST | UCMODE_3 | UCSYNC;
  UCB0CTL1 |= UCSSEL_2;                    // SMCLK
  UCB0BR0 = 12;                            // fSCL = SMCLK/12 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = address;                     // Set slave address

  // starting state machine
  UCB0CTL1 &= ~UCSWRST;
}

void i2c_write (uint8_t * buff, uint8_t len) {
  TX_byte_ctr = len;
  TX_byte_ptr = buff;
  repeat_start = 0;

  while (UCB0CTL1 & UCTXSTP);      // Ensure stop condition got sent
  UCB0CTL1 |= UCTR + UCTXSTT;      // I2C TX, start condition
  __disable_interrupt();
  IE2 |= UCB0TXIE;
  __bis_SR_register(GIE | CPUOFF); // Enter LPM0 w/ interrupts
}

void i2c_read(uint8_t reg, uint8_t * buff, uint8_t len) {
  uint8_t temp_buff = reg;
  repeat_start = 1;
  RX_byte_ctr = len;
  RX_byte_ptr = buff;

  TX_byte_ctr = 1;
  TX_byte_ptr = &temp_buff;
  while (UCB0CTL1 & UCTXSTP);
  //UCB0CTL1 |= UCTR;
  UCB0CTL1 |= UCTR | UCTXSTT;                     // I2C TX, start condition
  __disable_interrupt();
  IE2 |= UCB0TXIE;
  __bis_SR_register(GIE | CPUOFF);         // Enter LPM0 w/ interrupts
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void) {
  if(rx_mode) {
    if(RX_byte_ctr == 1) {
      *RX_byte_ptr = UCB0RXBUF;
      UCB0CTL1 |= UCTXSTP;                  //I2C stop condition
      IE2 &= ~(UCB0TXIE | UCB0RXIE);        //disable TX and RX interrupts
      rx_mode = 0;                          //clearing flag
      __bic_SR_register_on_exit(CPUOFF);    // Exit LPM0
    } else {
      *RX_byte_ptr++ = UCB0RXBUF;
    }
    RX_byte_ctr--;
  } else {
    if (TX_byte_ctr) {                      // Check TX byte counter
      UCB0TXBUF = *TX_byte_ptr++;           // Load TX buffer
      TX_byte_ctr--;                        // Decrement TX byte counter
    } else {
      if(repeat_start == 1) {               // reading data from register so send start condition
        IE2 &= ~UCB0TXIE;                   // disable tx interrupts
        IE2 |= UCB0RXIE;                    // enable RX interrupts
        UCB0CTL1 &= ~UCTR;                  // configuring to RX mode
        UCB0CTL1 |= UCTXSTT;                // I2C start condition
        rx_mode = 1;
      } else {
        UCB0CTL1 |= UCTXSTP;                // I2C stop condition
        IE2 &= ~UCB0TXIE;                   // disable tx interrupts
        __bic_SR_register_on_exit(CPUOFF);  // Exit LPM0
      }
    }
  }
}
