/*
 * ping.h
 *
 *  Created on: Oct 25, 2022
 *      Author: neastbur
 */

#ifndef COMPONENTS_PING_H_
#define COMPONENTS_PING_H_
#include <Components/Timer.h>
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include <math.h>

// Mhz
#define CLOCK 16

// Speed of sound in centimeters per second
#define SOUND_SPEED 34000.0

volatile int start_time;
volatile int end_time;

void ping_init();
int ping_time();
double calc_distance(int ping_time);
double ping_distance();
double average_distance(double* samples, const int NUM_SAMPLES);


#endif /* COMPONENTS_PING_H_ */
