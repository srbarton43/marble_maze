/*
 * header file for the ADC module
 *
 * Samuel R Barton - February 2025
 */

#ifndef __ADC_H
#define __ADC_H

#define BITWIDTH_12 0b00
#define BITWIDTH_10 0b01
#define BITWIDTH_08 0b10
#define BITWIDTH_06 0b11

/*
 * initializes the ADC unit
 *
 * sets up PA0 (channel 0) as default analog input pin
 * default bitwidth=12 bits
 */
void adc_init (void);


/*
 * same as `adc_init()` but calibrates the adc
 */
void adc_init_calibrate(void);


/*
 * Changes the channel, disabling the old channel
 *
 * Valid channels are 0..=22
 */
void adc_setChannel(unsigned int chNum);

/*
 * Sets the ADC precision
 *
 * Use predefined bitwidth macros defined in header
 */
void adc_setWidth(int widthCode );

/*
 * Start a conversion and return its result
 */
int adc_getValue(void);

#endif
