#ifndef TIM_H_
#define TIM_H_

#include <stdint.h>

void tim16_ms_interrupt_init(int milliseconds);
void tim14_pa7_pwm_init(uint16_t prescaleFactor, uint16_t timerMax);
void tim14_pwm_set(uint16_t pulse);

#endif /* TIM_H_ */
