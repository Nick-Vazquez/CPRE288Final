/*
 * servo.h
 *
 *  Created on: Nov 1, 2022
 *      Author: neastbur
 */

#ifndef COMPONENTS_SERVO_H_
#define COMPONENTS_SERVO_H_
#include <Components/cybot_select.h>
#include <Components/lcd.h>
#include <Components/Timer.h>
#include "math.h"
#include <inc/tm4c123gh6pm.h>

// In Mhz
// System clock speed.
#define CLOCK 16.0

// In ms
// Defined duty cycle period in servo datasheet.
#define PERIOD 20.0

// Default output compare match value. This should set the
// servo to 90 degrees. Does not take into consideration
// calibration.
#define START_VAL (PERIOD / (1000 / (CLOCK * pow(10, 6))))

// Prescaler used in output compare. This remains constant
// for all duty cycles for the servo.
#define PRESCALE 0x40000

// Inits the servo.
void servo_init(cybot_calib_values_t calib);

// Moves the servo to the specified angle.
void servo_move(float degrees);

// Runs the servo calibration routine. If bound_override is
// 1, then the servo can be set outside the range 180 to 0.
// If 0, then the servo is constrained to the range.
void servo_calib(int bound_override);

// Left and right calibration values. These
// are the match values for the output compare for the leftmost and
// rightmost servo positions. Instead of relying on the datasheet,
// the proper match values are determined by running the calibration
// program.
int left_val;
int right_val;

#endif /* COMPONENTS_SERVO_H_ */
