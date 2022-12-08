//#include "Components/cybot_select.h"
//#include "Components/IR/IR.h"
//#include "Components/json.h"
//#include "Components/ping.h"
//#include "Components/uart.h"
//#include "Components/lcd.h"
//#include "Components/servo.h"
//#include "Components/OI/open_interface.h"
//
//int get_power(double x)
//{
//    return (((80.0/7.0) * (x - 15)) + 100);
//}
//
//double ping_average(int samples)
//{
//    double sum = 0;
//
//    int i;
//
//    for(i = 0; i < samples; ++i)
//    {
//        sum += ping_distance();
//    }
//
//    return sum / samples;
//}
//
//int main()
//{
//    oi_t* oi = oi_alloc();
//
//    cybot_calib_values_t calib = set_cybot_calibration(3);
//
//    servo_init(calib);
//    ir_init(calib);
//    set_average_amount(64);
//    uart_init();
//    ping_init();
//    lcd_init();
//    lcd_clear();
//    oi_init(oi);
//    timer_waitMillis(1000);
//
//    servo_move(90);
//
//    json_object_t* json = create_object();
//
//    double ping = ping_distance();
//    double ir = calculate_ir_distance(ir_read());
//
//    //char* str = "Hello from json!";
//
//    //add_element(json, STRING, "str", &str);
//
//    add_element(json, NUMBER, "ping_distance", &ping);
//    add_element(json, NUMBER, "ir_distance", &ir);
//
//    char* str = malloc(500);
//
//    while(ping >= 10)
//    {
////        to_string(json, &str, 500);
////        uart_sendStr(str);
////        uart_sendStr("\n\r");
////        ping = ping_distance();
////        ir = calculate_ir_distance(ir_read());
////        lcd_printf("IR: %.3lf\nPing: %.3lf", ir, ping);
////        timer_waitMillis(250);
//
//        ping = ping_average(20);
//        oi_setWheels(get_power(ping), get_power(ping));
//    }
//
//    oi_setWheels(0, 0);
//
//    oi_free(oi);
//
//    return 0;
//}
