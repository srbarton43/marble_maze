/*
 * main.c
 *
 *  Created on: Mar 4, 2025
 *      Author: Sam Barton & Kevin Mahoney
 */

#include <stdio.h>
#include <stdlib.h>

#include "ES28.h"
#include "ADC.h"
#include "SevenSeg.h"
#include "i2c.h"
#include "uart.h"
#include "joystick.h"
#include "EXTIinits.h"
#include "motor.h"
#include "tim.h"
#include "playback.h"
#include "maze_SevenSeg.h"
#include "lights.h"

#define TIMER_MAX 50

void pb4_exti_init(void);

volatile uint8_t timer_flag = 0;    // global variable for 1s timer (TIM16)
volatile uint8_t start_pos = 0; // detects when marble leaves start position (PA5)
volatile uint8_t end_pos = 0;     // detects when marble hits end position (PA6)
volatile uint8_t UserButton = 0;	// detects whether on/off button pressed
volatile uint8_t JoystickButton = 0;	// detects whether on/off button pressed

int main() {

	uint8_t timer; 	// countdown timer
	state_t state = off;
	uint16_t joystickX, joystickY; // ADC value NOT voltage
	uint32_t start_time, cur_time; // used for move playback
	uint8_t playbackIsWritable = 1;

	// intiialize modules
	uart2_init();

	joystick_init();

	pb4_exti_init();
	pa5_exti_init();
	pa6_exti_init();
	pa10_exti_init();

	motor_init();

	tim16_ms_interrupt_init(1000);

	maze_SevenSeg_init();
	init_flashingLights();

	printf("Hello from STM32\r\n");
	printf("Marble Maze Program\r\n");

	// init the 7seg and LEDs
	// they are changed every state change
	maze_SevenSeg_display(state, 0);
	flashingLights(state);

	// start with empty playback memory
	playback_init_arr();

	// main loop
	while (1) {
		switch (state) {
		case off: // turn on the game
			if (UserButton) {
				printf("switching to wait_for_ready\r\n");
				UserButton = 0; // reset the user_button flag
				state = wait_for_ready;
				printf("switching to wait_for_ready\r\n");
				maze_SevenSeg_display(state, 0);
				flashingLights(state);
			}
			break;
		case wait_for_ready: // ensure sensors in correct position

			timer_flag = 0;    // global variable for 1s timer (TIM16)
			start_pos = 0; // detects when marble leaves start position (PA5)
			end_pos = 0;     // detects when marble hits end position (PA6)
			UserButton = 0;	// detects whether on/off button pressed
			JoystickButton = 0;	// detects whether on/off button pressed

			// read start and end sensors as digital values
			uint16_t start = (GPIOA->IDR & GPIO_IDR_ID5) >> GPIO_IDR_ID5_Pos;
			uint16_t end = (GPIOA->IDR & GPIO_IDR_ID6) >> GPIO_IDR_ID6_Pos;

			if (start && !end) {
				JoystickButton = 0;
				start_time = milliseconds();
				state = preStart;
				printf("switching to preStart\r\n");
				maze_SevenSeg_display(state, 0);
				flashingLights(state);
				delay_ms(500);
				start_pos = 0;
				JoystickButton = 0;
			}
			break;
		case preStart:
			if (JoystickButton) { // playback the moves if joystick pressed
				printf("switching to playback\r\n");
				state = playback;
				maze_SevenSeg_display(state, TIMER_MAX);
				flashingLights(state);
				JoystickButton = 0;
			} else if (start_pos) { // marble moves from start
				start_pos = 0;
				timer = TIMER_MAX; 	// initialize the timer
				timer_flag = 0;		// clear timer flag
				end_pos = 0;		// clear end_sensor flag
				state = solving;
				printf("switching to solving\r\n");
				maze_SevenSeg_display(state, TIMER_MAX);
				flashingLights(state);
			}
			break;
		case solving:
			if (end_pos) { // marble reaches end
				end_pos = 0;
				state = win;
				printf("switching to win\r\n");
				maze_SevenSeg_display(state, 0);
				flashingLights(state);
			}
			if (timer_flag) { // 1 sec timer tick
				maze_SevenSeg_display(state, timer); // show time on 7seg
				timer_flag = 0;
				if (timer == 0) { // timed out
					state = timed_out;
					printf("switching to timed_out\r\n");
					maze_SevenSeg_display(state, 0);
					flashingLights(state);
				} else {
					timer--;
				}
			}
			break;
		case timed_out:
			state = wait_for_ready;
			printf("switching to wait_for_ready\r\n");
			printf("saving playback to memory\r\n");
			playbackIsWritable = 0;
			maze_SevenSeg_display(state, 0);
			flashingLights(state);
			break;

		case playback:
			playback_solve_maze();
			printf("switching to wait_for_ready\r\n");
			state = wait_for_ready;
			maze_SevenSeg_display(state, 0);
			flashingLights(state);
			break;
		case win:
			// display time for 1 second
			maze_SevenSeg_display(solving, TIMER_MAX - timer);
			delay_ms(2000);
			printf("switching to wait_for_ready\r\n");
			printf("saving playback to memory\r\n");
			playbackIsWritable = 0;
			state = wait_for_ready;
			maze_SevenSeg_display(state, TIMER_MAX);
			flashingLights(state);
			break;
		} // switch

		// if in a valid state, allow motor control
		if (state == preStart || state == solving || state == wait_for_ready) {

			getJoystickValues(&joystickX, &joystickY);

			move_motor(joystickX, joystickY);

			// if not in wait_for_ready and not in deadzone, record moves for playback
			if ((state != wait_for_ready)
					&& ((abs(joystickX - MAX_ADC / 2) >= 500)
							|| (abs(joystickY - MAX_ADC / 2) >= 500))
					&& (playbackIsWritable)) {
				cur_time = milliseconds();
				playback_record_input(cur_time - start_time, joystickX,
						joystickY);
			}
		}

		// handle off button request
		if (state != off && UserButton) {
			UserButton = 0;
			printf("turning off now\r\n");
			state = off; // turn off game
			playback_init_arr(); // clear the playback memory
			playbackIsWritable = 1;
			maze_SevenSeg_display(state, 0);
			flashingLights(state);
		}

	} //loop

	return 0;
}

void EXTI4_15_IRQHandler(void) {
	// disable global interrupt
	__disable_irq();

	// Check that EXTI4 interrupted
	if (EXTI->FPR1 & EXTI_FPR1_FPIF4) {
		EXTI->FPR1 |= EXTI_FPR1_FPIF4; // Clear PR flag by writing 1 into it
		UserButton = 1;   			     // toggle flag
	}
	// Check EXTI5
	if (EXTI->FPR1 & EXTI_FPR1_FPIF5) {
		EXTI->FPR1 |= EXTI_FPR1_FPIF5;
		start_pos = 1;
	}

	// Check EXTI6
	if (EXTI->RPR1 & EXTI_RPR1_RPIF6) {
		EXTI->RPR1 |= EXTI_RPR1_RPIF6;
		end_pos = 1;
	}

	// Check EXTI10
	if (EXTI->FPR1 & EXTI_FPR1_FPIF10) {
		EXTI->FPR1 |= EXTI_FPR1_FPIF10;
		JoystickButton = 1;
	}
	// Enable global interrupts
	__enable_irq();
}

/*
 * interrupt handler for the TIM16
 */
void TIM16_IRQHandler(void) {
	__disable_irq();
	/* Clear UIF in status register */
	TIM16->SR &= ~TIM_SR_UIF;
	timer_flag = 1;
	__enable_irq();
}
