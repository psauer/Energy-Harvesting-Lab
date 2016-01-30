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
//#include "rfModule.h"
//#include "tmp102.h"

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

static void init_sensors(void) {
  // stop watchdog timer
  WDTCTL = WDTPW | WDTHOLD;
  DCOCTL  = 0;
  BCSCTL1 = CALBC1_1MHZ;   // Set range
  DCOCTL  = CALDCO_1MHZ;   // Set DCO step + modulation

  //init functions
  init_uart();
  //init_rfModule();
  //init_tmp102();

  //_BIS_SR(LPM0_bits | GIE); // enable interrupts & set to low power
  _BIS_SR(GIE);          // enable interrupts
}

void main(void) {
  volatile int i;
  uint8_t out_buffer[3], status;

  init_sensors();

  uartPutString("status: ", 9);
  print_binary(status);
  uartPutChar(0x0a);//new line



  // loop forever
  while (1) {
    for (i = 0; i < 0x600000; i++);
    // delay for a while
    //uartPutString("Paul", 4);
  }
}
