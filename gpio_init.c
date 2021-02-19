/*
 * gpio_init.c
 *
 *  Created on: Jun 8, 2019
 *      Author: John Rodriguez
 */

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "gpio_init.h"
#include "main.h"

void gpio_init(void)
{
    EALLOW;

    //Configure EPWM pins.
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1; //Set pin 0 to periph 1. (EPWM1A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1; //Set pin 1 to periph 1. (EPWM1B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1; //Set pin 2 to periph 1. (EPWM2A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1; //Set pin 3 to periph 1. (EPWM2B)

    //Configure input pins
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0; //Enable pin 4 pull-up resistor.
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0; //Set pin 4 to be GPIO.
    GpioCtrlRegs.GPAQSEL1.bit.GPIO4 = 2; //Use a 6 sample qualification for pin 4.

    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0; //Enable pin 5 pull-up resistor.
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0; //Set pin 5 to be GPIO.
    GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = 2; //Use a 6 sample qualification for pin 5.

    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0; //Enable pin 32 pull-up resistor.
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0; //Set pin 6 to be GPIO.
    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 2; //Use a 6 sample qualification for pin 6.

    GpioCtrlRegs.GPACTRL.bit.QUALPRD0 = 0xFF;//Sampling period for pins 0-7 is 510 clock cycles.
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD0 = 0xFF;//Sampling period for pins 32-39 is 510 clock cycles.

    //Configure Output pins.
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0; //Set pin 7 to be GPIO.
    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1; //Initialize pin 7 to be low.
    GpioCtrlRegs.GPADIR.bit.GPIO7 = 1; //Set pin 7 to be an output.

    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0; //Set pin 8 to be GPIO.
    GpioDataRegs.GPACLEAR.bit.GPIO8 = 1; //Initialize pin 8 to be low.
    GpioCtrlRegs.GPADIR.bit.GPIO8 = 1; //Set pin 8 to be an output.

    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0; //Set pin 9 to be GPIO.
    GpioDataRegs.GPACLEAR.bit.GPIO9 = 1; //Initialize pin 9 to be low.
    GpioCtrlRegs.GPADIR.bit.GPIO9 = 1; //Set pin 9 to be an output.

    EDIS;
}



