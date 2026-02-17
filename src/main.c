/**
  * @file    main.c
  * @brief   Digital Thermometer
  * @author  Rubin Khadka
  */

#include "stm32f10x.h"
#include "timer1.h"
#include "adc.h"
#include "gpio.h"
#include "spi.h"
#include "seven_segment.h"

int main(void)
{
	
	DisplayDigits_t display_digits; // Initialise Digital Display structure
	
	// Initialize modules
	TIMER1_Init();
	LED_init();
	ADC_Init();
	SPI1_init();
	
	ADC_StartConversion(); // Start Temperature Converison 
	
	while(1)
	{
		// Read Temperature when conversion is done
		if (ADC_IsReady()) // If conversion is complete
		{
			float temperature = ADC_ReadCelsius(); // Get Temperature Value

			Temperature_To_Digit(temperature, &display_digits); // Convert to format for 7Segment
			Update_Display(&display_digits); // Display on 7Segment
			
			ADC_StartConversion();  // Start Next conversion
		}
	}
}

