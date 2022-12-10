#include <Components/ping.h>

double average_distance(double* samples, const int NUM_SAMPLES)
{
    double sum = 0.0;
    int i;

    for(i = 0; i < NUM_SAMPLES; ++i)
    {
        sum += *samples;
        samples++;
    }

    return sum / NUM_SAMPLES;
}

// Returns distance in cm
// The clock is divided by 2 because the sound wave travels twice the distance of the object
// (to the object and then back)
double calc_distance(int ping_time)
{

    return ((SOUND_SPEED / 2) *  (ping_time * (1.0 / (CLOCK * pow(10, 6)))));
}

// Stores the time in the ISR volatile variables.
// The value of -1 is used to denote the lack of a meaningful value.
void store_time()
{
    // Bits 0 - 15 contain the timer value, bits 23 - 16 contain the prescalar
    if(start_time < 0)
    {
        start_time = TIMER3_TBR_R;
    }
    else if(start_time > 0 && end_time < 0)
    {
        end_time = TIMER3_TBR_R;

        // Stop timer
        TIMER3_CTL_R &= 0xFEFF;
    }

    TIMER3_ICR_R |= 0x400;
}


// PING pin is PB3
void ping_init()
{
    start_time = -1;
    end_time = -1;

    // GPIO Config
    timer_init();

    SYSCTL_RCGCGPIO_R |= 0x2;
    SYSCTL_RCGCTIMER_R |= 0x8;
    timer_waitMillis(1);
    GPIO_PORTB_DEN_R |= 0x8;
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFF0FFF) | 0x7000;

    // Disable timer b event capture interrupt
    TIMER3_IMR_R &= 0xFBFF;

    // Enable NVIC
    NVIC_EN1_R |= 0x10;

    IntRegister(INT_TIMER3B, store_time);

    // Timer Config
    TIMER3_CFG_R = 0x4;
    TIMER3_TBMR_R = 0x7;
    TIMER3_CTL_R = 0xC00;
    TIMER3_TBPR_R = 0xFF;
    TIMER3_TBILR_R = 0xFFFF;
}

int ping_time()
{
    int result = 0;

    // Disable AFSEL
    GPIO_PORTB_AFSEL_R &= 0xF7;

    // Set GPIO to output
    GPIO_PORTB_DIR_R |= 0x8;

    // Assert PING
    GPIO_PORTB_DATA_R |= 0x8;
    timer_waitMicros(10);

    // De-assert PING
    GPIO_PORTB_DATA_R &= 0xF7;

    // Set GPIO to input
    GPIO_PORTB_DIR_R &= 0xF7;

    // Enable AFSEL
    GPIO_PORTB_AFSEL_R |= 0x8;

    // Clear timer B event capture
    TIMER3_ICR_R |= 0x400;

    // Enable timer b event capture interrupt
    TIMER3_IMR_R |= 0x400;

    // Reset timer values
    TIMER3_TBPR_R = 0xFF;
    TIMER3_TBILR_R = 0xFFFF;

    // Start timer
    TIMER3_CTL_R |= 0x100;

    while(start_time == -1 || end_time == -1)
    {

    }

    // Disable timer b event capture interrupt
    TIMER3_IMR_R &= 0xFBFF;

    // Clear timer B event capture
    TIMER3_ICR_R |= 0x400;

    result = start_time - end_time;
    start_time = -1;
    end_time = -1;

    return result;
}

double ping_distance()
{
    return calc_distance(ping_time());
}
