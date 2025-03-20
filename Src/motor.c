/*
 * motor.c
 *
 *  Created on: Mar 4, 2025
 *      Author: sbarton
 */
#include <stdio.h>
#include <stdlib.h>

#include "motor.h"
#include "ES28.h"
#include "main.h"

#define STEPS 5
#define DELAY_MS 10

void motor_init(void) {
	// Enable clock access to GPIOB
	RCC->IOPENR |=
			(RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN | RCC_IOPENR_GPIOCEN);

	// Configure PB4 as input pin (not necessary since this is default reset value)
	GPIOA->MODER &= ~GPIO_MODER_MODE9_Msk;
	GPIOA->MODER |= (GPIO_OUTPUT << GPIO_MODER_MODE9_Pos);

	// Configure PB4 as input pin (not necessary since this is default reset value)
	GPIOC->MODER &= ~GPIO_MODER_MODE7_Msk;
	GPIOC->MODER |= (GPIO_OUTPUT << GPIO_MODER_MODE7_Pos);

	// Configure PB4 as input pin (not necessary since this is default reset value)
	GPIOB->MODER &= ~GPIO_MODER_MODE0_Msk;
	GPIOB->MODER |= (GPIO_OUTPUT << GPIO_MODER_MODE0_Pos);

	// Configure PB4 as input pin (not necessary since this is default reset value)
	GPIOA->MODER &= ~GPIO_MODER_MODE7_Msk;
	GPIOA->MODER |= (GPIO_OUTPUT << GPIO_MODER_MODE7_Pos);
}

void move_motor(uint16_t adcX, uint16_t adcY) {
	// handling X
	uint8_t xDeadzone = 1;
	if (adcX >= (MAX_ADC / 2 + 500)) {
		GPIOA->ODR |= GPIO_ODR_OD9;
		xDeadzone = 0;
	} else if (adcX <= (MAX_ADC / 2 - 500)) {
		//printf("LOW DIRX\r\n");
		xDeadzone = 0;
		GPIOA->ODR &= ~GPIO_ODR_OD9;
	}

	// handling Y
	uint8_t yDeadzone = 1;
	if (adcY >= MAX_ADC / 2 + 500) {
		GPIOB->ODR &= ~GPIO_ODR_OD0;
		yDeadzone = 0;
	} else if (adcY <= MAX_ADC / 2 - 500) {
		GPIOB->ODR |= GPIO_ODR_OD0;
		yDeadzone = 0;
	}
	if (!xDeadzone || !yDeadzone) {
		for (int i = 0; i < STEPS; i++) {
			if (!xDeadzone)
				GPIOC->ODR |= GPIO_ODR_OD7;
			if (!yDeadzone)
				GPIOA->ODR |= GPIO_ODR_OD7;
			delay_ms(DELAY_MS);
			if (!xDeadzone)
				GPIOC->ODR &= ~GPIO_ODR_OD7;
			if (!yDeadzone)
				GPIOA->ODR &= ~GPIO_ODR_OD7;
			delay_ms(DELAY_MS);
		}
	}
}
