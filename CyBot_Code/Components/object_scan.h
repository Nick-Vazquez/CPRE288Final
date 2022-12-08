/*
 * Ojbect_Scan.h
 *
 *  Created on: Oct 6, 2022
 *      Author: neastbur
 */
#ifndef COMPONENTS_OBJECT_SCAN_H_
#define COMPONENTS_OBJECT_SCAN_H_

#include <Components/IR/IR.h>
#include <Components/lcd.h>
#include <Components/servo.h>
#include <Components/Timer.h>
#include <Components/uart.h>
#include <stdlib.h>
#include "Components/json.h"

#define TIME_DELTA 75
#define ANGLE_DELTA 1
#define DISTANCE_TOLERANCE 4.0
#define DIFFERENT_OBJECT_TOLERANCE 5.0
#define AVERAGE_TOLERANCE 4.0
//#define FARTHEST_DISTANCE 50

// For now, there will be no shortest distance
#define SHORTEST_DISTANCE 0

#define MIN_WIDTH 1.5
#define MAX_WIDTH 10.0
#define DATA_BLOCK_SIZE 6
#define NUM_READINGS_IR_SCAN 1
#define MAX_OBJECTS 10
#define PI 3.14159
#define JSON_STRING_SIZE 5000

float farthest_distance;

typedef struct scan_data
{
    float ir_distance;
    int angle;
} scan_data_t;

typedef struct object
{
    float distance;
    int radial_width;
    float linear_width;
    int start_angle;
    int median_angle;
    int room_number;
} object_t;

typedef struct objects_data
{
    object_t* objects;
    int object_count;
} objects_data_t;

objects_data_t scan(int start_angle, int end_angle, int silent);
float get_largest_average_distance(int start_angle, int end_angle, int average_count);
float get_smallest_average_distance(int start_angle, int end_angle, int average_count);
object_t get_smallest_object(objects_data_t data);
void object_scan_alloc();
void object_scan_free();
float compute_sample_average_float(float* samples, int num_samples);
int compute_sample_average_int(int* samples, int num_samples);
float calculate_ir_distance(int raw_ir_val);

#endif /* COMPONENTS_OBJECT_SCAN_H_ */
