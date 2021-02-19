/*
 * PWM_init.h
 *
 *  Created on: Jun 22, 2019
 *      Author: John Rodriguez
 */
#include <stdint.h>

#ifndef INCLUDE_PWM_INIT_H_
#define INCLUDE_PWM_INIT_H_

uint16_t TBperiod(void);
uint16_t deadBand(void);
uint16_t dutyCyc(void);
void PWM_init(void);

extern volatile double amplitude;

#endif
