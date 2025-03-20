/*
 * Control blinking lights based on input
 * Green: PB10
 * Red:   PB3
 *
 */
#ifndef LIGHTS_H_
#define LIGHTS_H_

void init_flashingLights(void);
void flashingLights(state_t state);

#endif /* LIGHTS_H_ */
