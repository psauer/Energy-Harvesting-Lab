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
#include "rfModule.h"
#include "tmp102.h"

static void init_sensors(void) {
  // stop watchdog timer
  WDTCTL = WDTPW | WDTHOLD;
  DCOCTL  = 0;
  BCSCTL1 = CALBC1_1MHZ;   // Set range
  DCOCTL  = CALDCO_1MHZ;   // Set DCO step + modulation

  init_rfModule();
  init_tmp102();

  _BIS_SR(GIE);          // enable interrupts
}

int main(void) {
  int16_t temp;
  uint8_t out_buffer;

  init_sensors();
  temp = tmp102_get_temp();

  // transmitting data
  out_buffer = (uint8_t)temp;

  TX_packet(&out_buffer);

  _BIS_SR(SCG1+SCG0+OSCOFF+CPUOFF);
  return 0;
}
