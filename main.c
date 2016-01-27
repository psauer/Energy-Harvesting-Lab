/*
 * main.c
 *
 *  Created on: Jan 25, 2016
 *      Author: paul
 */


//Demo app to blink the red LED (LED1) on the TI Launchpad
//which is attached to P1.0
//The green LED(LED2) is connected to P1.6

#include <msp430g2553.h>
#include <inttypes.h>
#include "uart.h"
#include "config.h"

int main(void) {
volatile int i;
  // stop watchdog timer
  WDTCTL = WDTPW | WDTHOLD;
  DCOCTL  = 0;
  BCSCTL1 = CALBC1_1MHZ;   // Set range
  DCOCTL  = CALDCO_1MHZ;   // Set DCO step + modulation

  // set up bit 0 of P1 as output
  P1DIR |= LED1 | LED2;
  // intialize bit 0 of P1 to 0
  P1OUT &= ~(LED1 | LED2);

  init_uart();

  //_BIS_SR(GIE);          // interrupt

  // loop forever
  while (1) {
    for (i = 0; i < 0x6000; i++);
    // delay for a while
    uartPutString("Paul", 4);
  }
}
