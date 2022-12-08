//#include <Components/cybot_select.h>
//#include <Components/IR/ir_calibration.h>
//#include <Components/IR/IR.h>
//#include <Components/Object_Scan.h>
//#include <Components/OI/movement.h>
//#include <Components/ping.h>
//#include <Components/IMU.h>
//#include <Components/servo.h>
//
//#define STR_LEN 100
//#define ANGLE_CUTOFF 10
//#define DRIVE_POWER 0.3
//#define SCAN_ANGLE_DELTA 90
//
//// Accounts for sensor not being at end of roomba
//#define END_DISTANCE 12
//
//// Clockwise turning: negative
//// Counter-clockwise turning: positive
//int main()
//{
//    cybot_calib_values_t calib = set_cybot_calibration(5);
//
//    oi_t* oi = oi_alloc();
//
//    object_scan_alloc();
//
//    object_t target;
//
//    int start_angle = 180;
//    int end_angle = 0;
//
//    // Dummy value
//
//    int target_angle = 0;
//
//    int auto_drive_distance = 100;
//
//    int tele_drive_distance = 100;
//
//    int tele_turn = 10;
//
//    int current_angle = 0;
//
//    int auto_finished = 1;
//
//    int tele_op = 1;
//
//    int bump_left = 0;
//
//    int bump_right = 0;
//
//    int byte = 0;
//
//    char* str = malloc(STR_LEN);
//
//    char* const str_start = str;
//
//    int i;
//    for(i = 0; i < STR_LEN; ++i)
//    {
//        *str = 0;
//        str++;
//    }
//
//    str = str_start;
//
//    uart_init();
//    lcd_init();
//    ping_init();
//    imu_init();
//    ir_init(calib);
//    set_average_amount(64);
//    servo_init(calib);
//
//    timer_waitMillis(1);
//
//    oi_init(oi);
//
//    servo_move(90);
//
//    timer_waitMillis(2000);
//
//    scan(180, 0, 1);
//
//    timer_waitMillis(20000);
//
//    while(1)
//    {
//
//        byte = uart_receive_blocking();
//
//        if(tele_op)
//        {
//            switch(byte)
//            {
//
//            case 'c':
//            {
//                servo_calib(1);
//
//                break;
//            }
//
//            case 'z':
//            {
//                ir_calibrate(calib);
//                break;
//            }
//
//            case 'p':
//            {
//                servo_move(90);
//
//                sprintf(str, "Read an IR distance of: %.3f ::: Read a sound distance of: %.3f\n\r", calculate_ir_distance(ir_read()), ping_distance());
//                uart_sendStr(str);
//
//                break;
//            }
//
//            case 'i':
//            {
//                sprintf(str, "Current Angle: %d ::: Current Target Angle %d\n\rCurrent Turn Radius: %d ::: Current Drive Distance: %d\n\r",
//                        current_angle, target_angle, tele_turn, tele_drive_distance);
//                uart_sendStr(str);
//
//                break;
//            }
//
//            case 'm':
//            {
//                start_angle = 180;
//                end_angle = 0;
//                farthest_distance = 30;
//
//                target = get_smallest_object(scan(start_angle, end_angle, 1));
//
//                if(target.distance != -1)
//                {
//                    target_angle = target.median_angle - 90;
//
//                    sprintf(str, "Target Angle: %d\n\r", target_angle);
//
//                    uart_sendStr(str);
//                }
//                else
//                {
//                    uart_sendStr("No objects detected :(\n\r");
//                }
//
//                break;
//            }
//
//            case 'w':
//            {
//                //                    bump_left = 0;
//                //                    bump_right = 0;
//                //
//                //                    move_collision(oi, tele_drive_distance, DRIVE_POWER, 0, 0, &bump_left, &bump_right);
//                //
//                //                    if(bump_left)
//                //                    {
//                //                        uart_sendStr("Left bump sensor hit!\n\r");
//                //                    }
//                //
//                //                    if(bump_right)
//                //                    {
//                //                        uart_sendStr("Right bump sensor hit!\n\r");
//                //                    }
//
//                move(oi, tele_drive_distance, 250, 1);
//
//                break;
//            }
//
//            case 's':
//            {
//                move(oi, tele_drive_distance, 250, -1);
//                break;
//            }
//            case 'd':
//            {
//                turn(oi, tele_turn, CLOCKWISE);
//
//                current_angle -= tele_turn;
//
//                break;
//            }
//            case 'a':
//            {
//                turn(oi, tele_turn, COUNTER_CLOCKWISE);
//
//                current_angle += tele_turn;
//
//                break;
//            }
//            case '[':
//            {
//                tele_drive_distance -= 10;
//
//                sprintf(str, "Distance is now: %d\n\r", tele_drive_distance);
//
//                uart_sendStr(str);
//
//                break;
//            }
//
//            case ']':
//            {
//                tele_drive_distance += 10;
//
//                sprintf(str, "Distance is now: %d\n\r", tele_drive_distance);
//
//                uart_sendStr(str);
//
//                break;
//            }
//
//            case '-':
//            {
//                tele_turn -= 5;
//
//                sprintf(str, "Turn angle now %d\n\r", tele_turn);
//
//                uart_sendStr(str);
//
//                break;
//            }
//
//            case '+':
//            {
//                tele_turn += 5;
//
//                sprintf(str, "Turn angle now %d\n\r", tele_turn);
//
//                uart_sendStr(str);
//
//                break;
//            }
//
//            }
//        }
//        else
//        {
//            switch(byte)
//            {
//
//            case 'm':
//            {
//                reset_interrupt();
//
//                start_angle = 180;
//                end_angle = 0;
//
//                // Start auto
//                auto_finished = 0;
//
//                while(!auto_finished && !(receive_flag && byte == 't'))
//                {
//                    target = get_smallest_object(scan(start_angle, end_angle, 0));
//
//                    if(target.distance != -1)
//                    {
//                        target_angle = target.median_angle - 90;
//
//                        sprintf(str, "Target Angle: %d\n\r", target_angle);
//
//                        uart_sendStr(str);
//                    }
//                    else
//                    {
//                        uart_sendStr("No objects detected :(\n\r");
//                        auto_finished = 1;
//                        break;
//                    }
//
//                    auto_drive_distance = (target.distance / 3) * 10;
//
//                    // Needs to turn counter-clockwise
//                    if(target_angle > ANGLE_CUTOFF)
//                    {
//                        turn(oi, target_angle, COUNTER_CLOCKWISE);
//                    }
//                    // Needs to turn clockwise
//                    else if(target_angle < -ANGLE_CUTOFF)
//                    {
//                        turn(oi, target_angle, CLOCKWISE);
//                    }
//
//                    if(target.distance <= END_DISTANCE)
//                    {
//                        uart_sendStr("At closest object :D\n\r");
//                        auto_finished = 1;
//                        break;
//                    }
//
//                    if((start_angle - end_angle) > SCAN_ANGLE_DELTA)
//                    {
//                        start_angle -= SCAN_ANGLE_DELTA / 2;
//                        end_angle += SCAN_ANGLE_DELTA / 2;
//                    }
//
//                    move_collision(oi, auto_drive_distance, DRIVE_POWER, 1, target_angle, &bump_left, &bump_right);
//
//                    // Widen scan-cone if bumped into object
//                    if((bump_left || bump_right) && start_angle < 180)
//                    {
//                        start_angle += SCAN_ANGLE_DELTA / 2;
//                        end_angle -= SCAN_ANGLE_DELTA / 2;
//                    }
//
//                    bump_left = 0;
//                    bump_right = 0;
//                }
//
//                if(receive_flag && byte == 't')
//                {
//                    tele_op = 1;
//
//                    sprintf(str, "Stopped auto! Transitioned to tele-op!\n\r");
//                    uart_sendStr(str);
//
//
//                }
//
//                break;
//            }
//
//            }
//        }
//
//        switch(byte)
//        {
//        case 't':
//        {
//            tele_op ^= 1;
//
//            sprintf(str, "Tele Op: %d\n\r", tele_op);
//            uart_sendStr(str);
//            break;
//        }
//        case 'x':
//        {
//            oi_free(oi);
//            return 0;
//        }
//        }
//
//
//
//    }
//
//
//
//}
