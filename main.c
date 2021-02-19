/*
 * main.c
 *
 *  Created on: Jun 8, 2019
 *      Author: John Rodriguez
 */

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "main.h"
#include "interrupt_init.h"
#include "gpio_init.h"
#include "PWM_init.h"
#include "adc_init.h"
#include <stdint.h>

const double PI = 3.14159265358979323846;

volatile double DCvoltage = 0, DCcurrent = 0,
                ACvoltage = 0, ACcurrent = 0;

volatile uint16_t count = 0;


void main(void)
{
    // Initialize System Control
    // PLL, WatchDog, enable Peripheral Clocks
    InitSysCtrl();

#ifdef FLASH_LINK
    // Copy time critical code and Flash setup code to RAM
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

    // Call Flash Initialization to setup flash waitstates
    // This function must reside in RAM
    InitFlash();
#endif

    // Set the High-Speed Peripheral Clock Prescaler (HISPCP) such that HSPCLK = 25 MHz
    // HSPCLK = SYSCLKOUT/(HISPCP x 2)
    EALLOW;
#if CPU_FRQ_150MHZ
    SysCtrlRegs.HISPCP.all = 3;
#else
    SysCtrlRegs.HISPCP.all = 2;
#endif
    EDIS;


    //Initialize all device peripherals.
    interrupt_init();
    gpio_init();
    PWM_init();
    adc_init();

    if(GpioDataRegs.GPADAT.bit.GPIO4 == 0)//If power switch is on when MCU is booted.
    {
        //Enable the clock input to ePWM modules.
        EALLOW;
        SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
        EDIS;

        GpioDataRegs.GPASET.bit.GPIO9 = 1;//Turn on green LED.
    }

    while (1)
    {
        //Do nothing.
        asm(" NOP");
    }
}


