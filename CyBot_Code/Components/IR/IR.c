/*
 * IR.c
 *
 *  Created on: Oct 18, 2022
 *      Author: neastbur
 */
#include <Components/cybot_select.h>
#include <Components/IR/IR.h>
#include <Components/Timer.h>
#include <inc/tm4c123gh6pm.h>
#include <math.h>


float calculate_ir_distance(int raw_ir_val)
{
    float result = (2 * ir_calib_a) / (float)((-ir_calib_b + sqrt(pow(ir_calib_b, 2) - (4 * ir_calib_a * (ir_calib_c - raw_ir_val)))));

    if(result <= 0)
    {
        return 0;
    }

    return result;
}

int compute_adc_sample_average(const int SAMPLE_SIZE)
{
    int sum = 0;
    int i;
    for(i = 0; i < SAMPLE_SIZE; ++i)
    {
        sum += (ADC0_SSFIFO0_R & 0xFFF);
    }

    return sum / SAMPLE_SIZE;
}

void set_average_amount(int avg)
{
    ADC0_SAC_R = log2(avg);
}

// Using ADC0 and SS0
void ir_init(cybot_calib_values_t calib)
{
    ir_calib_a = calib.ir_a;
    ir_calib_b = calib.ir_b;
    ir_calib_c = calib.ir_c;

    timer_init();

    // Enable ADC0 clock
    SYSCTL_RCGCADC_R |= 0x1;
    timer_waitMillis(1);

    SYSCTL_RCGCGPIO_R |= 0x2;
    timer_waitMillis(1);
    GPIO_PORTB_AMSEL_R |= 0x10;
    GPIO_PORTB_AFSEL_R &= 0xEF;
    GPIO_PORTB_DEN_R &= 0xEF;
    GPIO_PORTB_DIR_R &= 0xEF;

    // Disable SS3
    ADC0_ACTSS_R &= 0x1FFF7;

    // Set trigger source to be GPIO
    ADC0_EMUX_R &= 0xFFF;

    // Analog input sources - AIN10
    ADC0_SSMUX3_R = 0xA;

    //SS config
    ADC0_SSCTL3_R = 0x6;

    // Enable SS3
    ADC0_ACTSS_R |= 0x8;

    set_average_amount(0);

    // Start sampling
    ADC0_PSSI_R |= 0x8;
}

int ir_read()
{

    while(!(ADC0_RIS_R & 0x8))
    {

    }

    ADC0_ISC_R |= 0x8;

    ADC0_PSSI_R |= 0x8;

    return ADC0_SSFIFO3_R & 0xFFF;
}



