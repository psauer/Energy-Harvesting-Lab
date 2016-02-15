/*
 * rfModule.c
 *
 *  Created on: Jan 27, 2016
 *      Author: paul
 */

#include <msp430g2553.h>
#include <stdio.h>
#include "rfModule.h"
#include "spi.h"
#include "config.h"

//#include "uart.h"

//==========================NRF24L01============================================
#define TX_ADR_WIDTH    5     // 5 uints TX address width
#define RX_ADR_WIDTH    5     // 5 uints RX address width
#define TX_PLOAD_WIDTH  0x01    // 32 TX payload
//****************************************************************//
// SPI(nRF24L01) commands
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define REGISTER_MASK 0x1F
#define ACTIVATE      0x50
#define R_RX_PL_WID   0x60
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define W_ACK_PAYLOAD 0xA8
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define NOP           0xFF

static uint8_t  tx_address[TX_ADR_WIDTH]= {0xaa,0xaa,0xaa,0xaa,0xaa}; //TX address

static uint8_t flush_tx(void) {
  uint8_t status;

  SET_CS_LOW;           // CSN low, initialize SPI communication...
  status = SPI_transfer( FLUSH_TX );    // Select register to write to
  SET_CS_HIGH;         // CSN high, terminate SPI communication

  return status;
}

static uint8_t reuse(void) {
  uint8_t status;

  rf_read_reg_byte(STATUS, &status);
  rf_write_reg_byte(STATUS, status | MAX_RT);
  SET_CS_LOW;           // CSN low, initialize SPI communication...
  status = SPI_transfer( REUSE_TX_PL );
  SET_CS_HIGH;         // CSN high, terminate SPI communication

  SET_CE_LOW
  __delay_cycles(15);
  SET_CE_HIGH

  return status;
}
/*
static void print_setup(void) {
  uint8_t buffer[6];
  char string[40];

  rf_read_reg_byte(RF_SETUP, buffer);
  sprintf(string, "RF_SETUP = "BYTETOBINARYPATTERN"\n", BYTETOBINARY(buffer[0]));
  uartPutString(string);

  rf_read_reg_byte(RF_CH, buffer);
  sprintf(string, "channel = %d\n", buffer[0]);
  uartPutString(string);

  rf_read_reg(RX_ADDR_P0, buffer, 5);
  sprintf(string, "rx address = 0x%x%x%x%x%x\n", buffer[0], buffer[1],
                                      buffer[2], buffer[3], buffer[4]);
  uartPutString(string);

  rf_read_reg(TX_ADDR, buffer, 5);
  sprintf(string, "tx address = 0x%x%x%x%x%x\n", buffer[0], buffer[1],
                                      buffer[2], buffer[3], buffer[4]);
  uartPutString(string);

  rf_read_reg_byte(RX_PW_P0, buffer);
  sprintf(string, "RX Payload width= 0x%x\n", buffer[0]);
  uartPutString(string);

}*/

void init_rfModule (void) {
  __delay_cycles(5000);

  //setting RF power to be -18dBm, Data rate to 1Mbps
  //byte = (1 << RF_PWR_L) | (1 << RF_PWR_H) | (1 << LNA_HCURR) | (1 << RF_DR);
  rf_write_reg_byte(RF_SETUP, RF_DR_HIGH | RF_PWR_H | RF_PWR_L);

  //setting the TX - RX channel
  rf_write_reg_byte(RF_CH, 76);//choosing channel 76

  //disabling auto ack
  //rf_write_reg_byte(EN_AA, 0x00);

  //enabling only pipe 0
  rf_write_reg_byte(EN_RXADDR, 0x01);

  //setting up tx pipe
  rf_write_reg(RX_ADDR_P0, tx_address, TX_ADR_WIDTH);
  rf_write_reg(TX_ADDR, tx_address, TX_ADR_WIDTH);

  rf_write_reg_byte(RX_PW_P0, TX_PLOAD_WIDTH);

  //setting 1500us delay between retransmissions and 15 retries
  rf_write_reg_byte(SETUP_RETR, ARC_0 | ARC_1 | ARC_2 | ARC_3 | ARD_1);

  //clearing status bits
  rf_write_reg_byte(STATUS, MAX_RT | TX_DS | RX_DR);

  flush_tx();

  //print_setup();

  //setting power up to 1
  rf_write_reg_byte(CONFIG, EN_CRC | PWR_UP | CRCO);
  //delay 5ms
  __delay_cycles(5000);
}

uint8_t rf_write_reg_byte(uint8_t reg, uint8_t data) {
  uint8_t status;

  SET_CS_LOW;           // CSN low, initialize SPI communication...

  status = SPI_transfer( W_REGISTER | reg );    // Select register to write to
  SPI_transfer(data);            // Send data

  SET_CS_HIGH;         // CSN high, terminate SPI communication

  return status;
}

uint8_t rf_write_reg(uint8_t reg, uint8_t* data, uint8_t len) {
  uint8_t status;

  SET_CS_LOW;           // CSN low, initialize SPI communication...

  status = SPI_transfer( W_REGISTER | reg );    // Select register to write to
  while(len--) {
    SPI_transfer(*data++);            // Send data
  }

  SET_CS_HIGH;         // CSN high, terminate SPI communication

  return status;
}

uint8_t rf_write_payload(uint8_t reg, uint8_t* data, uint8_t len) {
  uint8_t status;

  SET_CS_LOW;           // CSN low, initialize SPI communication...

  status = SPI_transfer( reg );    // Select register to write to
  while(len--) {
    SPI_transfer(*data++);            // Send data
  }

  SET_CS_HIGH;         // CSN high, terminate SPI communication

  return status;
}

// Reading registers which 1 byte
uint8_t rf_read_reg_byte(uint8_t reg, uint8_t* buf) {
  uint8_t status;

  SET_CS_LOW;           // CSN low, initialize SPI communication...

  status = SPI_transfer( R_REGISTER | reg );    // Select register to read from
  *buf   = SPI_transfer(NOP);    // Read Register value

  SET_CS_HIGH;         // CSN high, terminate SPI communication

  return status;
}

// Reading registers which are more than 1 byte
uint8_t rf_read_reg(uint8_t reg, uint8_t* buf, uint8_t len) {
  uint8_t status;

  SET_CS_LOW;             // CS low. Init SPI transaction

  status = SPI_transfer(R_REGISTER | reg);    // Select register to write to and read status byte

  while( len-- ) {
    *buf++ = SPI_transfer(NOP);
  }

  SET_CS_HIGH;           // CS high. SPI communication finished
  return status;
}

uint8_t TX_packet (uint8_t * data) {
  uint8_t status;
  char string[50];

  rf_read_reg_byte(FIFO_STATUS, &status);
  //sprintf(string, "FIFO STATUS  = "BYTETOBINARYPATTERN"\n", BYTETOBINARY(status));
  //uartPutString(string);

  rf_write_payload(W_TX_PAYLOAD, data, 1);      // Writes data to TX payload

  rf_read_reg_byte(FIFO_STATUS, &status);
  //sprintf(string, "FIFO STATUS  = "BYTETOBINARYPATTERN"\n", BYTETOBINARY(status));
  //uartPutString(string);

  rf_read_reg_byte(STATUS, &status);
  //sprintf(string, "Status  = "BYTETOBINARYPATTERN"\n", BYTETOBINARY(status));
  //uartPutString(string);

  SET_CE_HIGH
  //10 us delay
  __delay_cycles(85000);
  SET_CE_LOW
  __delay_cycles(1000);

  rf_read_reg_byte(FIFO_STATUS, &status);
  //sprintf(string, "FIFO STATUS2  = "BYTETOBINARYPATTERN"\n", BYTETOBINARY(status));
  //uartPutString(string);

  rf_read_reg_byte(STATUS, &status);
  //sprintf(string, "Status1 = "BYTETOBINARYPATTERN"\n", BYTETOBINARY(status));
  //uartPutString(string);

  while(((status & MAX_RT) != MAX_RT) & ((status & TX_DS) != TX_DS)) {
    __delay_cycles(5);
    rf_read_reg_byte(STATUS, &status);
  }

  //data not successfully sent
  if ((status & MAX_RT) == MAX_RT) {
    rf_write_reg_byte(STATUS, status | MAX_RT);
    flush_tx();
    return 0;
  } else if ((status & TX_DS) == TX_DS) {
    rf_write_reg_byte(STATUS, status | TX_DS);
    return 1;
  }

  /*if (status & TX_FULL) {
    flush_tx();
  }*/
  /*while(~(status & (TX_DS | MAX_RT))) {
    sprintf(string, "Status = "BYTETOBINARYPATTERN"\n", BYTETOBINARY(status));
    uartPutString(string);

    rf_read_reg_byte(STATUS, &status);
    inerDelay_us(50000);
  }*/
    return 0;
}

