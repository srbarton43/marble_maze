/*
 * joystick.h
 *
 *  Created on: Mar 4, 2025
 *      Author: sbarton
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "ES28.h"


void joystick_init(void);
void getJoystickValues(uint16_t *X_value, uint16_t *Y_value);


#endif /* JOYSTICK_H_ */
