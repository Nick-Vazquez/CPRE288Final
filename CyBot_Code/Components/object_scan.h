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

// Time between scan readings.
#define TIME_DELTA 75

// Angle delta between readings.
#define ANGLE_DELTA 1

// Tolerance used to determine if two samples are part of the same object.
#define DISTANCE_TOLERANCE 4.0

// Min distance used to tell sequential objects apart.
#define DIFFERENT_OBJECT_TOLERANCE 5.0

// On the second scan-through, this is used to determine if samples
// are a part of the recorded average distance per object.
// This identifies at what angles the objects start and stop.
#define AVERAGE_TOLERANCE 4.0

// For now, there will be no shortest distance
// If a distance reading is less than this, then disregard it.
#define SHORTEST_DISTANCE 0

// Min width of objects, if less, disregard.
#define MIN_WIDTH 1.5

// Max width of objects, if greater, disregard.
#define MAX_WIDTH 10.0

// How many readings define a sample.
#define DATA_BLOCK_SIZE 6

// How many IR readings are taken at each scan.
#define NUM_READINGS_IR_SCAN 1

// Max number of objects.
#define MAX_OBJECTS 10

#define PI 3.14159

// JSON output string size
#define JSON_STRING_SIZE 5000

// If a distance reading is greater than this, disregard it.
// Is a varaible so the programmer can set at runtime.
float farthest_distance;

// Contains raw IR scan data.
typedef struct scan_data
{
    float ir_distance;
    int angle;
} scan_data_t;

// Contains all data associated with an object.
typedef struct object
{
    float distance;
    int radial_width;
    float linear_width;
    int start_angle;
    int median_angle;
    int room_number;
} object_t;

// Contains all read objects.
typedef struct objects_data
{
    object_t* objects;
    int object_count;
} objects_data_t;

// Performs a scan from start_angle to end_angle. This returns
// an objects_data_t struct containing all objects as well as the number of objects.
// Silent: 1 = no uart prints besides the JSON output | 0 = uart print outs are enabled.
objects_data_t scan(int start_angle, int end_angle, int silent);

// Returns the average of the average_count largest distance readings from start_angle to end_angle
float get_largest_average_distance(int start_angle, int end_angle, int average_count);

// Returns the average of the average_count smallest distance readings from start_angle to end_angle
float get_smallest_average_distance(int start_angle, int end_angle, int average_count);

// Returns the object with the smallest linear width from the data set.
object_t get_smallest_object(objects_data_t data);

// Allocates space needed for scan().
void object_scan_alloc();

// Frees memory allocated in object_scan_alloc().
void object_scan_free();

// Returns the average of the samples as a float.
float compute_sample_average_float(float* samples, int num_samples);

// Returns the average of the samples as an int.
int compute_sample_average_int(int* samples, int num_samples);

#endif /* COMPONENTS_OBJECT_SCAN_H_ */
