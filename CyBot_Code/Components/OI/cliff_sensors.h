/*
 * cliff.h
 *
 *  Created on: Nov 29, 2022
 *      Author: rrlowe02
 */

#ifndef CLIFF_H_
#define CLIFF_H_

#include "open_interface.h"
#include "Components/lcd.h"
#include <inc/tm4c123gh6pm.h>

// Stores the raw sensor values of each of the cliff sensors
// as well as the OR of all the values.
typedef struct cliff_values
{
    uint32_t left_sensor;
    uint32_t right_sensor;
    uint32_t front_right;
    uint32_t front_left;
    uint32_t overall;
}cliff_values_t;

// Returns a struct containing all cliff sensor data
cliff_values_t set_cliff_values(oi_t* oi);

// Returns the overall cliff sensor status
int get_cliff(oi_t* oi);

#endif /* CLIFF_H_ */


