#include "ES28.h"

#include "ADC.h"

// Initialize the ADC
void adc_init(void) {
	// Configure analog input pin for pa0
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;	// Enable clock access to GPIOA
	GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk; // Set PA0 to analog (11)
	GPIOA->MODER |= (GPIO_ANALOG << GPIO_MODER_MODE0_Pos);

	// Configure ADC module
	RCC->APBENR2 |= RCC_APBENR2_ADCEN;  // Enable clock access

	// Turn on the reference voltage
	ADC1->CR |= ADC_CR_ADVREGEN;

	// wait for the specified startup time (2 ms is plenty )
	delay_ms(2);

	// Now poke the calibrate bit and wait for it to finish
	ADC1->CR |= ADC_CR_ADCAL;
	while (ADC1->CR & ADC_CR_ADCAL) {
	}

	// set default channel to channel 0
	adc_setChannel(0);

	// set default width to 12 bits
	adc_setWidth(BITWIDTH_12);

	// the adc module is now enabled as a result of setting the bitwidth
}

void adc_init_calibrate(void) {
	// Configure analog input pin for pa0
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;	// Enable clock access to GPIOA
	GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk; // Set PA0 to analog (11)
	GPIOA->MODER |= (GPIO_ANALOG << GPIO_MODER_MODE0_Pos);

	// Configure ADC module
	RCC->APBENR2 |= RCC_APBENR2_ADCEN;  // Enable clock access

	// Turn on the reference voltage
	ADC1->CR |= ADC_CR_ADVREGEN;

	// wait for the specified startup time (2 ms is plenty )
	delay_ms(2);

	// Now poke the calibrate bit and wait for it to finish
	ADC1->CR |= ADC_CR_ADCAL;
	while (ADC1->CR & ADC_CR_ADCAL) {
	}

	// set default channel to channel 0
	adc_setChannel(0);

	// set default width to 12 bits
	adc_setWidth(BITWIDTH_12);

	// the adc module is now enabled as a result of setting the bitwidth
}

// Change to a new channel
void adc_setChannel(unsigned int chNum) {
	if (chNum > 22)  // invalid channel number (must be 0-22)
		return;

	ADC1->ISR = ADC_ISR_CCRDY;  // clear the CCRDY bit on the ADC_ISR register

	ADC1->CHSELR = 1UL << chNum;  // switch the channel (turns off old channel)

	//while ((ADC1->ISR & ADC_ISR_CCRDY) != 1){}// wait for channel configuration to apply

	// now the channel configuration has been applied and can return
}

// Set the precision using a macro in the header as widthCode
void adc_setWidth(int widthCode) {
	if ((ADC1->CR & ADC_CR_ADEN) == 1)
		ADC1->CR = ADC_CR_ADDIS;                       // disable the ADC
	while ((ADC1->CR & ADC_CR_ADDIS) == 1) {
	}        // wait for adc to finish disabling
	ADC1->CFGR1 &= ~ADC_CFGR1_RES_Msk;              // clear the res bits
	ADC1->CFGR1 |= widthCode << ADC_CFGR1_RES_Pos;  // set the res bits
	ADC1->ISR = ADC_ISR_ADRDY;  // Clear the ready bit by writing '1' to it
	ADC1->CR = ADC_CR_ADEN;                        // Set the ADC enable bit
}

// Start a conversion and return its result
int adc_getValue(void) {
	while ((ADC1->ISR & ADC_ISR_ADRDY) == 0) {
	}  // Wait for ADC to be ready

	ADC1->CR |= ADC_CR_ADSTART;  // Start a single conversion

	// Wait for conversion to finish
	while ((ADC1->ISR & ADC_ISR_EOC) == 0) {
	}

	// Read converted value
	return (uint16_t) ADC1->DR;
}
