#ifndef SEVENSEGPARTIAL_H_
#define SEVENSEGPARTIAL_H_

#include <stdint.h>

#define HT16K33_ADDR	      0x70		// I2C address

#define HT16K33_NBUF		  5			// Size of display buffer
										// HT16K33 has 8, we only need 5 for display

// HT16K33 commands & options
#define HT16K33_SYSTEM_CMD    0x20		// System setup
#define HT16K33_OSC_ON        0x01

#define HT16K33_DISPLAY_CMD   0x80		// Display setup
#define HT16K33_DISPLAY_ON    0x01
#define HT16K33_BLINK_OFF     0x00		// Display blinking
#define HT16K33_BLINK_HALFHZ  0x06
#define HT16K33_BLINK_1HZ     0x04
#define HT16K33_BLINK_2HZ     0x02

#define HT16K33_BRIGHT_CMD    0xE0		// Brightness, OR with brightness 0 to 15
#define HT16K33_ADDR_PTR      0x00		// Address pointer, OR with location 0 to 15

#define MINUS 				  0x40		// minus sign


// HEX to 7seg lookup table
static const uint8_t numbertable[] = {
/* .gfedcba -- OR numbertable entries with 0x80 to activate decimal point */
	0x3F, /* 0 */
	0x06, /* 1 */
	0x5B, /* 2 */
	0x4F, /* 3 */
	0x66, /* 4 */
	0x6D, /* 5 */
	0x7D, /* 6 */
	0x07, /* 7 */
	0x7F, /* 8 */
	0x6F, /* 9 */
	0x77, /* a */
	0x7C, /* b */
	0x39, /* C */
	0x5E, /* d */
	0x79, /* E */
	0x71, /* F */
 };

//Function prototypes
void SevenSeg_init(void);										// Initialize the display
void SevenSeg_write(uint8_t *display_buffer); 					// Write the display RAM
																// display_buffer is expected to be of size 2*HT16K33_NBUF
void SevenSeg_blink(uint8_t rate);								// Change display flashing
void SevenSeg_dim(uint8_t brightness); 							// Dim the display
void SevenSeg_number(uint16_t num, uint8_t *display_buffer);	// Convert 4-digit number to segments
																// and fill the display buffer


#endif /* SEVENSEGPARTIAL_H_ */
