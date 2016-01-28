/*
 * main.c
 *
 *  Created on: Jan 25, 2016
 *      Author: paul
 */

#include <msp430g2553.h>
#include <inttypes.h>
#include "uart.h"
#include "config.h"
#include "rfModule.h"

//******************************************************************************************
// Delay for about n us
//******************************************************************************************
static void inerDelay_us(int n) {
  for(;n>0;n--);
}

static void print_binary(uint8_t n) {
  int i;
  uint8_t mask = 1 << 7;

  for(i = 0; i < 8; i++) {
       if((n & (mask >> i)) != 0) {
         uartPutChar('1');
       } else {
         uartPutChar('0');
       }
  }
}

void main(void) {
  volatile int i;
  // stop watchdog timer
  WDTCTL = WDTPW | WDTHOLD;
  DCOCTL  = 0;
  BCSCTL1 = CALBC1_1MHZ;   // Set range
  DCOCTL  = CALDCO_1MHZ;   // Set DCO step + modulation

  /*
  // set up GPIO pins P1.0 & P1.6 as output pins
  P1DIR |= LED1 | LED2;
  // Setting P1.0 & P1.6 to low
  P1OUT &= ~(LED1 | LED2);*/

  //init functions
  init_uart();
  init_spi();
  init_rfModule();

  //_BIS_SR(LPM0_bits | GIE); // enable interrupts & set to low power
  _BIS_SR(GIE);          // enable interrupts

  uint8_t out_buffer[2];

  // read response
  SPI_read(CONFIG, out_buffer);

  uartPutString("status: ", 9);
  print_binary(out_buffer[0]);
  uartPutChar(0x0a);//new line

  uartPutString("config: ", 8);
  print_binary(out_buffer[1]);
  uartPutChar(0x0a);//new line

  // loop forever
  while (1) {
    for (i = 0; i < 0x6000; i++);
    // delay for a while
    //uartPutString("Paul", 4);
  }
}
