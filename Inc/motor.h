/*
 * motor.h
 *
 *  Created on: Mar 4, 2025
 *      Author: sbarton
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "ES28.h"

void motor_init(void);
void move_motor(uint16_t adcX, uint16_t adcY);

#endif /* MOTOR_H_ */
