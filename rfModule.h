/*
 * rfModule.h
 *
 *  Created on: Jan 27, 2016
 *      Author: paul
 */

#ifndef RFMODULE_H_
#define RFMODULE_H_

#include <inttypes.h>

//***************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address

 // CONFIG register bits
#define EN_CRC          0x08
#define CRCO            0x04
#define PWR_UP          0x02

/// RF_SETUP register bits
#define RF_DR           0x08
#define RF_PWR_H        0x04
#define RF_PWR_L        0x02

 // STATUS register bits

#define RX_DR           0x40
#define TX_DS           0x20
#define MAX_RT          0x10
#define TX_FULL         0x01

//SETUP_RETR
#define ARD_3           0x08
#define ARD_2           0x04
#define ARD_1           0x02
#define ARD_0           0x01
#define ARC_3           0x08
#define ARC_2           0x04
#define ARC_1           0x02
#define ARC_0           0x01

void init_rfModule (void);
uint8_t rf_write_reg_byte(uint8_t reg, uint8_t data);
uint8_t rf_write_reg(uint8_t reg, uint8_t* data, uint8_t len);
uint8_t rf_read_reg_byte(uint8_t reg, uint8_t* buf);
uint8_t rf_read_reg(uint8_t reg, uint8_t* buf, uint8_t len);
uint8_t TX_packet (uint8_t * data);


#endif /* RFMODULE_H_ */
