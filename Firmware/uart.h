/*
 * uart.h
 *
 *  Created on: Mar 17, 2023
 *      Author: afkar.mi
 */

#ifndef UART_H_
#define UART_H_

#include <msp430.h>
#include "driverlib.h"
#include <stdio.h>
#include <stdlib.h>

#define UART_RECV_TIMOUT_VALUE      0xFBBB
#define UART_RECV_LENGTH            7                   // {PositionByte, Motor Selection, Direction, 4 Steps Byte}

#define UART_RECV_FLAG              (UCA1IFG & UCRXIFG)
#define UART_RECV_BUF               UCA1RXBUF           // Receive buffer
#define UART_SEND_FLAG              (UCA1IFG & UCTXIFG)
#define UART_SEND_BUF               UCA1TXBUF           // Transmission buffer

extern uint8_t uart_recv_buf[UART_RECV_LENGTH];         // Buffer to store data received
extern uint8_t flag_uart_recv;

uint8_t uart_recv_one_byte(uint16_t *fp);
uint8_t uart_recv_data(uint16_t len, uint8_t* buff);
void uart_init(void);                                   //Uart config
void uart_send_data(uint8_t data);                      // Send data from MCU

#endif /* UART_H_ */
