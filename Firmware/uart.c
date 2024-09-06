/*
 * uart.c
 *
 *  Created on: Mar 17, 2023
 *      Author: afkar.mi
 */

// --------10/04/2023-------------//




#include "uart.h"



uint8_t uart_recv_one_byte(uint16_t *fp)                // Receive one byte data
{
    uint32_t t;
    uint8_t dat;
    for (t = 0; t < UART_RECV_TIMOUT_VALUE; t++) {
        if (UART_RECV_FLAG) {
            dat = UCA1RXBUF;
            *fp = 1;
            break;
        }
    }
    if (t >= UART_RECV_TIMOUT_VALUE) {
        dat = 0xFF;
        *fp = 0;
    }
    return dat;
}



uint8_t uart_recv_data(uint16_t len, uint8_t* buff)     // Receive all data and
{                                                       // and store them in a buffer
    uint16_t i = 0;
    uint16_t flag = 1;
    for (i= 0 ; (i < len) & (flag == 1); i++){
        buff[i] = uart_recv_one_byte(&flag);
    }
    if ((i == len) & (flag == 1)) {
        return 1;   // success
    } else {
        return 0;   // fail
    }
}

void uart_send_data(uint8_t data)
{

    while (!UART_SEND_FLAG);
    UART_SEND_BUF = data;

}

void uart_init(void)
{
    //port mappingsS
      P4SEL |= BIT4+BIT5;                       // P4.4,5 = USCI_A1 TXD/RXD
      UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
      UCA1CTL1 |= UCSSEL_2;                     // SMCLK
      UCA1BR0 = 0x04;                           // 8MHz/115200=4.41 (see User's Guide)
      UCA1BR1 = 0x00;
      UCA1MCTL |= UCOS16 + UCBRS_5 + UCBRF_3;
      UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
      UCA1IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt

}


//-------------------------------UART RX /TX ------------------------------------------------------------//


//-------------------------------Interrupt------------------------------------------------------//
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    // FY - need to close all interrupt
    _DINT();
    uart_recv_data(UART_RECV_LENGTH, uart_recv_buf);
    flag_uart_recv = 1;
   _EINT();
}

//-------------------------------Interrupt END------------------------------------------------------//
