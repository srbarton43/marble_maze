/*
 * main.h
 *
 *  Created on: Mar 5, 2025
 *      Author: sbarton
 */

#ifndef MAIN_H_
#define MAIN_H_

#define MAX_ADC 4096

typedef enum {
	off, wait_for_ready, preStart, solving, timed_out, win, playback
} state_t;

#endif /* MAIN_H_ */
