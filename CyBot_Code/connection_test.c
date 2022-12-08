//#include "Components/uart.h"
//#include "Components/lcd.h"
//
//#define LEN 21
//
//extern volatile char receive_data;
//extern volatile int receive_flag;
//
//int main()
//{
//    lcd_init();
//    uart_init();
//
//    int count = 0;
//
//    char byte = 0;
//
//    char str[LEN];
//
//    int i;
//    for(i = 0; i < LEN; ++i)
//    {
//        str[i] = 0;
//    }
//
//    while(1)
//    {
//        //byte = uart_receive();
//
//        if(receive_flag)
//        {
//            receive_flag = 0;
//            byte = receive_data;
//
//            if(count < 20 && byte != '\r')
//            {
//                uart_sendChar(byte);
//
//                str[count] = byte;
//                count++;
//            }
//            else
//            {
//                uart_sendStr("\n\r");
//
//                lcd_printf("%s", str);
//
//                for(i = 0; i < LEN; ++i)
//                {
//                    str[i] = 0;
//                }
//
//                count = 0;
//            }
//
//            // Reset interrupt
//            UART1_ICR_R |= 0x10;
//        }
//
//
//    }
//
//    return 0;
//}
