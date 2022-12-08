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
#define CLOCK 16.0

// In ms
#define PERIOD 20.0

#define START_VAL (PERIOD / (1000 / (CLOCK * pow(10, 6))))

#define PRESCALE 0x40000

void servo_init(cybot_calib_values_t calib);
void servo_move(float degrees);
void servo_calib(int bound_override);

int left_val;
int right_val;

#endif /* COMPONENTS_SERVO_H_ */
