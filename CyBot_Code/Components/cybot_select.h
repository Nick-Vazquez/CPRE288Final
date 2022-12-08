/*
 * cybot_Select.h
 *
 *  Created on: Nov 8, 2022
 *      Author: neastbur
 */

#ifndef COMPONENTS_CYBOT_SELECT_H_
#define COMPONENTS_CYBOT_SELECT_H_

typedef struct cybot_calib_values
{
    int left_calibration_value;
    int right_calibration_value;
    float ir_a;
    float ir_b;
    float ir_c;
} cybot_calib_values_t;

cybot_calib_values_t set_cybot_calibration(int cybot_num);

#endif /* COMPONENTS_CYBOT_SELECT_H_ */
