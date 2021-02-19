/*
 * adc_init.c
 *
 *  Created on: Jul 11, 2019
 *      Author: John Rodriguez
 */

#include "main.h"
#include "gpio_init.h"
#include "PWM_init.h"
#include "adc_init.h"
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

void adc_init(void)
{
    //Must call this function for initial calibration.
    InitAdc();

    //Disable interrupt for SEQ1.
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0;

    //Set the SEQ1 start trigger to be ePWM1A start-of-conversion (SOCA) signal.
    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;

    //Set # of conversions (i.e. inputs) per autoconversion session to allow 4 signals.
    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 3;

    //Setup the channels to sequence.
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0; //ADCINA0 pin
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 1; //ADCINA1 pin
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 2; //ADCINA2 pin
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 3; //ADCINA3 pin

    //Enable interrupt for SEQ1.
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;
}




