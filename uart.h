/*
 * uart.h
 *
 *  Created on: Jan 26, 2016
 *      Author: paul
 */

#ifndef UART_H_
#define UART_H_

void init_uart(void);
void uartPutChar(char c);
void uartPutString(char * InputBufferPtr);

#endif /* UART_H_ */
