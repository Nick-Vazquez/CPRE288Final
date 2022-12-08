#include <Components/servo.h>

int can_position_change(float current, int dir, float delta)
{
    if(dir == 1)
    {
        return current + delta <= 180;
    }
    else
    {
        return current - delta >= 0;
    }
}

// Via clock speeds and duty cycles in data sheet, not by calibration

//int determine_match(float degrees)
//{
//    // In ms
//    const float TIME = ((1/180.0) * degrees) + 1;
//
//    const int CYCLES = TIME * (CLOCK * pow(10, 6)) / 1000;
//
//    return START_VAL - CYCLES;
//}

// This is done by creating a linear relationship between the recorded match values at 180 and 0 degrees
int determine_match(float degrees)
{
    return (((left_val - right_val) / 180.0) * degrees) + right_val;
}

void servo_init(cybot_calib_values_t calib)
{
    left_val = calib.left_calibration_value + PRESCALE;
    right_val = calib.right_calibration_value + PRESCALE;

    timer_init();

    SYSCTL_RCGCTIMER_R |= 0x2;
    SYSCTL_RCGCGPIO_R |= 0x2;
    timer_waitMillis(1);

    // GPIO Config
    GPIO_PORTB_DEN_R |= 0x20;
    GPIO_PORTB_DIR_R |= 0x20;
    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFF0FFFFF) | 0x700000;
    GPIO_PORTB_AMSEL_R &= 0xDF;

    // Timer Config
    TIMER1_CFG_R = 0x4;
    TIMER1_TBMR_R = 0xA;

    // Default to 90 degrees
    TIMER1_TBPR_R = ((int)(START_VAL) >> 16);
    TIMER1_TBILR_R = START_VAL;
    TIMER1_TBPMR_R = determine_match(90.0) >> 16;
    TIMER1_TBMATCHR_R = determine_match(90.0);

    // Turn timer on
    TIMER1_CTL_R = 0x100;
}

void servo_move(float degrees)
{
    // Turn timer on
    TIMER1_TBPMR_R = determine_match(degrees) >> 16;
    TIMER1_TBMATCHR_R = determine_match(degrees);
}

void servo_calib(int bound_override)
{
    int s1_flag = 0;
    int s2_flag = 0;
    int s3_flag = 0;
    int s4_flag = 0;

    int dir = 1;

    int cont = 1;

    float servo_pos = 90.0;
    servo_move(servo_pos);

    lcd_init();

    // GPIO Setup
    SYSCTL_RCGCGPIO_R |= 0x10;
    GPIO_PORTE_DEN_R |= 0xF;
    GPIO_PORTE_DIR_R &= 0xF0;
    GPIO_PORTE_AFSEL_R &= 0xF0;
    GPIO_PORTE_AMSEL_R &= 0xF0;

    while(cont)
    {

        // Exit on S1 and S4
        if(!(GPIO_PORTE_DATA_R & 0x1) && !(GPIO_PORTE_DATA_R & 0x8))
        {
            cont = 0;
        }

        // S1
        if(!(GPIO_PORTE_DATA_R & 0x1) && !s1_flag && (can_position_change(servo_pos, dir, 1) || bound_override))
        {
            s1_flag = 1;
            servo_pos += (1 * dir);
            servo_move(servo_pos);

        }
        else if(GPIO_PORTE_DATA_R & 0x1)
        {
            s1_flag = 0;
        }

        // S2
        if(!(GPIO_PORTE_DATA_R & 0x2) && !s2_flag && (can_position_change(servo_pos, dir, 5) || bound_override))
        {
            s2_flag = 1;
            servo_pos += (5 * dir);
            servo_move(servo_pos);
        }
        else if(GPIO_PORTE_DATA_R & 0x2)
        {
            s2_flag = 0;
        }

        // S3
        if(!(GPIO_PORTE_DATA_R & 0x4) && !s3_flag)
        {
            s3_flag = 1;
            dir *= -1;
        }
        else if(GPIO_PORTE_DATA_R & 0x4)
        {
            s3_flag = 0;
        }

        // S4
        if(!(GPIO_PORTE_DATA_R & 0x8) && !s4_flag)
        {
            s4_flag = 1;

            if(dir == 1)
            {
                servo_pos = 180.0;
            }
            else
            {
                servo_pos = 0.0;
            }

            servo_move(servo_pos);
        }
        else if(GPIO_PORTE_DATA_R & 0x8)
        {
            s4_flag = 0;
        }

        lcd_printf("Match Prescale: %d\nMatch: %d\nServo Pos: %.1f\nDirection: %d", TIMER1_TBPMR_R, TIMER1_TBMATCHR_R, servo_pos, dir);
    }



}
