/*
 * tmp102.h
 *
 *  Created on: Jan 29, 2016
 *      Author: paul
 */

#ifndef TMP102_H_
#define TMP102_H_
#include <inttypes.h>

/* Register Addresses - 2 Bytes */
#define TMP102_TEMP_REG          0x00    /*Read Only*/
#define TMP102_CONFIG_REG        0x01    /*Read/Write*/
#define TMP102_T_LOW_REG         0x02    /*Read/Write*/
#define TMP102_T_HIGH_REG        0x03    /*Read/Write*/

/* Configuration Register Bits */
/* Byte 1 */
#define TMP102_OS                       0x80    /*One Shot temperature measurement mode*/
#define TMP102_R1                       0x40    /*Converstion Resolution bit 1 (Read Only)*/
#define TMP102_R0                       0x20    /*Converstion Resolution bit 0 (Read Only)*/
#define TMP102_F1                       0x10    /*Fault queue bit 1*/
#define TMP102_F0                       0x08    /*Fault queue bit 0*/
#define TMP102_POL                      0x04    /*Polarity of Alert pin*/
#define TMP102_TM                       0x02    /*Thermostat mode*/
#define TMP102_SD                       0x01    /*Shut Down mode*/

/* Byte 2 */
#define TMP102_CR1                      0x80    /*Conversion Rate Bit 1*/
#define TMP102_CR0                      0x40    /*Conversion Rate Bit 0*/
#define TMP102_AL                       0x20    /*Alert Bit (Read Only)*/
#define TMP102_EM                       0x10    /*Extended mode*/

/* Conversion Rates */
#define TMP102_CR_0_25Hz                CR0
#define TMP102_CR_1Hz                   CR1
#define TMP102_CR_4Hz                   CR1           /*default*/
#define TMP102_CR_8Hz                   (CR1 + CR0)

/* Temp = Temp. Register Value * TMP102_CONVERSION */
#define TMP102_CONVERSION               0.0625

void init_tmp102(void);
int tmp102_get_temp(void);

#endif /* TMP102_H_ */
