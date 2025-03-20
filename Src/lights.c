/*
 * Control blinking lights based on input
 * Green: PB10
 * Red:   PB3
 *
 */

#include "main.h"
#include "ES28.h"

void init_flashingLights(void) {

	// Enable clock access to GPIOB
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

	// Config PB3 as output pin
	GPIOB->MODER &= ~GPIO_MODER_MODE3_Msk;
	GPIOB->MODER |= (GPIO_OUTPUT << GPIO_MODER_MODE3_Pos);

	// Config PB10 as output pin
	GPIOB->MODER &= ~GPIO_MODER_MODE10_Msk;
	GPIOB->MODER |= (GPIO_OUTPUT << GPIO_MODER_MODE10_Pos);
}

void flashingLights(state_t state) {
	int t1 = 0;     // ms timer
	int t2 = 1000;

	// turn off red and green
	GPIOB->ODR &= ~GPIO_ODR_OD3;	// PB3 (red)
	GPIOB->ODR &= ~GPIO_ODR_OD10;	// PB10 (green)

	if (state == wait_for_ready || state == preStart) {
		// turn on red
		GPIOB->ODR |= GPIO_ODR_OD3;	    // PB3 (red)
	} else if (state == solving) {
		// turn on green
		GPIOB->ODR |= GPIO_ODR_OD10;	    // PB10 (green)
	} else if (state == win) {
		t1 = milliseconds();
		t2 = milliseconds();
		while ((t1 - t2) < 2000) {
			t1 = milliseconds();
			// turn on green
			GPIOB->ODR |= GPIO_ODR_OD10;	    // PB10 (green)
			delay_ms(100);
			// turn off green
			GPIOB->ODR &= ~GPIO_ODR_OD10;	// PB10 (green)
			delay_ms(100);
		}
		// turn off green
	} else if (state == timed_out) {
		t1 = milliseconds();
		t2 = milliseconds();
		while ((t1 - t2) < 2000) {
			t1 = milliseconds();
			// turn on red
			GPIOB->ODR |= GPIO_ODR_OD3;	    // PB3 (red)
			delay_ms(100);
			// turn off red
			GPIOB->ODR &= ~GPIO_ODR_OD3;	// PB3 (red)
			delay_ms(100);
		}
	}
}
