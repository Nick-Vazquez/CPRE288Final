/*
 * movement.c
 *
 *  Created on: Sep 6, 2022
 *      Author: neastbur
 */
#include <Components/OI/movement.h>

// Returns the result of a greater than, less than logic statement based on the direction
// direction > 0 means that target is greater than current
// direction < 0 means that target is less than current
int current_met_target(float target, float current, int direction)
{
    if(direction > 0)
    {
        return current > target;
    }
    else if(direction < 0)
    {
        return current < target;
    }
    else
    {
        return 0;
    }
}

// Returns 1 if the turn operation has met the target destination.
// Returns 0 otherwise.
int met_turn_target(float target, int direction)
{
    return get_target_delta(target, direction) <= TURN_LOW_DELTA;
}

float get_target_delta(float target, int direction)
{
    float current = get_orientation().heading;

    if(direction > 0)
    {
        // This will run if the rollover has already occurred or no rollover will happen
        return  add_angle_heading(target, -(current));
    }
    else
    {
        // This will run if the rollover has already occurred or no rollover will happen
        return  add_angle_heading(-target, current);
    }
}

// Given the linear constants, determines the desired turning power based
// upon how much farther the CyBot needs to turn.
float get_linear_turn_power(float delta)
{
    if(delta > TURN_HIGH_DELTA)
    {
        return TURN_HIGH_POWER;
    }
    else if(delta < TURN_LOW_DELTA)
    {
        return TURN_LOW_DELTA;
    }

    return (((TURN_HIGH_POWER - TURN_LOW_POWER) / (TURN_HIGH_DELTA - TURN_LOW_DELTA) * (delta - TURN_HIGH_DELTA)) + TURN_HIGH_POWER);
}

// The param "error" is the degree error from the desired heading
float get_linear_drive_power(float error)
{
    return (((DRIVE_HIGH_POWER - DRIVE_LOW_POWER) / (DRIVE_HIGH_ERROR - DRIVE_LOW_ERROR) * (error - DRIVE_HIGH_ERROR)) + DRIVE_HIGH_POWER);
}

// This is a helper function to set the drive power
void set_drive_power(float target_heading, float default_power)
{

    float current = get_orientation().heading;

    // Determine which direction correction needs to be made in (CW or CCW)
    float clockwise_error = get_target_delta(target_heading, CLOCKWISE);
    float counter_clockwise_error = get_target_delta(target_heading, COUNTER_CLOCKWISE);

    // Helper variable that stores the power scaler value
    float scaler;

    if(clockwise_error <= counter_clockwise_error)
    {
        // Adjust clockwise
        scaler = ((MAX_POWER - default_power) * get_linear_drive_power(clockwise_error));
        oi_setWheels(default_power - scaler, default_power + scaler);
    }
    else
    {
        // Adjust counterclockwise
        scaler = ((MAX_POWER - default_power) * get_linear_drive_power(counter_clockwise_error));
        oi_setWheels(default_power + scaler, default_power - scaler);
    }
}


// -1 = backwards 1 = forwards
void move(oi_t* oi, float start_heading, float distance, float power, int direction)
{
    power = fabs(power);

    float displacement = 0.0;

    while(!current_met_target((direction * distance), displacement, direction))
    {
        oi_update(oi);

        displacement += oi->distance;

        if(direction < 0)
        {
            set_drive_power(start_heading, -power);
        }
        else
        {
            set_drive_power(start_heading, power);
        }
    }


    oi_setWheels(0, 0);
}

// 1 = CW (+)   |   -1 = CCW (-)
void turn(oi_t* oi, float degrees, turn_dir_t direction)
{
    float power = 0.0;

    float delta = 0.0;

    degrees = fabs(degrees);

    oi_update(oi);

    float target = add_angle_heading(get_orientation().heading, (direction * degrees));

    delta = get_target_delta(target, direction);

    power = get_linear_turn_power(delta);

    while(!(met_turn_target(target, direction)))
    {
        delta = get_target_delta(target, direction);
        power = get_linear_turn_power(delta);

        oi_setWheels((-direction * (MAX_POWER * power)), (direction * (MAX_POWER * power)));
        lcd_printf("Degree Delta: %.3f", get_target_delta(target, direction));
    }

    oi_setWheels(0, 0);
}

void turn_to(oi_t* oi, float dest)
{

    float clockwise_delta = get_target_delta(dest, CLOCKWISE);

    float counter_clockwise_delta = get_target_delta(dest, COUNTER_CLOCKWISE);

    if(clockwise_delta <= counter_clockwise_delta)
    {
        turn(oi, clockwise_delta, CLOCKWISE);
    }
    else
    {
        turn(oi, counter_clockwise_delta, COUNTER_CLOCKWISE);
    }
}
