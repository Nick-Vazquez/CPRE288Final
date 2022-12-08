#include "IMU.h"
#include <stdarg.h>
#include <stdlib.h>

typedef enum status_wait_options
{BUS_BUSY = 0x40, ACK = 0x8, ERROR = 0x2, BUSY = 0x1}
status_wait_options_t;

// 0: Starting not set
// 1: Starting set
int orientation_flag = 0;

char* data;

// This will poll a while loop until all provided flags are 0
void status_wait(int num, ...)
{
    int mask = 0;

    va_list list;
    va_start(list, num);

    int i;
    for(i = 0; i < num; ++i)
    {
        mask |= va_arg(list, status_wait_options_t);
    }

    va_end(list);

    // Wait for corresponding flags to all be zero
    while(I2C1_MCS_R & mask)
    {

    }
}

void reset_imu()
{
    GPIO_PORTB_DATA_R &= 0xBF;
    timer_waitMillis(500);
    GPIO_PORTB_DATA_R |= 0x40;
    timer_waitMillis(500);
}

float convert_accel(signed short raw)
{
    return (float)((1.0/100.0) * raw);
}

float convert_orientation(signed short raw)
{
    return (float)((1.0/EUL_LSB) * raw);
}

raw_accel_t get_raw_accel()
{
    raw_accel_t result;

    // 3 axis, each need two bytes
    // Order: LSB then MSB: X,Y,Z
    char* data_start = data;

    // Read X
    read_register_nbyte(2, BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR, data);
    data += 2;

    // Read Y
    read_register_nbyte(2, BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR, data);
    data += 2;

    // Read Z
    read_register_nbyte(2, BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR, data);
    data = data_start;

    // Package data
    result.x = (data[1] << 8) | data[0];
    result.y = (data[3] << 8) | data[2];
    result.z = (data[5] << 8) | data[4];

    return result;
}

void reset_starting_orientation()
{
    orientation_flag = 0;

    starting_orientation = get_orientation();

    orientation_flag = 1;
}

float add_angle_heading(float angle_a, float angle_b)
{
    signed short final_raw = (signed short)(angle_a * EUL_LSB) + (signed short)(angle_b * EUL_LSB);

    // Now, apply the modulus operator,
    final_raw %= (signed short)(360 * EUL_LSB);

    // Check to see if negative, if so, add 360
    if(final_raw < 0)
    {
        final_raw += (signed short)(360 * EUL_LSB);
    }

    // Finally, convert back to degrees
    return convert_orientation(final_raw);
}

raw_orientation_t get_raw_orientation()
{
    raw_orientation_t result;

    // 3 axis, each need two bytes
    // Order: LSB then MSB: Heading, Roll, Pitch
    char* data_start = data;

    // Read Heading
    read_register_nbyte(2, BNO055_EULER_H_LSB_ADDR, data);
    data += 2;

    // Read Roll
    read_register_nbyte(2, BNO055_EULER_R_LSB_ADDR, data);
    data += 2;

    // Read Pitch
    read_register_nbyte(2, BNO055_EULER_P_LSB_ADDR, data);
    data = data_start;

    // Package data
    result.heading = (data[1] << 8) | data[0];
    result.roll = (data[3] << 8) | data[2];
    result.pitch = (data[5] << 8) | data[4];

    return result;
}

orientation_t get_orientation()
{
    orientation_t result;
    raw_orientation_t raw = get_raw_orientation();

    if(orientation_flag)
    {
        result.heading = add_angle_heading(convert_orientation(raw.heading), -(starting_orientation.heading));
        result.roll = convert_orientation(raw.roll) - starting_orientation.roll;
        result.pitch = convert_orientation(raw.pitch) - starting_orientation.pitch;
    }
    else
    {
        result.heading = convert_orientation(raw.heading);
        result.roll = convert_orientation(raw.roll);
        result.pitch = convert_orientation(raw.pitch);
    }

    return result;
}

accel_t get_accel()
{
    accel_t result;
    raw_accel_t raw = get_raw_accel();

    result.x = convert_accel(raw.x);
    result.y = convert_accel(raw.y);
    result.z = convert_accel(raw.z);

    return result;
}

void imu_init()
{
    // This will hold 6 bytes for X, Y, and Z acceleration and orientation data
    data = malloc(6);

    timer_init();

    // Enable I2C clocking for I2C 1
    SYSCTL_RCGCI2C_R |= 0x2;
    // Enable clocking for GPIO A and GPIO B, where the SCL and SDA live
    SYSCTL_RCGCGPIO_R |= 0x3;

    timer_waitMillis(1);


    // GPIO Setup
    GPIO_PORTA_DEN_R |= 0xC0;
    GPIO_PORTA_DIR_R |= 0xC0;
    // Enable Open-Drain mode for PA7, which is SDA
    GPIO_PORTA_AFSEL_R |= 0xC0;
    GPIO_PORTA_ODR_R |= 0x80;

    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF) | 0x33000000;

    // I2C Setup
    // Set the module as master
    I2C1_MCR_R = 0x10;

    // Setup SCL clock speed, this represents the number of system clock periods in one SCL clock period.
    I2C1_MTPR_R = ((CLOCK * pow(10,6)) / (2 * (SCL_LP + SCL_HP) * SCL_CLK)) - 1;

    // Select default address by setting COM3 to 1 | PB7
    // Set resetn high on PB6
    GPIO_PORTB_DEN_R |= 0xC0;
    GPIO_PORTB_DIR_R |= 0xC0;
    GPIO_PORTB_AFSEL_R &= 0x3F;
    GPIO_PORTB_DATA_R |= 0xC0;

    reset_imu();

    // Flip signs, all signs from readings seemed to be wrong
    write_register(0x7, BNO055_AXIS_MAP_SIGN_ADDR);

    write_register(0x8, BNO055_OPR_MODE_ADDR);

    // Allow for gyro calib
    timer_waitMillis(2000);

    reset_starting_orientation();

}

void imu_free()
{
    free(data);
}

calib_t get_calib_state()
{
    calib_t result;
    char state = read_register_single_byte(BNO055_CALIB_STAT_ADDR);

    result.mag = (calib_state_t) (state & 0x3);
    result.accel = (calib_state_t) ((state >> 2) & 0x3);
    result.gyro = (calib_state_t) ((state >> 4) & 0x3);
    result.system = (calib_state_t) ((state >> 6) & 0x3);

    return result;

}

char* calib_to_string(calib_state_t state)
{
    switch(state)
    {
        case CALIBRATED:
        {
            return "CALIBRATED";
        }
        case PENDING:
        {
            return "PENDING";
        }
        default:
        {
            return "UNKNOWN";
        }
    }
}

void calibrate_imu()
{
    // Put in AMG mode so all sensors can be used

    //write_register(0x7, BNO055_OPR_MODE_ADDR);

    //timer_waitMillis(100);

    calib_t calib = get_calib_state();

    lcd_init();

    while(calib.gyro == PENDING)
    {
        calib = get_calib_state();
        lcd_printf("Accel: %s\nGyro: %s", calib_to_string(calib.accel), calib_to_string(calib.gyro));
    }

    write_register(0x0, BNO055_OPR_MODE_ADDR);
    timer_waitMillis(100);

    while(1)
    {
        lcd_printf("Gryo Off X LSB: %d\nGryo Off X MSB: %d", read_register_single_byte(BNO055_GYRO_OFFSET_X_LSB_ADDR), read_register_single_byte(BNO055_GYRO_OFFSET_X_MSB_ADDR));
    }
}

void send_reg_addr(char reg_addr)
{

    // Set slave address and R/W bit
    I2C1_MSA_R = (SLAVE_ADDR << 1) & 0xFE;

    // Load address to send
    I2C1_MDR_R = reg_addr;

    status_wait(1, BUS_BUSY);

    // Send address. Flow: Start -> Transmit (no ack bit needs to be sent since this is a transmit, no stop since we need to send the data next)
    I2C1_MCS_R = 0x3;

    status_wait(2, BUSY, ACK);
}

char read_register_single_byte(char reg_addr)
{

    send_reg_addr(reg_addr);

    // Set to read
    I2C1_MSA_R |= 0x1;

    // Receive data. Flow: Start -> Receive -> Send NACK because we are only receiving a single byte
    // If more bytes were to be received, an ACK would be sent instead The NACK tells the sensor that the byte it
    // just sent is the final byte of the transmission.
    I2C1_MCS_R = 0x3;

    status_wait(1, BUSY);

    // Send stop
    I2C1_MCS_R = 0x4;

    // Check BUS_BUSY since the bus should no longer be busy after the STOP is sent
    status_wait(2, BUSY, BUS_BUSY);

    return I2C1_MDR_R;

}

void read_register_nbyte(int size, char reg_addr, char* dest)
{
    send_reg_addr(reg_addr);

    // Set to read
    I2C1_MSA_R |= 0x1;

    // Do first receive, put I2C into receive mode, ack enabled
    // Flow: Repeated Start -> Receive (with ack)
    I2C1_MCS_R = 0xB;

    status_wait(1, BUSY);

    *dest = I2C1_MDR_R;
    dest++;

    // One byte has already be received and last byte needs to be received with a NACK, so we only iterate size - 2 times.
    int i;
    for(i = 0; i < size - 2; ++i)
    {
        // Do another receive with ack enabled. NOTE: I2C module is in the receive state
        // Flow: Receive (with ack)
        I2C1_MCS_R = 0x9;

        status_wait(1, BUSY);

        *dest = I2C1_MDR_R;
        dest++;
    }

    // Do another receive with ack enabled. NOTE: I2C module is in the receive state
    // Flow: Receive (with negative ack)
    I2C1_MCS_R = 0x1;

    status_wait(1, BUSY);

    *dest = I2C1_MDR_R;
    dest++;

    // Send stop
    I2C1_MCS_R = 0x4;

    // Check BUS_BUSY since the bus should no longer be busy after the STOP is sent
    status_wait(2, BUSY, BUS_BUSY);
}

void write_register(char data, char reg_addr)
{
    send_reg_addr(reg_addr);

    // Send data. Flow: Start -> Transmit (no ack bit needs to be sent since this is a transmit, no stop since we need to send the data next)
    I2C1_MCS_R = 0x3;

    // Waits for controller to be idle
    status_wait(2, BUSY, ACK);

    // Set data to be the data to put in the specified register
    I2C1_MDR_R = data;

    // Send data. Flow: Transmit -> Stop (no futher information needs to be sent, stop bit can be sent now. No extra start bit needed)
    I2C1_MCS_R = 0x5;

    // Check BUS_BUSY since the bus should no longer be busy after the STOP is sent
    status_wait(2, BUSY, BUS_BUSY);


}
