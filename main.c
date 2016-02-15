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

  //init_tmp102();
  //init_uart();
  init_rfModule();

  _BIS_SR(GIE);          // enable interrupts
}

void main(void) {
  int16_t temp;
  char string[40];

  init_sensors();
  //temp = tmp102_get_temp();
  //sprintf(string, "temperature = %d\n", temp);
  //uartPutString(string);


  // transmitting data
  out_buffer = 0x00;

  // loop forever
  while (1) {
    // delay for a while
    __delay_cycles(500000);

    status = TX_packet(&out_buffer);
    if(status == 0x00) {
      sprintf(string, "Tx failed\n");
      uartPutString(string);
    } else {
      sprintf(string, "Tx!!!\n");
      uartPutString(string);
    }
    out_buffer++;
    if (out_buffer == 255) {
      out_buffer = 0;
    }
  }

}
