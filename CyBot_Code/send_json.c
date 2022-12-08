//#include "Components/lcd.h"
//#include "Components/uart.h"
//#include "Components/json.h"
//#include <stdlib.h>
//
//int main()
//{
//    lcd_init();
//    uart_init();
//    timer_init();
//
//    json_object_t* root = create_object();
//
//    char* str = "Hello World!";
//
//    char* output = malloc(100);
//
//    int i;
//    for(i = 0; i < 100; ++i)
//    {
//        output[i] = 0;
//    }
//
//    add_element(root, STRING, "message", &str);
//
//    to_string(root, &output, 100);
//
////    while(1)
////    {
////        uart_sendStr(output);
////
////        timer_waitMillis(1000);
////    }
//
//    return 0;
//}
