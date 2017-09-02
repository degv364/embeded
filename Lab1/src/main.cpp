/*****************************************************************************
*
* Copyright (C) 2013 - 2016 Texas Instruments Incorporated - http://www.ti.com/
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the
*   distribution.
*
* * Neither the name of Texas Instruments Incorporated nor the names of
*   its contributors may be used to endorse or promote products derived
*   from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*
* MSP432 empty main.c template
*
* for GNU compiler
*
******************************************************************************/
/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Project Includes*/
#include "../include/common_def.hh"

/* Hardware independent */
#include "../include/hi/hi_def.hh"
#include "../include/hi/hi_state_machine.hh"
#include "../include/hi/hi_utils.hh"


/* Hardware dependent */
#include "../include/hd/periph.h"
// FIXME: remove once this functions are implemented
#include "../include/hd/stubs.hh"




volatile uint_fast16_t micBuffer[UINT8_MAX];
volatile uint8_t micPos;

periph::OutputGPIO led(GPIO_PORT_P1, GPIO_PIN0);
periph::MicrophoneADC mic(ADC_SAMPLES_PER_SECOND, ADC_MEM0);


int main(void)
{
    //Stop Watchdog timer
    MAP_WDT_A_holdTimer();

    //User Button configuration
    periph::InputGPIO button(GPIO_PORT_P5, GPIO_PIN1);
    button.configPullUp();
    button.enableInterrupt(GPIO_HIGH_TO_LOW_TRANSITION);

    //Start Microphone ADC sampling
    micPos = 0;
    mic.start();

    //Enable interrupts
    MAP_Interrupt_enableMaster();

    while(1)
    {
        if (micBuffer[micPos-1] > 8300)
            led.set();
        else
            led.reset();
    }

    return 0;
}
