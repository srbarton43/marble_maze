/*
 * Initialize EXTIs
 *
 * UserButton:  PB4
 * Start_pos:   PA5
 * End_pos:     PA6
 * JoyStButton: PA10
 */


#include "ES28.h"

#include "EXTIinits.h"

void pb4_exti_init(void);
void pa5_exti_init(void);
void pa6_exti_init(void);
void pa10_exti_init(void);


void pb4_exti_init(void) {
	// disable global interrupt
	__disable_irq();

	// Enable clock access to GPIOB
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

	// Configure PB4 as input pin (not necessary since this is default reset value)
	GPIOB->MODER &= ~GPIO_MODER_MODE4_Msk;
	GPIOB->MODER |= (GPIO_INPUT << GPIO_MODER_MODE4_Pos);

	// enable pull-up for PB4
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD4_Msk;
	GPIOB->PUPDR |= (GPIO_PULLUP << GPIO_PUPDR_PUPD4_Pos);

	// Configure EXTICR2
	// Note: EXTICR1..4 are accessed as EXTICR[0..3]
	EXTI->EXTICR[1] &= ~EXTI_EXTICR2_EXTI4_Msk;
	EXTI->EXTICR[1] |= (EXTI_PB << EXTI_EXTICR2_EXTI4_Pos);

	// Select falling edge trigger for line EXTI4
	EXTI->FTSR1 |= EXTI_FTSR1_FT4;

	// Unmask line EXTI4
	EXTI->IMR1 |= EXTI_IMR1_IM4;

	// Enable EXTI4 and EXTI13 line in NVIC
	NVIC_EnableIRQ(EXTI4_15_IRQn);

	// Enable global interrupts
	__enable_irq();
}

void pa5_exti_init(void) {
	// disable global interrupt
	__disable_irq();

	// Enable clock access to GPIOA
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

	// Config PA5 as input pin
	GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
	GPIOA->MODER |= (GPIO_INPUT << GPIO_MODER_MODE5_Pos);

	//Configure EXTICR2
	EXTI->EXTICR[1] &= ~EXTI_EXTICR2_EXTI5_Msk;
	EXTI->EXTICR[1] |= (EXTI_PA << EXTI_EXTICR2_EXTI5_Pos);

	// Unmask line EXTI5
	EXTI->IMR1 |= EXTI_IMR1_IM5;

	// Select falling edge trigger for line EXTI5
	EXTI->FTSR1 |= EXTI_FTSR1_FT5;

	// Enable EXTI line in NVIC
	NVIC_EnableIRQ(EXTI4_15_IRQn);

	// Enable global interrupts
	__enable_irq();
}

void pa6_exti_init(void) {
	// disable global interrupt
	__disable_irq();

	// Enable clock access to GPIOA
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

	// Config PA6 as input pin
	GPIOA->MODER &= ~GPIO_MODER_MODE6_Msk;
	GPIOA->MODER |= (GPIO_INPUT << GPIO_MODER_MODE6_Pos);

	//Configure EXTICR2
	EXTI->EXTICR[1] &= ~EXTI_EXTICR2_EXTI6_Msk;
	EXTI->EXTICR[1] |= (EXTI_PA << EXTI_EXTICR2_EXTI6_Pos);

	// Unmask line EXTI6
	EXTI->IMR1 |= EXTI_IMR1_IM6;

	// Select rising edge trigger for line EXTI6
	EXTI->RTSR1 |= EXTI_RTSR1_RT6;

	// Enable EXTI line in NVIC
	NVIC_EnableIRQ(EXTI4_15_IRQn);

	// Enable global interrupts
	__enable_irq();
}


void pa10_exti_init(void) {
	// disable global interrupt
	__disable_irq();

	// Enable clock access to GPIOA
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

	// Config PA10 as input pin
	GPIOA->MODER &= ~GPIO_MODER_MODE10_Msk;
	GPIOA->MODER |= (GPIO_INPUT << GPIO_MODER_MODE10_Pos);

	// pullup for PA10
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD10_Msk;
	GPIOA->PUPDR |= (GPIO_PULLUP << GPIO_PUPDR_PUPD10_Pos);

	//Configure EXTICR3
	EXTI->EXTICR[2] &= ~EXTI_EXTICR3_EXTI10_Msk;
	EXTI->EXTICR[2] |= (EXTI_PA << EXTI_EXTICR3_EXTI10_Pos);

	// Unmask line EXTI10
	EXTI->IMR1 |= EXTI_IMR1_IM10;

	// Select falling edge trigger for line EXTI10
	EXTI->FTSR1 |= EXTI_FTSR1_FT10;

	// Enable EXTI line in NVIC
	NVIC_EnableIRQ(EXTI4_15_IRQn);

	// Enable global interrupts
	__enable_irq();
}



