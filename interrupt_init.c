/*
 * interrupt_init.c
 *
 *  Created on: Jul 16, 2019
 *      Author: John Rodriguez
 */

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "main.h"
#include "ISRs.h"

void interrupt_init(void)
{
    // Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    DINT;

    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    InitPieCtrl();

    // Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;

    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    InitPieVectTable();

    //Enter ISR pointers into PIE vector table.
    EALLOW;
    PieVectTable.XINT1 = &powerSwitch; //ISR for power switch.
    PieVectTable.XINT2 = &upButtonPress; //ISR for up voltage button.
    PieVectTable.XINT3 = &downButtonPress; //ISR for down voltage button.
    PieVectTable.EPWM1_INT = &epwm1Int; //ISR for EPWM1 interrupt.
    EDIS;

    //Enable the PIE block.
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;

    //Enable PIE interrupts
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1; //Corresponds to XINT1
    PieCtrlRegs.PIEIER1.bit.INTx5 = 1; //Corresponds to XINT2
    PieCtrlRegs.PIEIER12.bit.INTx1 = 1; //Corresponds to XINT3
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1; //Corresponds to EPWM1_INT

    //Enable CPU interrupts.
    IER |= M_INT1; //Enable CPU int1.
    IER |= M_INT3; //Enable CPU int3.
    IER |= M_INT12; //Enable CPU int12.

    // Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM


    //Configure GPIO pins to corresponding XINTn interrupt.
    EALLOW;
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 4; //XINT1 is pin 4.
    GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 5; //XINT2 is pin 5.
    GpioIntRegs.GPIOXINT3SEL.bit.GPIOSEL = 0; //XINT3 is pin 32.
    EDIS;

    //Configure XINTn interrupts.
    XIntruptRegs.XINT1CR.bit.POLARITY = 3; //XINT1 is falling & rising edge triggered.
    XIntruptRegs.XINT2CR.bit.POLARITY = 0; //XINT2 is falling edge triggered.
    XIntruptRegs.XINT3CR.bit.POLARITY = 0; //XINT3 is falling edge triggered.

    //Enable XINTn interrupts.
    XIntruptRegs.XINT1CR.bit.ENABLE = 1; //XINT1 enabled.
    XIntruptRegs.XINT2CR.bit.ENABLE = 1; //XINT2 enabled.
    XIntruptRegs.XINT3CR.bit.ENABLE = 1; //XINT3 enabled.
}


