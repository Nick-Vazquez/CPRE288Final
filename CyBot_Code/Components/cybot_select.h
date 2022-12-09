/*
 * cybot_Select.h
 *
 *  Created on: Nov 8, 2022
 *      Author: neastbur
 */

#ifndef COMPONENTS_CYBOT_SELECT_H_
#define COMPONENTS_CYBOT_SELECT_H_

// Represents all CyBot specific sensor calibration values.
typedef struct cybot_calib_values
{
    // Timer match value for the left servo position (180 degrees)
    int left_calibration_value;

    // Timer match value for the right servo position (0 degrees)
    int right_calibration_value;

    // A variable for quadratic formula for quadratic fit
    float ir_a;

    // B variable for quadratic formula for quadratic fit
    float ir_b;

    // C variable for quadratic formula for quadratic fit
    float ir_c;
} cybot_calib_values_t;

// Returns the calibration struct for a specific CyBot number
cybot_calib_values_t set_cybot_calibration(int cybot_num);

#endif /* COMPONENTS_CYBOT_SELECT_H_ */
