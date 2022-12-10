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

// Variables used in ISR to keep track of event times.
volatile int start_time;
volatile int end_time;

// Inits the ping sensor.
void ping_init();

// Gets the time delta between the sound send and receive.
int ping_time();

// Calculates the distance based on the ping_time, which is the
// delta between the sound send and receive.
double calc_distance(int ping_time);

// Runs ping_time() and passes the result into calc_distance().
// This is just a helper function to get the distance the ping
// determined in a single function call.
double ping_distance();

// Returns the average distance provided by the samples.
double average_distance(double* samples, const int NUM_SAMPLES);

#endif /* COMPONENTS_PING_H_ */
