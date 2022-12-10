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

// Contains the raw 16-bit signed integer values for the x, y, and z axes acceleration values
typedef struct raw_accel
{
    signed short x;
    signed short y;
    signed short z;
} raw_accel_t;

// Units: meters per second squared
// Contains converted acceleration values for the x, y, and z axes
typedef struct accel
{
    float x;
    float y;
    float z;
} accel_t;

// Contains the raw 16-bit signed integer values for the x, y, and z axes orientation values
typedef struct raw_orientation
{
    signed short heading;
    signed short roll;
    signed short pitch;
} raw_orientation_t;

// Units: degrees
// Contains converted orientation values for the x, y, and z axes
typedef struct orientation
{
    float heading;
    float roll;
    float pitch;
} orientation_t;

// Represents the different calibration states that the IMU sensor can have
typedef enum calib_state
{
    CALIBRATED = 0x3,
    PENDING = 0x0
} calib_state_t;

// Stores the calibration states of all sensors on the IMU module
typedef struct calib
{
    calib_state_t system;
    calib_state_t gyro;
    calib_state_t accel;
    calib_state_t mag;
} calib_t;

// Stores the starting orientation value. This is used as the "zero" for the IMU sensor.
orientation_t starting_orientation;

// Displays the calibration data of the IMU and loops until the calibration is complete.
// The IMU will automatically calibrate itself under certain movement conditions.
void calibrate_imu();

// Resets the starting orientation. All future orientations are relative to the
// orientation of the CyBot when this is called.
void reset_starting_orientation();

// Adds two angles, removes coterminal angles and converts any negative angles
// to the positive counterparts.
float add_angle_heading(float angle_a, float angle_b);

// Gets the raw orientation values of all axes.
raw_orientation_t get_raw_orientation();

// Gets the real orientation values. Runs get_raw_orientation() and coverts values.
orientation_t get_orientation();

// Gets the raw acceleration values of all axes.
raw_accel_t get_raw_accel();

// Gets the real acceleration values. Runs get_raw_accel() and coverts values.
accel_t get_accel();

// Inits the IMU sensor. Sets up the I2C interface and appropriate GPIO lines.
void imu_init();

// Frees any reserved heap space used on init.
void imu_free();

// Writes a byte to a specified register on the IMU.
void write_register(char data, char reg_addr);

// Reads a single byte from a register on the IMU.
char read_register_single_byte(char reg_addr);

// Reads a specified number of bytes from a starting register on the IMU.
// Sequential reads are taken from the next register, which lies at previous_address + 1.
void read_register_nbyte(int size, char reg_addr, char* dest);

#endif
