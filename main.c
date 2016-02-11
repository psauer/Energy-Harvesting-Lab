/*
 * main.c
 *
 *  Created on: Jan 25, 2016
 *      Author: paul
 */

#include <msp430g2553.h>
#include <inttypes.h>
#include <stdio.h>
#include "uart.h"
#include "config.h"
//#include "rfModule.h"
#include "tmp102.h"

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
  char string[40];
  // stop watchdog timer
  WDTCTL = WDTPW | WDTHOLD;
  DCOCTL  = 0;
  BCSCTL1 = CALBC1_1MHZ;   // Set range
  DCOCTL  = CALDCO_1MHZ;   // Set DCO step + modulation

  //init functions
  init_uart();

  //init_rfModule();
  init_tmp102();

  _BIS_SR(GIE);          // enable interrupts
}

void main(void) {
  int16_t temp;
  char string[40];

  init_sensors();

  //__delay_cycles(500000);
  temp = tmp102_get_temp();
  sprintf(string, "temperature = %d\n", temp);
  uartPutString(string);

}
