/**
  * @file    main.c
  * @brief   Test TIMER1 delay function with LED blinking
  * @author  Rubin Khadka
  */

#include "stm32f10x.h"
#include "timer1.h"
#include "gpio.h"

int main(void)
{
    // Initialize modules
    TIMER1_Init();
    LED_init();
    
    // Turn LED off initially
    LED_OFF();
    
    while(1)
    {
        // Turn LED on
        LED_ON();
        
        // Start 500ms non-blocking delay
        TIMER1_Delay_ms(2000);
        
        // While timer is running, we could do other tasks
        while(TIMER1_IsBusy())
        {
            // Do nothing - or we could add other tasks here later
            // For now, just wait (this is still polling, but cleaner)
            __NOP();  // Do nothing instruction
        }
        
        // Turn LED off
        LED_OFF();
        
        // Start another 500ms delay
        TIMER1_Delay_ms(2000);
        while(TIMER1_IsBusy())
        {
            __NOP();
        }
    }
}

