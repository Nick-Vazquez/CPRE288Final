/*
 * IR.h
 *
 *  Created on: Oct 18, 2022
 *      Author: neastbur
 */

#ifndef COMPONENTS_IR_IR_H_
#define COMPONENTS_IR_IR_H_
#include <Components/cybot_select.h>

typedef struct reading
{
    // 1/cm
    float distance_recip;

    int ir_val;
} reading_t;

#define THROW_AWAY 100.0

float ir_calib_a;
float ir_calib_b;
float ir_calib_c;

void ir_init(cybot_calib_values_t calib);
int ir_read();
void set_average_amount(int avg);
int compute_adc_sample_average(const int SAMPLE_SIZE);
float calculate_ir_distance(int raw_ir_val);

#endif /* COMPONENTS_IR_IR_H_ */
