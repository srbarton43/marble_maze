#include "playback.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ES28.h"
#include "motor.h"

#define BUFFER_SIZE 512

typedef struct input {
	uint32_t ms_elapsed;
	uint16_t joystickX;
	uint16_t joystickY;
} input_t;

// global
input_t input_buffer[BUFFER_SIZE];
uint16_t last_input_index;

// clears the buffer and sets up global list
void playback_init_arr(void) {
	// clear
	memset(input_buffer, 0, BUFFER_SIZE * sizeof(input_t));
	// set struct fields
	last_input_index = 0;
}

void playback_record_input(uint32_t ms_elapsed, uint16_t joystickX,
		uint16_t joystickY) {
	if (last_input_index == BUFFER_SIZE) {
		printf("ERROR: Reached BUFFER_SIZE");
		return;
	}
	input_t new_input = { ms_elapsed, joystickX, joystickY };
	printf("storing\t{ %ld,\t%d,\t%d }\r\n", ms_elapsed, joystickX, joystickY);
	input_buffer[last_input_index] = new_input;
	last_input_index++;
}

void playback_solve_maze(void) {
	int index = 0;
	input_t next_input = input_buffer[index];
	uint32_t start_time = milliseconds(), current_time;
	printf("Playing Back\r\n");
	while (1) {
		current_time = milliseconds();
		if (current_time - start_time >= next_input.ms_elapsed) {
			move_motor(next_input.joystickX, next_input.joystickY);
			printf("playing\t{ %ld,\t%d,\t%d }\r\n", next_input.ms_elapsed,
					next_input.joystickX, next_input.joystickY);
			next_input = input_buffer[++index];
		}
		if (index >= last_input_index)
			break;
	}
}
