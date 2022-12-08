//#include "Components/IMU.h"
//#include "Components/lcd.h"
//#include <stdlib.h>
//#include "Components/OI/movement.h"
//#include "Components/ping.h"
//
//int main()
//{
//    lcd_init();
//    init_imu();
//    ping_init();
//    oi_t* oi = oi_alloc();
//    oi_init(oi);
//
//    timer_waitMillis(500);
//
//    //    oi_setWheels(250, 250);
//    //
//    //    while(1)
//    //    {
//    //        if(ping_distance() < 10)
//    //        {
//    //            oi_setWheels(0, 0);
//    //
//    //            timer_waitMillis(250);
//    //
//    //            turn(oi, 90, CLOCKWISE);
//    //
//    //            timer_waitMillis(250);
//    //
//    //            oi_setWheels(250,250);
//    //        }
//    //    }
//
//    //    int i;
//    //    for(i = 0; i < 10; ++i)
//    //    {
//    //        turn(oi, 270, ((i % 2) ? CLOCKWISE : COUNTER_CLOCKWISE));
//    //        timer_waitMillis(500);
//    //    }
//
//    turn_to(oi, 90);
//
//    timer_waitMillis(500);
//
//    turn_to(oi, 330);
//
//    //move(oi, 5000, 250, 1);
//
//    oi_free(oi);
//
//    return 0;
//}
