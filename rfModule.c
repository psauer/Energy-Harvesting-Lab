/*
 * rfModule.c
 *
 *  Created on: Jan 27, 2016
 *      Author: paul
 */

#include <msp430g2553.h>
#include "rfModule.h"
#include "spi.h"
#include "config.h"

//==========================NRF24L01============================================
#define TX_ADR_WIDTH    5     // 5 uints TX address width
#define RX_ADR_WIDTH    5     // 5 uints RX address width
#define TX_PLOAD_WIDTH  3    // 32 TX payload
#define RX_PLOAD_WIDTH  3    // 32 uints TX payload
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

static uint8_t  tx_address[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01}; //TX address

static void inerDelay_us(int n) {
  for(;n>0;n--);
}

void init_rfModule (void) {
  uint8_t byte;
//TODO check status byte to see if == TX_FULL

  //setting RF power to be -18dBm, Data rate to 1Mbps
  //byte = (1 << RF_PWR_L) | (1 << RF_PWR_H) | (1 << LNA_HCURR) | (1 << RF_DR);
  byte = (1 << RF_PWR_L) | (1 << RF_PWR_H) | (1 << LNA_HCURR);
  rf_write_reg_byte(RF_SETUP, byte);

  //setting the TX - RX channel
  rf_write_reg_byte(RF_CH, CHANNEL);//choosing channel 76

  //setting up tx pipe
  rf_write_reg(RX_ADDR_P0, tx_address, TX_ADR_WIDTH);
  rf_write_reg(TX_ADDR, tx_address, TX_ADR_WIDTH);
  rf_write_reg_byte(RX_PW_P0, RX_PLOAD_WIDTH);

  //setting power up to 1
  byte = (1 << EN_CRC) | (1 << PWR_UP);
  rf_write_reg_byte(CONFIG, byte);
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

void TX_packet (uint8_t * data) {

  SET_CE_LOW

  rf_write_reg(W_TX_PAYLOAD, data, TX_PLOAD_WIDTH);      // Writes data to TX payload

  //SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);        // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..

  SET_CE_HIGH
  //TODO insert wait here. min 10 us or interupt checking
  inerDelay_us(100);
  SET_CE_LOW
}

