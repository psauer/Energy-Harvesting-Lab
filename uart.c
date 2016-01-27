/*
 * uart.c
 *
 *  Created on: Jan 26, 2016
 *      Author: paul
 */
#include <msp430g2553.h>
#include <inttypes.h>
#include "config.h"

#define RXD BIT1
#define TXD BIT2

void init_uart(void) {
  P1SEL  = RXD + TXD;
  P1SEL2 = RXD + TXD;
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 104;                            // 1MHz 9600
  UCA0BR1 = 0;                              // 1MHz 9600
  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST;                     // Initialize USCI state machine
  //IE2 |= UCA0TXIE; // enable TX interrupts
}

void uartPutChar(uint8_t c) {
  volatile int i;
  //delay to make text constant
  for (i = 0; i < 0x6; i++);

  while (!(IFG2 & UCA0TXIFG));
  UCA0TXBUF = c;
}

void uartPutString(char * InputBufferPtr, uint32_t NumBytes) {
  uint32_t i;

  for (i = 0; i < NumBytes; i++) {
    uartPutChar(InputBufferPtr[0]);
    InputBufferPtr++;
  }
  uartPutChar(0x0a);//new line
  P1OUT ^= LED2;
}


