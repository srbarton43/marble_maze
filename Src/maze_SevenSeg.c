/*
 * maze_SevenSeg.c
 *
 *  Created on: Mar 5, 2025
 *      Author: sbarton
 */

#include "ES28.h"
#include "maze_SevenSeg.h"
#include "i2c.h"
#include "SevenSeg.h"

#define S_hex 0x6d
#define E_hex 0x79
#define t_hex 0x78
uint8_t SET_DISP[2 * HT16K33_NBUF] =
		{ S_hex, 0, E_hex, 0, 0, 0, t_hex, 0, 0, 0 };

#define P_hex 0x73
#define L_hex 0x38
#define A_hex 0x77
#define Y_hex 0x6E
uint8_t PLAY[2 * HT16K33_NBUF] = { P_hex, 0, L_hex, 0, 0, 0, A_hex, 0,
Y_hex, 0 };

#define r_hex 0x50
#define d_hex 0x5E

uint8_t RDY[2 * HT16K33_NBUF] = { r_hex, 0, d_hex, 0, 0, 0, Y_hex, 0, 0, 0 };

#define G_hex 0x3D
#define o_hex 0x5C
#define a_hex 0x5F
uint8_t GOAL[2 * HT16K33_NBUF] = { G_hex, 0, o_hex, 0, 0, 0, a_hex, 0,
L_hex, 0 };

#define O_hex 0x3F
#define F_hex 0x71

uint8_t OFF[2 * HT16K33_NBUF] = { O_hex, 0, F_hex, 0, 0, 0, F_hex, 0, 0, 0 };

uint8_t LOSE[2 * HT16K33_NBUF] = { L_hex, 0, O_hex, 0, 0, 0, S_hex, 0,
E_hex, 0 };

uint8_t EMPTY[2 * HT16K33_NBUF] = { 0 };

void maze_SevenSeg_init(void) {
	i2c1_init();
	SevenSeg_init();
}

void maze_SevenSeg_display(state_t state, uint8_t timer) {
	uint8_t *buffer_ptr;
	uint8_t number_buffer[2 * HT16K33_NBUF] = { 0 };
	switch (state) {
	case wait_for_ready:
		buffer_ptr = SET_DISP;
		break;
	case preStart:
		buffer_ptr = RDY;
		break;
	case win:
		buffer_ptr = GOAL;
		break;
	case timed_out:
		buffer_ptr = LOSE;
		break;
	case off:
		buffer_ptr = OFF;
		break;
	case solving:
		SevenSeg_number(timer, number_buffer);
		buffer_ptr = number_buffer;
		break;
	case playback:
		buffer_ptr = PLAY;
		break;
	}
	SevenSeg_write(buffer_ptr);
}
