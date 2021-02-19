/*
 * PWM_init.c
 *
 *  Created on: Jun 22, 2019
 *      Author: John Rodriguez
 */

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "PWM_init.h"
#include "main.h"
#include <stdint.h>
#include <math.h>

const uint32_t CLK_FREQ = 150000000; //Clk speed is 150MHz
const uint32_t PWM_FREQ = 50000; //We want 50kHz PWM switching freq.
const float DEAD_BAND = 0.0000003; //We want 300ns deadband.
static uint16_t clksPerPeriod = 0;
volatile double amplitude = 0.30;

void PWM_init(void)
{
    clksPerPeriod = TBperiod();;

    //Disable the clock input to ePWM modules.
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    ///////////////////////
    //Setup EPWM1 module.//
    ///////////////////////
    //Configure time-based submodule (TB).
    EPwm1Regs.TBCTL.all = 0; //Clear TBCTL bits to get default state.
    EPwm1Regs.TBCTL.bit.CLKDIV = 0x00; // CLKDIV = 1
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0x00; // HSPCLKDIV = 1
    EPwm1Regs.TBCTL.bit.CTRMODE = 0x00; //Set PWM1 to Up-count mode.
    EPwm1Regs.TBCTL.bit.PRDLD = 0x00; //Enable TBPRD shadow register.
    EPwm1Regs.TBPRD = clksPerPeriod; //Set period of signal.

    //Configure counter-compare submodule (CC).
    EPwm1Regs.CMPCTL.all = 0; //Clear CMPCTL bits to get defaults.
    EPwm1Regs.CMPA.half.CMPA = dutyCyc();// Set CMPA (duty cycle control).
    EPwm1Regs.CMPB = 150; //(TBperiod()) / 2; //Used to trigger SOCA to ADC.

    //Configure action-quantifier submodule (AQ).
    EPwm1Regs.AQCTLA.all = 0; //Set to defaults.
    EPwm1Regs.AQCTLB.all = 0; //Set to defaults.
    EPwm1Regs.AQCTLA.bit.ZRO = 2; //PWMa high when count is zero.
    EPwm1Regs.AQCTLA.bit.CAU = 1; //PWMa low when count equals CMPA.
    EPwm1Regs.AQCTLB.bit.ZRO = 1; //PWMb low when count is zero.
    EPwm1Regs.AQCTLB.bit.CAU = 2; //PWMb high when count equals CMPA.

    //Configure dead-band submodule (DB) for active high complementary (AHC).
    EPwm1Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm1Regs.DBCTL.bit.POLSEL = 2;
    EPwm1Regs.DBCTL.bit.IN_MODE = 0;
    EPwm1Regs.DBRED = deadBand(); //Rising edge deadband.
    EPwm1Regs.DBFED = deadBand(); //Falling edge deadband.

    //Configure Event trigger and interrupt submodule (ET).
    EPwm1Regs.ETSEL.bit.INTSEL = 2; //EPWM1 INT generated when TBCTR = TBPRD.
    EPwm1Regs.ETPS.bit.INTPRD =  1; //Set EPWM1 INT to occur on first event (i.e. every ePWM1 period).
    EPwm1Regs.ETSEL.bit.INTEN = 1; //Enable EPWM INT generation.
    EPwm1Regs.ETSEL.bit.SOCASEL = 6; //SOCA signal generated for ADC when TBCTR=CMPB.
    EPwm1Regs.ETPS.bit.SOCAPRD = 1; //Set SOCA pulse to occur on first event.
    EPwm1Regs.ETSEL.bit.SOCAEN = 1; //Enable ADC SOCA signal.

    ///////////////////////
    //Setup EPWM2 module.//
    ///////////////////////
    //Configure time-based submodule (TB).
    EPwm2Regs.TBCTL.all = 0; //Clear TBCTL bits to get default state.
    EPwm2Regs.TBCTL.bit.CLKDIV = 0x00; // CLKDIV = 1
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0x00; // HSPCLKDIV = 1
    EPwm2Regs.TBCTL.bit.CTRMODE = 0x00; //Set PWM2 to Up-count mode.
    EPwm2Regs.TBCTL.bit.PRDLD = 0x00; //Enable TBPRD shadow register.
    EPwm2Regs.TBPRD = clksPerPeriod; //Set period of signal.

    //Configure counter-compare submodule (CC).
    EPwm2Regs.CMPCTL.all = 0; //Clear CMPCTL bits to get defaults.
    EPwm2Regs.CMPA.half.CMPA = dutyCyc();// Set CMPA (duty cycle control).

    //Configure action-quantifier submodule (AQ).
    EPwm2Regs.AQCTLA.all = 0; //Set to defaults.
    EPwm2Regs.AQCTLB.all = 0; //Set to defaults.
    EPwm2Regs.AQCTLA.bit.ZRO = 1; //PWMa low when count is zero.
    EPwm2Regs.AQCTLA.bit.CAU = 2; //PWMa high when count equals CMPA.
    EPwm2Regs.AQCTLB.bit.ZRO = 2; //PWMb high when count is zero.
    EPwm2Regs.AQCTLB.bit.CAU = 1; //PWMb low when count equals CMPA.

    //Configure dead-band submodule (DB) for active high complementary (AHC).
    EPwm2Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm2Regs.DBCTL.bit.POLSEL = 2;
    EPwm2Regs.DBCTL.bit.IN_MODE = 0;
    EPwm2Regs.DBRED = deadBand(); //Rising edge deadband.
    EPwm2Regs.DBFED = deadBand(); //Falling edge deadband.
}

uint16_t TBperiod(void)
{
    uint32_t x = 0;

    x = ((CLK_FREQ / PWM_FREQ) - 1);
    return (uint16_t) x;
}

uint16_t deadBand(void)
{
    double y = 0;

    y = (DEAD_BAND * CLK_FREQ);
    return (uint16_t) y;
}

uint16_t dutyCyc(void)
{
    double z = 0, t = 0, dutyCycle = 0;

    t = (double)count / PWM_FREQ;
    dutyCycle = 0.5 + amplitude*sin(2*PI*60*t);
    z = (dutyCycle * clksPerPeriod);
    return (uint16_t) z;
}
