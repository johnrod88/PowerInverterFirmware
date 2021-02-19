/*
 * ISRs.h
 *
 *  Created on: Jul 18, 2019
 *      Author: John Rodriguez
 */

#ifndef INCLUDE_ISRS_H_
#define INCLUDE_ISRS_H_

__interrupt void powerSwitch(void);
__interrupt void epwm1Int(void);
__interrupt void upButtonPress(void);
__interrupt void downButtonPress(void);

#endif 
