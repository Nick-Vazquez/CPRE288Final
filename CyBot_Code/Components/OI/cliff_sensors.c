#include "cliff_sensors.h"


/*
 * cliff.c
 *
 *  Created on: Nov 29, 2022
 *      Author: rrlowe02
 */

    cliff_values_t set_cliff_values(oi_t* oi){
        cliff_values_t cliff_values;

        oi_update(oi);

        cliff_values.left_sensor = oi->cliffLeft;
        cliff_values.right_sensor = oi->cliffRight;
        cliff_values.front_left = oi->cliffFrontLeft;
        cliff_values.front_right = oi->cliffFrontRight;

        if(cliff_values.left_sensor == 1 || cliff_values.right_sensor == 1 || cliff_values.front_left == 1 || cliff_values.front_right == 1){
            cliff_values.overall = 1;
        }
        else{
            cliff_values.overall = 0;
        }
        return cliff_values;
    }

    int get_cliff(oi_t* oi){
        cliff_values_t cliff_values = set_cliff_values(oi);

        if(cliff_values.overall == 1){
            return 1;
        }
        return 0;
    }



