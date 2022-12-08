#include <Components/cybot_select.h>

cybot_calib_values_t set_cybot_calibration(int cybot_num)
{
    cybot_calib_values_t cybot_calib;
    switch(cybot_num)
    {
    case 1:
    {
        cybot_calib.left_calibration_value = 1261750;
        cybot_calib.right_calibration_value = 253750;
        cybot_calib.ir_a = -73753.0;
        cybot_calib.ir_b = 27747.0;
        cybot_calib.ir_c = 157.65;
        break;
    }
    case 3:
    {
        cybot_calib.left_calibration_value = 23323;
        cybot_calib.right_calibration_value = 50406;
        cybot_calib.ir_a = -46374.0;
        cybot_calib.ir_b = 32295.0;
        cybot_calib.ir_c = 24.361;
        break;
    }
    case 4:
    {
        cybot_calib.left_calibration_value = 22678;
        cybot_calib.right_calibration_value = 51709;
        cybot_calib.ir_a = 17590.0;
        cybot_calib.ir_b = 26170.0;
        cybot_calib.ir_c = 311.75;
        break;
    }
    case 5:
    {
        cybot_calib.left_calibration_value = 21774;
        cybot_calib.right_calibration_value = 49773;
        cybot_calib.ir_a = -17791.0;
        cybot_calib.ir_b = 30513.0;
        cybot_calib.ir_c = 142.86;
        break;
    }
    case 6:
    {
        cybot_calib.left_calibration_value = 22678;
        cybot_calib.right_calibration_value = 49773;
        cybot_calib.ir_a = -60654.0;
        cybot_calib.ir_b = 26309.0;
        cybot_calib.ir_c = 145.74;
        break;
    }
    case 7:
    {
        cybot_calib.left_calibration_value = 20612;
        cybot_calib.right_calibration_value = 49765;
        break;
    }
    case 8:
    {
        cybot_calib.left_calibration_value = 22396;
        cybot_calib.right_calibration_value = 50239;
        cybot_calib.ir_a = -2363.2;
        cybot_calib.ir_b = 25680.0;
        cybot_calib.ir_c = 209.24;
        break;
    }
    case 9:
    {
        cybot_calib.left_calibration_value = 22032;
        cybot_calib.right_calibration_value = 49934;
        cybot_calib.ir_a = -34529.0;
        cybot_calib.ir_b = 39854.0;
        cybot_calib.ir_c = 115.67;
        break;
    }
    case 10:
    {
        cybot_calib.left_calibration_value = 1314250;
        cybot_calib.right_calibration_value = 295750;
        cybot_calib.ir_a = -49129.0;
        cybot_calib.ir_b = 19105.0;
        cybot_calib.ir_c = -111.12;
        break;
    }
    case 12:
    {
        cybot_calib.left_calibration_value = 22807;
        cybot_calib.right_calibration_value = 51019;
        break;
    }
    case 15:
    {
        cybot_calib.left_calibration_value = 21075;
        cybot_calib.right_calibration_value = 49945;
        cybot_calib.ir_a = -6873.6;
        cybot_calib.ir_b = 28419.0;
        cybot_calib.ir_c = 138.22;
        break;
    }
    }
    return cybot_calib;

}

