/*
 *
 *   uart.c
 *
 *
 *
 *
 *
 *   @author
 *   @date
 */

#include <Components/uart.h>

char uart_int_get_data(void)
{
    char result = 0;

    // Enables RX
    UART1_CTL_R |= 0x200;

    result = (char) (UART1_DR_R & 0xFF);

    return result;
}

void receive_handler()
{
    receive_data = uart_int_get_data();
    receive_flag = 1;
}

void uart_init(void)
{
    receive_data = 0;
    receive_flag = 0;

    timer_init();

    // GPIO Setup
    // Start clock for port B
    SYSCTL_RCGCGPIO_R |= 0x2;

    timer_waitMillis(1);

    // Setup alternate function select
    GPIO_PORTB_PCTL_R |= 0x11;
    GPIO_PORTB_AFSEL_R |= 0x3;

    // Enable digital functionality
    GPIO_PORTB_DEN_R |= 0x3;

    // Setup directions
    GPIO_PORTB_DIR_R = 0x2;

    // UART Setup
    // Enable UART 1 Clock
    SYSCTL_RCGCUART_R |= 0x2;

    timer_waitMillis(1);

    // Setup baud rate

    // Disable UART 1
    UART1_CTL_R &= 0xFFFE;

    // Write BRD int
    // BRD = 1.6x10^7 / (16 * 115200) = 8.680555556
    // IBRD = 8 | FBRD = integer(0.680555556 * 64 + 0.5) = 44;
    UART1_IBRD_R = 0x8;
    UART1_FBRD_R = 0x2C;

    // Line Control - 8 bits, 1 stop bit, no parity
    UART1_LCRH_R |= 0x60;

    // Enable UART 1
    UART1_CTL_R |= 0x1;

    // Interrupt Setup
    UART1_IM_R |= 0x10;

    NVIC_EN0_R |= 0x40;

    UART1_ICR_R |= 0x10;

    IntRegister(INT_UART1, receive_handler);

}

void uart_sendChar(char data)
{
    UART1_CTL_R |= 0x100;

    UART1_DR_R = data;

    // Wait for transmit to finish
    while(UART1_FR_R & 0x8)
    {

    }

    //UART1_CTL_R &= 0xFEFF;
}

// This uses interrupts now
char uart_receive_blocking(void)
{
    char result;

    while(!receive_flag && !(UART1_RSR_R & 0x8))
    {

    }

    if(UART1_RSR_R & 0x8)
    {
        // Write to ECR to clear error
        UART1_ECR_R = 0;

        result = 0;
    }
    else
    {
        result = receive_data;
    }

    reset_interrupt();

    return result;
}

void uart_receive_str(char* dest)
{
    char byte = uart_receive_blocking();

    while(byte != 0 && byte != '\r' && byte != '\n' && byte!= '}')
    {
        *dest = byte;
        dest++;

        byte = uart_receive_blocking();
    }

    *dest = '}';

    dest++;

    *dest = 0;
}

void uart_sendStr(const char *data)
{
    while(*data != 0)
    {
        uart_sendChar(*data);
        data++;
    }
}

void reset_interrupt()
{
    UART1_ICR_R |= 0x10;
    receive_flag = 0;
}
