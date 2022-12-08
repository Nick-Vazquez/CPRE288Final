#include <Components/object_scan.h>
#include "Components/OI/movement.h"
#include "Components/servo.h"
#include "Components/Timer.h"
#include "Components/cybot_select.h"
#include "Components/ping.h"
#include "Components/IR/IR.h"
#include "Components/OI/cliff_sensors.h"
#include "Components/uart.h"
#include "Components/json.h"
#include "Components/OI/song.h"
#include "Components/jsmn.h"

extern float farthest_distance;

// In cm
#define MIN_DIST 15.0

// In mm
// This is the distance between the ping sensor and the center of the CyBot
// which is the CLEAN button
#define PING_CENTER_DIST 100

// In mm
#define BACKWARDS_CORRECT 0
#define ROOM_ENTRY_DIST 80

#define DRIVE_DIST_FACTOR (2.0/3.0)
#define SIDE_WALL_THRESHOLD 20.0
#define SAMPLES 100
#define DRIVE_POWER 200

// This is the extra distance that will be driven after the move
// forward to the end the gap. This is so the CyBot does not start at
// the end of the gap. If the CyBot starts at the end of the gap,
// the ping sensor may still be reading the inside of the gap.
// The ensures that the main hallway wall will be read.
#define GAP_EXTRA_DIST 20

// In ms
#define MOVE_DELAY 500
#define GAP_TIME 500
#define SERVO_DELAY 750

// Malloc sizes
#define MAX_STRING_SIZE 100

#define MAX_TOKENS 100

// Relative to starting position
// Value assignment used in ternary operators
// Syntax wall_dir ? <when LEFT> : <when RIGHT>
typedef enum wall_dir {LEFT = 1, RIGHT = 0} wall_dir_t;

// These are only messages received
typedef enum message_types
{UNKNOWN_MESSAGE = 0, OP_MODE_SEL = 1, MOVE = 2, SCAN = 3, PLAY_SOUND = 6, SENSOR_SEL = 7} message_types_t;
typedef enum op_modes
{AUTO, TELEOP} op_modes_t;

double highest_ping(int sample_count)
{
    double highest = 0;
    double dist = 0;
    int i;

    for(i = 0; i < sample_count; ++i)
    {
        dist = ping_distance();

        if(dist > highest)
        {
            highest = dist;
        }
    }

    return highest;
}

double ping_dist_avg(int sample_count)
{
    double sum = 0.0;

    int i;
    for(i = 0; i < sample_count; ++i)
    {
        sum += ping_distance();
    }

    return sum / sample_count;
}

double ir_dist_avg(int sample_count)
{
    double sum = 0.0;

    int i;
    for(i = 0; i < sample_count; ++i)
    {
        sum += calculate_ir_distance(ir_read());
    }

    return sum / sample_count;
}

double get_back_wall_dist(int sample_count)
{
    servo_move(90.0);
    timer_waitMillis(SERVO_DELAY);

    double ping = highest_ping(sample_count);
    double ir = ir_dist_avg(sample_count);

    // This is so that if the IR reading is low enough, the IR is used.
    // This removes the issue of the ping spiking at short distances

    // The > 9 is to prevent bad super small readings
    if(ir >= 9 && ir <= 50)
    {
        return ir;
    }
    else
    {
        return ping;
    }
}

double get_side_wall_dist(int sample_count)
{
    servo_move(180.0);
    timer_waitMillis(SERVO_DELAY);

    return ir_dist_avg(sample_count);
}

// Backwards driving has be inconsistent, this will turn and then drive forward
void backwards_drive(oi_t* oi, float current_heading, int dist, float power)
{
    turn_to(oi, current_heading + 180);
    timer_waitMillis(MOVE_DELAY);
    move(oi, current_heading + 180, dist, power, FORWARD);
    timer_waitMillis(MOVE_DELAY);
    turn_to(oi, current_heading);
}

// Globals used in alloc and free functions
oi_t* oi;

// Stores parsed json strings
char* jsmn_data;

char* uart_rec;

// JSON globals
json_object_t* scan_thing;
json_object_t* single_scan;

// JSMN globals
jsmn_parser parser;

jsmntok_t* tokens;

// This will get the message type so that we can parse correctly
message_types_t get_message_type(char* js)
{
    char temp[10];
    int result = 0;

    clear_tokens(tokens, MAX_TOKENS);

    jsmn_parse(&parser, js, strlen(js), tokens, MAX_TOKENS);
    jsmn_init(&parser);

    get_jsmn_data(temp, "mes_type", js, tokens, MAX_TOKENS);

    sscanf(temp, "%d", &result);

    return (message_types_t) result;
}

void clear_string(char* str)
{
    // If the first char is a null byte, then the string is
    // treated as empty

    *str = 0;
}

void final_alloc()
{
    oi = oi_alloc();
    object_scan_alloc();
    tokens = malloc(sizeof(jsmntok_t) * MAX_TOKENS);
    jsmn_data = malloc(MAX_STRING_SIZE);
    uart_rec = malloc(MAX_STRING_SIZE);
    jsmn_init(&parser);
}

void final_free()
{
    oi_free(oi);
    object_scan_free();
    imu_free();
    free(jsmn_data);
    free(uart_rec);
    free(tokens);
}

void init()
{
    cybot_calib_values_t calib = set_cybot_calibration(5);

    final_alloc();

    uart_init();
    lcd_init();
    ping_init();
    imu_init();
    ir_init(calib);
    set_average_amount(64);
    servo_init(calib);

    timer_waitMillis(1);

    oi_init(oi);
}

int main()
{
    int dist_target = 0;
    wall_dir_t wall_dir = LEFT;
    op_modes_t op_mode;

    // This is used to keep track of the distance moved while NOT in the gap detection loop
    // Whenever oi->distance is used, we just desire the relative to the current location
    double dist_current = 0;

    double side_wall_dist = 0;
    double back_wall_dist = 0;
    double dist_reading = 0;
    int auto_finished = 0;

    init();

    timer_waitMillis(1000);

    // Main program loop here

    // Wait for autonomous start

    //lcd_printf("Waiting for command...");

    //uart_receive_str(uart_rec);

    //strcpy(uart_rec, "{\"mes_type\": 1, \"op_mode\": \"AUTO\"}");

    //lcd_printf("Command received!");

    //timer_waitMillis(1000);

    //strcpy(uart_rec, "{\"mes_type\": 1, \"op_mode\": \"TELEOP\"}");

    //lcd_printf("%s", uart_rec);

    //timer_waitMillis(2000);

//    if(get_message_type(uart_rec) == OP_MODE_SEL)
//    {
//        // Determine op mode
//        clear_tokens(tokens, MAX_TOKENS);
//        jsmn_parse(&parser, uart_rec, strlen(uart_rec), tokens, MAX_TOKENS);
//        jsmn_init(&parser);
//
//        clear_string(jsmn_data);
//
//        get_jsmn_data(jsmn_data, "op_mode", uart_rec, tokens, MAX_TOKENS);
//
//        if(!strcmp("AUTO", jsmn_data))
//        {
//            lcd_printf("Autonomous was selected!");
//            op_mode = AUTO;
//
//        }
//        else
//        {
//            lcd_printf("Tele-Op was selected!");
//            op_mode = TELEOP;
//        }
//    }

    op_mode = AUTO;

    if(op_mode == AUTO)
    {

        // lcd_printf("Side Wall: %.3lf", side_wall_dist);

        while(!auto_finished)
        {
            // Get wall dist
            side_wall_dist = get_side_wall_dist(SAMPLES);
            timer_waitMillis(SERVO_DELAY);
            back_wall_dist = get_back_wall_dist(SAMPLES);

            lcd_printf("Back wall dist: %.3lf", back_wall_dist);

            timer_waitMillis(SERVO_DELAY);

            dist_current = 0;

            if(back_wall_dist > MIN_DIST)
            {
                // Multiplied by 10 to convert from CM to MM
                dist_target = (back_wall_dist * 10) * DRIVE_DIST_FACTOR;

                // Drive loop
                while(dist_current < dist_target)
                {
                    // Point servo at side wall
                    servo_move(180.0);
                    timer_waitMillis(SERVO_DELAY);

                    dist_reading = ir_dist_avg(SAMPLES);

                    lcd_printf("Thresh: %.3lf\nCurrent: %.3lf", side_wall_dist + SIDE_WALL_THRESHOLD, dist_reading);

                    if(dist_reading >= side_wall_dist + SIDE_WALL_THRESHOLD)
                    {

                        dist_reading = ir_dist_avg(SAMPLES);

                        // Room detected, determine room width
                        int dist_gap = 0;
                        int dist_drive = 0;

                        // This is to keep track of a start time
                        int time_start = timer_getMillis();

                        // GAP_TIME is used here to ensure that GAP_TIME time has passed since the edge detection.
                        // This is so at the point of edge detection, if there are noisy readings, this conditional won't be
                        // satisfied right away.
                        while(dist_reading >= side_wall_dist + SIDE_WALL_THRESHOLD || (timer_getMillis() - time_start) < GAP_TIME)
                        {
                            // Continue drive
                            set_drive_power(wall_dir ? 0.0 : 180.0, DRIVE_POWER);
                            oi_update(oi);
                            dist_gap += oi->distance;
                            dist_reading = ir_dist_avg(SAMPLES);
                            lcd_printf("Thresh: %.3lf\nCurrent: %.3lf\nIn gap!", side_wall_dist + SIDE_WALL_THRESHOLD, dist_reading);
                        }

                        // Stop once at end of gap
                        oi_setWheels(0.0, 0.0);
                        timer_waitMillis(MOVE_DELAY);

                        // This will get the distance needed to drive such that the center of the CyBot
                        // is in the middle of the gap
                        dist_drive = (dist_gap - PING_CENTER_DIST) - (dist_gap / 2);

                        // Drive backwards to the middle of the gap
                        backwards_drive(oi, wall_dir ? 0.0 : 180.0, dist_drive, DRIVE_POWER);
                        timer_waitMillis(MOVE_DELAY);

                        // Turn towards gap
                        turn_to(oi, wall_dir ? 270.0 : 90.0);
                        timer_waitMillis(MOVE_DELAY);

                        // Cliff drive
                        int dist_drive_cliff = 0;
                        int dist_target_cliff = ROOM_ENTRY_DIST + side_wall_dist + PING_CENTER_DIST;
                        int cliff = get_cliff(oi);

                        while(dist_drive_cliff < dist_target_cliff && !cliff)
                        {
                            set_drive_power(wall_dir ? 270.0 : 90.0, 200);

                            // get_cliff(oi) executes an oi_upate(oi) command
                            cliff = get_cliff(oi);
                            dist_drive_cliff += oi->distance;
                        }

                        oi_setWheels(0.0, 0.0);
                        timer_waitMillis(MOVE_DELAY);

                        if(cliff)
                        {
                            lcd_printf("Cliff Detected!");
                            oi_setWheels(0.0, 0.0);
                            timer_waitMillis(MOVE_DELAY);

                            // Traditional move used since cannot turn when cliff is detected
                            // since a turn will put it in the hole.
                            move(oi, wall_dir ? 270.0 : 90.0, (ROOM_ENTRY_DIST + side_wall_dist - dist_drive_cliff + PING_CENTER_DIST), DRIVE_POWER, BACKWARD);
                            timer_waitMillis(MOVE_DELAY);
                        }
                        else
                        {

                            float short_distance = get_smallest_average_distance(160, 20, 2);
                            float exit_room_dist = ROOM_ENTRY_DIST + side_wall_dist + PING_CENTER_DIST;

                            timer_waitMillis(SERVO_DELAY);

                            while(short_distance > 15)
                            {
                                move(oi, wall_dir ? 270.0 : 90.0, (short_distance * 10 * DRIVE_DIST_FACTOR), 75, FORWARD);
                                timer_waitMillis(MOVE_DELAY);

                                // Add extra distance driven forward
                                exit_room_dist += (short_distance * 10 * DRIVE_DIST_FACTOR);

                                short_distance = get_smallest_average_distance(160, 20, 2);

                                lcd_printf("Short distance: %.3f", short_distance);
                                timer_waitMillis(1000);
                            }

                            timer_waitMillis(SERVO_DELAY);
                            //farthest_distance = get_largest_average_distance(160, 20, 10) - 3;

                            //lcd_printf("Large dist: %.3lf\nShort dist: %.3f", farthest_distance, short_distance);

                            //timer_waitMillis(SERVO_DELAY);
                            farthest_distance = 20;

                            objects_data_t data = scan(160, 20, 0);
                            lcd_printf("Object count: %d", data.object_count);
                            timer_waitMillis(1000);
                            // Back out
                            backwards_drive(oi, wall_dir ? 270.0 : 90.0, exit_room_dist, DRIVE_POWER);
                            timer_waitMillis(MOVE_DELAY);
                        }


                        // Turn back
                        turn_to(oi, wall_dir ? 0.0 : 180.0);
                        timer_waitMillis(MOVE_DELAY);

                        lcd_clear();

                        // Drive back, plus a little more to ensure that the ping is facing a wall
                        move(oi, wall_dir ? 0.0 : 180.0, dist_drive + GAP_EXTRA_DIST + PING_CENTER_DIST, DRIVE_POWER, FORWARD);

                        // We stop recording distance when a gap is detected, incorporate
                        // the gap distance into the current distance
                        dist_current += dist_gap + GAP_EXTRA_DIST;

                        timer_waitMillis(2000);
                    }

                    // First param is the heading
                    set_drive_power(wall_dir ? 0.0 : 180.0, DRIVE_POWER);
                    oi_update(oi);
                    dist_current += oi->distance;

                    //lcd_printf("Current: %d", dist_current);
                }

                oi_setWheels(0.0, 0.0);
            }
            // Side finished
            else
            {
                if(wall_dir == LEFT)
                {
                    // Turn CyBot around
                    wall_dir = RIGHT;
                    turn_to(oi, 180.0);
                    timer_waitMillis(MOVE_DELAY);
                }
                else
                {
                    // Turn CyBot back to starting orientation
                    turn_to(oi, 0.0);
                    timer_waitMillis(MOVE_DELAY);
                    auto_finished = 1;
                }
            }
        }
    }
    else
    {
//        while(1)
//        {
//            // Tele-Op
//            // Clear string out
//            int i;
//            for(i = 0; i < MAX_STRING_SIZE; ++i)
//            {
//                uart_rec[i] = 0;
//            }
//
//            lcd_printf("Waiting for command...");
//            uart_receive_str(uart_rec);
//
//            switch(get_message_type(uart_rec))
//            {
//            case MOVE:
//            {
//                // Get direction and mag
//                move_dir_t drive_dir = FORWARD;
//                turn_dir_t turn_dir = CLOCKWISE;
//                double mag = 0;
//
//                int do_turn = 0;
//
//                clear_tokens(tokens, MAX_TOKENS);
//                jsmn_parse(&parser, uart_rec, strlen(uart_rec), tokens, MAX_TOKENS);
//                jsmn_init(&parser);
//
//                clear_string(jsmn_data);
//
//                get_jsmn_data(jsmn_data, "direction", uart_rec, tokens, MAX_TOKENS);
//
//                if(!strcmp(jsmn_data, "CLOCKWISE"))
//                {
//                    do_turn = 1;
//                    turn_dir = CLOCKWISE;
//                }
//                else if(!strcmp(jsmn_data, "C_CLOCKWISE"))
//                {
//                    do_turn = 1;
//                    turn_dir = COUNTER_CLOCKWISE;
//                }
//                else if(!strcmp(jsmn_data, "FORWARD"))
//                {
//                    do_turn = 0;
//                    drive_dir = FORWARD;
//                }
//                else if(!strcmp(jsmn_data, "REVERSE"))
//                {
//                    do_turn = 0;
//                    drive_dir = BACKWARD;
//                }
//
//                clear_string(jsmn_data);
//                get_jsmn_data(jsmn_data, "magnitude", uart_rec, tokens, MAX_TOKENS);
//
//                sscanf(jsmn_data, "%lf", &mag);
//
//                clear_string(jsmn_data);
//
//                if(do_turn)
//                {
//                    turn(oi, mag, turn_dir);
//                }
//                else
//                {
//                    move(oi, get_orientation().heading, mag, DRIVE_POWER, drive_dir);
//                }
//
//                break;
//            }
//            }
//        }


    }



    final_free();

    return 0;
}
