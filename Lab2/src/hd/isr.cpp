/**
 Copyright 2017 Daniel Garcia Vaglio <degv364@gmail.com> Esteban Zamora Alvarado <estebanzacr.20@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 **/

/* Standard Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "msp.h"

/* Project Includes*/
#include "common_def.hh"
#include "hd/periph.hh"
#include "hi/hi_def.hh"
#include "hi/hi_utils.hh"

/*Variables defined in other files*/
extern volatile uint64_t g_SystemTicks;

/*Interrupt Service Routines (ISR) Definition*/
extern "C"
{

/* ISR that manages the microphone reading. Stores the current
 * ADC sample (after some processing) in the microphone circular
 * buffer for later analysis
 */
void ADC14_IRQHandler(void)
{
    __disable_irq();
    //FIXME: Implement ADC ISR

    __enable_irq();
}

/* ISR activated by Timer32 for time measurement through the
 * software timer update
*/
void T32_INT1_IRQHandler(void)
{
    __disable_irq();

    periph::Timer::cleanIRQ(TIMER32_0_BASE);
    g_SystemTicks++;

    __enable_irq();
}

}

