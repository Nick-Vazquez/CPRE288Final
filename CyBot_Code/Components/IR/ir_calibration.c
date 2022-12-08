#include <Components/IR/IR.h>
#include <Components/lcd.h>
#include <Components/Timer.h>
#include <Components/uart.h>
#include <string.h>

#define START 9
#define END 50
#define STEPS 41
#define DELTA ((END - START) / STEPS)
#define SAMPLES 100

int button_flag = 0;

void button_wait()
{

    while((GPIO_PORTE_DATA_R & 0x1) || button_flag)
    {
        // If flag is asserted and button not being pressed, set button_flag to 0
        if(GPIO_PORTE_DATA_R & 0x1 && button_flag)
        {
            button_flag = 0;
        }
    }

    button_flag = 1;

    timer_waitMillis(250);
}

void ir_calibrate(cybot_calib_values_t calib)
{
    reading_t readings[STEPS + 1];
    int ir_vals[SAMPLES];
    int i;
    int j;

    char str[100] = "";

    // Set defaults
    for(i = 0; i <= STEPS; ++i)
    {
        readings[i].distance_recip = 0.0;
        readings[i].ir_val = 0;
    }

    for(i = 0; i < SAMPLES; ++i)
    {
        ir_vals[i] = 0;
    }

    lcd_init();
    timer_init();
    ir_init(calib);
    uart_init();
    set_average_amount(64);

    // Button 1 on PE0, active low

    // GPIO setup:
    SYSCTL_RCGCGPIO_R |= 0x10;
    timer_waitMillis(1);
    GPIO_PORTE_DEN_R |= 0x1;
    GPIO_PORTE_DIR_R &= 0xFE;
    GPIO_PORTE_AFSEL_R &= 0xFE;
    GPIO_PORTE_AMSEL_R &= 0xFE;

    lcd_printf("Setup PUTTY to write to a .csv file. Press button 1 when ready:");

    button_wait();

    for(i = 0; i <= STEPS; ++i)
    {
        lcd_printf("Move bot to %dcm: Press button 1 when done!", (i * DELTA) + START);
        button_wait();

        // Take readings and average
        // NOTE: 64 samples will be taken and averaged by the HW
        // Additional averaging for more precision
        for(j = 0; j < SAMPLES; ++j)
        {
            ir_vals[j] = ir_read();
        }

        readings[i].distance_recip = ((double) 1 / ((i * DELTA) + START));
        readings[i].ir_val = compute_sample_average_int(ir_vals, SAMPLES);

        sprintf(str, "%.10lf,%d\n", readings[i].distance_recip, readings[i].ir_val);
        uart_sendStr(str);

        lcd_clear();

    }

}
