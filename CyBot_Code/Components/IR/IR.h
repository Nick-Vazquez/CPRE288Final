/*
 * IR.h
 *
 *  Created on: Oct 18, 2022
 *      Author: neastbur
 */

#ifndef COMPONENTS_IR_IR_H_
#define COMPONENTS_IR_IR_H_
#include <Components/cybot_select.h>

// Struct to represent both the read IR value and the reciprocal of the distance.
// These data points are used in the IR calibration process.
typedef struct reading
{
    // 1/cm
    float distance_recip;
    
    // This is the raw IR value read from the ADC.
    int ir_val;
} reading_t;

#define THROW_AWAY 100.0

float ir_calib_a;
float ir_calib_b;
float ir_calib_c;

// Inits the IR sensor. Sets ADC registers to read from the IR.
void ir_init(cybot_calib_values_t calib);

// Read the raw ADC value from the IR sensor.
int ir_read();

// Sets the hardware averaging amount
void set_average_amount(int avg);

// Computes the average of the IR ADC values where the number of samples
// is given by SAMPLE_SIZE
int compute_adc_sample_average(const int SAMPLE_SIZE);

// Given a raw IR ADC value, compute the distance in CM
float calculate_ir_distance(int raw_ir_val);

#endif /* COMPONENTS_IR_IR_H_ */
