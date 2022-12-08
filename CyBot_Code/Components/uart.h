/*
*
*   uart.h
*
*   Used to set up the UART
*   uses UART1 at 115200
*
*
*   @author Dane Larson
*   @date 07/18/2016
*   Phillip Jones updated 9/2019, removed WiFi.h
*/

#ifndef COMPONENTS_UART_H_
#define COMPONENTS_UART_H_

#include <Components/Timer.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

volatile char receive_data;
volatile int receive_flag;

void uart_init(void);

void uart_sendChar(char data);

char uart_receive_blocking(void);

void uart_sendStr(const char *data);

void uart_receive_str(char* dest);

void reset_interrupt();


#endif /* COMPONENTS_UART_H_ */
