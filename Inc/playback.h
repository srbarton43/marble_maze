#ifndef __PLAYBACK_H
#define __PLAYBACK_H

#include "ES28.h"

void playback_init_arr(void);

void playback_record_input(uint32_t ms_elapsed, uint16_t joystickX,
		uint16_t joystickY);

void playback_solve_maze(void);

#endif/*__PLAYBACK_H*/
