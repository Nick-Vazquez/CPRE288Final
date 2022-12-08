#include <Components/OI/open_interface.h>
#include <math.h>
#include "Components/IMU.h"

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#define TURN_ERROR 10
#define BACKUP_DIST 150
#define MAX_POWER 500

// For linear turn power scaling

// Dependent variable
#define TURN_HIGH_POWER 0.6
#define TURN_LOW_POWER 0.05

// Independent variable
// The delta values are taken from the current and the target
// So, if the CyBot has a lot of turning to do, the delta is high, and the power is high
#define TURN_HIGH_DELTA 80.0
#define TURN_LOW_DELTA  0.5

// For linear drive power scaling

// Dependent variable
#define DRIVE_HIGH_POWER 0.4
#define DRIVE_LOW_POWER 0.05

// Independent variable
#define DRIVE_HIGH_ERROR 10.0
#define DRIVE_LOW_ERROR 1.0

typedef enum turn_dir
{
    CLOCKWISE = 1, COUNTER_CLOCKWISE = -1
} turn_dir_t;

typedef enum move_dir
{
    FORWARD = 1, BACKWARD = -1
} move_dir_t;

void imu_init();

// Move helper functions
float get_target_delta(float target, int direction);
float get_linear_drive_power(float error);
void set_drive_power(float target_heading, float default_power);
// End helper functions

void move(oi_t* oi, float start_heading, float distance, float power, int direction);

void turn(oi_t* oi, float degrees, turn_dir_t direction);

void turn_to(oi_t* oi, float dest);

//void move_collision(oi_t* oi, int distance, float power, int move_around, int current_angle, int* bump_left, int* bump_right);


#endif /* MOVEMENT_H_ */
