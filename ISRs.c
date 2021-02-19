/*
 * ISRs.c
 *
 *  Created on: Jul 18, 2019
 *      Author: John Rodriguez
 */

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "main.h"
#include "PWM_init.h"

__interrupt void powerSwitch(void)
{
    //Check for status of power switch, then take appropriate action.
    if(GpioDataRegs.GPADAT.bit.GPIO4 == 0)//If power switch is up/on.
    {
        GpioDataRegs.GPASET.bit.GPIO9 = 1;//Turn on green LED.

        EALLOW;
        SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;//Disable the clock input to ePWM modules.

        EPwm1Regs.AQCTLA.bit.ZRO = 2; //PWMa high when count is zero.
        EPwm1Regs.AQCTLA.bit.CAU = 1; //PWMa low when count equals CMPA.
        EPwm1Regs.AQCTLB.bit.ZRO = 1; //PWMb low when count is zero.
        EPwm1Regs.AQCTLB.bit.CAU = 2; //PWMb high when count equals CMPA.

        EPwm2Regs.AQCTLA.bit.ZRO = 1; //PWMa low when count is zero.
        EPwm2Regs.AQCTLA.bit.CAU = 2; //PWMa high when count equals CMPA.
        EPwm2Regs.AQCTLB.bit.ZRO = 2; //PWMb high when count is zero.
        EPwm2Regs.AQCTLB.bit.CAU = 1; //PWMb low when count equals CMPA.

        SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;//Enable the clock input to ePWM modules.
        EDIS;
    }
    else //If power switch is down/off.
    {
        GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;//Turn off green LED.

        EALLOW;
        SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;//Disable the clock input to ePWM modules.

        EPwm1Regs.AQCTLA.bit.ZRO = 1; //PWMa low when count is zero.
        EPwm1Regs.AQCTLA.bit.CAU = 1; //PWMa low when count equals CMPA.
        EPwm1Regs.AQCTLB.bit.ZRO = 1; //PWMb low when count is zero.
        EPwm1Regs.AQCTLB.bit.CAU = 1; //PWMb low when count equals CMPA.

        EPwm2Regs.AQCTLA.bit.ZRO = 1; //PWMa low when count is zero.
        EPwm2Regs.AQCTLA.bit.CAU = 1; //PWMa low when count equals CMPA.
        EPwm2Regs.AQCTLB.bit.ZRO = 1; //PWMb low when count is zero.
        EPwm2Regs.AQCTLB.bit.CAU = 1; //PWMb low when count equals CMPA.

        SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;//Enable the clock input to ePWM modules.
        EDIS;
    }

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; //Clear the interrupt.
}

__interrupt void epwm1Int(void)
{
    if(count == 49999)
    {
        count = 0;
    }
    else
    {
        count++;
    }

    EPwm1Regs.CMPA.half.CMPA = dutyCyc(); //Set EPWM1 CMPA (duty cycle control).
    EPwm2Regs.CMPA.half.CMPA = dutyCyc(); // Set EPWM2 CMPA (duty cycle control).

    if(GpioDataRegs.GPADAT.bit.GPIO4 == 0) //If power switch is on.
    {
        if((amplitude == 0.30)&&(GpioDataRegs.GPADAT.bit.GPIO7 == 0))
        {
            GpioDataRegs.GPASET.bit.GPIO7 = 1;//Turn on blue LED.
        }
        else if((amplitude < 0.30)&&(GpioDataRegs.GPADAT.bit.GPIO7 == 1)&&(GpioDataRegs.GPADAT.bit.GPIO4 == 0))
        {
            GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;//Turn off blue LED.
        }
    }
    else //Power switch is off.
    {
        GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;//Turn off blue LED.
    }

    //Update ADC values.
    DCvoltage = ((AdcRegs.ADCRESULT0 >> 4)*3) / 4096.0;
    DCcurrent = ((AdcRegs.ADCRESULT1 >> 4)*3) / 4096.0;
    ACvoltage = ((AdcRegs.ADCRESULT2 >> 4)*3) / 4096.0;
    ACcurrent = ((AdcRegs.ADCRESULT3 >> 4)*3) / 4096.0;

    EPwm1Regs.ETCLR.bit.INT = 1; //Clear the EPWM1 interrupt flag.
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3; //Clear the interrupt.
}

__interrupt void upButtonPress(void)
{
    if(GpioDataRegs.GPADAT.bit.GPIO4 == 0) //If power switch is on.
    {
        if((GpioDataRegs.GPBDAT.bit.GPIO32 == 0) && (PieCtrlRegs.PIEIFR12.bit.INTx1 == 0))
        {
            amplitude = 0.30;
        }
        else if(amplitude < 0.29)
        {
            amplitude += 0.03;
        }
    }

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; //Clear the interrupt.
}

__interrupt void downButtonPress(void)
{
    if(GpioDataRegs.GPADAT.bit.GPIO4 == 0) //If power switch is on.
    {
        if((GpioDataRegs.GPADAT.bit.GPIO5 == 0) && (PieCtrlRegs.PIEIFR1.bit.INTx5 == 0))
        {
            amplitude = 0.30;
        }
        else if(amplitude > 0.01)
        {
            amplitude -= 0.03;
        }
    }

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12; //Clear the interrupt.
}

