#ifndef IMU_H
#define IMU_H

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <math.h>
#include "bno055.h"
#include "Components/Timer.h"
#include "Components/lcd.h"

// Mhz
#define CLOCK 16

// Clock constants, see datasheet for info
#define SCL_LP 6
#define SCL_HP 4
#define SLAVE_ADDR 0x29

// 100kbps
#define SCL_CLK 100000

// Euler Angle LSB
#define EUL_LSB 16.0

typedef struct raw_accel
{
    signed short x;
    signed short y;
    signed short z;
} raw_accel_t;

// Units: meters per second squared
typedef struct accel
{
    float x;
    float y;
    float z;
} accel_t;

typedef struct raw_orientation
{
    signed short heading;
    signed short roll;
    signed short pitch;
} raw_orientation_t;

typedef struct orientation
{
    float heading;
    float roll;
    float pitch;
} orientation_t;

typedef enum calib_state
{
    CALIBRATED = 0x3,
    PENDING = 0x0
} calib_state_t;

typedef struct calib
{
    calib_state_t system;
    calib_state_t gyro;
    calib_state_t accel;
    calib_state_t mag;
} calib_t;

orientation_t starting_orientation;

void calibrate_imu();

void reset_starting_orientation();
float add_angle_heading(float angle_a, float angle_b);
raw_orientation_t get_raw_orientation();
orientation_t get_orientation();

raw_accel_t get_raw_accel();
accel_t get_accel();

void imu_init();

void imu_free();

void write_register(char data, char reg_addr);

char read_register_single_byte(char reg_addr);

void read_register_nbyte(int size, char reg_addr, char* dest);

#endif
