#ifndef COMPONENTS_UART_H_
#define COMPONENTS_UART_H_

#include <Components/Timer.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

// ISR volatile variables to store UART received bytes and the flag
// to notify the other functions when a byte has been received.
volatile char receive_data;
volatile int receive_flag;


// Inits the UART.
void uart_init(void);

// Sends the provided character/byte.
void uart_sendChar(char data);

// Receives a char/byte. Waits until the char/byte is received.
char uart_receive_blocking(void);

// Sends a string.
void uart_sendStr(const char *data);

// Receives a string. Currently uses the following chars as the end of the string:
// null byte, \n, \r, and } for JSON.
void uart_receive_str(char* dest);

// Resets interrupts using the clear register. Also resets the flag variable.
void reset_interrupt();

#endif /* COMPONENTS_UART_H_ */
