/*
 * joystick.c
 *
 *  Created on: Mar 4, 2025
 *      Author: sbarton
 */

#include "joystick.h"

#include "ES28.h"
#include "ADC.h"

void joystick_init(void) {
	adc_init();
	GPIOA->MODER &= ~GPIO_MODER_MODE1_Msk; // Set PA1 to analog (11)
	GPIOA->MODER |= (GPIO_ANALOG << GPIO_MODER_MODE1_Pos);
}

void getJoystickValues(uint16_t *X_value, uint16_t *Y_value) {
	// get joystick values;
    // first sample A0
    adc_setChannel(1);
    *X_value = adc_getValue();
    // next sample A1
    adc_setChannel(0);
    *Y_value = adc_getValue();
    // no need to convert to volts
}
