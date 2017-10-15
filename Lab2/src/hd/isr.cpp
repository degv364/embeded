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
#include "hi/scheduler.hh"

// Tasks
#include "hi/tasks/adc_irq_task.hh"

/*Variables defined in other files*/

extern AdcIRQTask g_AdcIRQTask;

// Global Main Scheduler
extern Scheduler g_MainScheduler;

//Global Tick Counter
extern volatile uint64_t g_SystemTicks;


/*Interrupt Service Routines (ISR) Definition*/
extern "C"
{

/* ISR activated by Timer32-1 for time measurement through the
 * software timer update
*/
void T32_INT1_IRQHandler(void)
{
    __disable_irq();

    periph::Timer::cleanIRQ(TIMER32_0_BASE);
    //FIXME: Check if frame is finished
    g_SystemTicks++;

    __enable_irq();
}


/* ISR activated by Timer32-2 for ADC sampling frequency
*/
void T32_INT2_IRQHandler(void)
{
    __disable_irq();

    periph::Timer::cleanIRQ(TIMER32_1_BASE);
    MAP_ADC14_toggleConversionTrigger();

    __enable_irq();
}

/* ISR that manages the ADC Accelerometer reading
 * sampling frequency
*/
void ADC14_IRQHandler(void)
{
    __disable_irq();

    if (periph::AccelADC::CheckAndCleanIRQ(ADC_INT2)
        && !g_AdcIRQTask.IsTaskFinished())
    {
      // Create message with accel data
      uint32_t* l_pHeapADC = g_AdcIRQTask.m_pHeapMem;

      l_pHeapADC[0] = (uint32_t) ADC14_getResult(ADC_MEM0);
      l_pHeapADC[1] = (uint32_t) ADC14_getResult(ADC_MEM1);
      l_pHeapADC[2] = (uint32_t) ADC14_getResult(ADC_MEM2);

      message_t l_stAccelDataMessage = {ADC_IRQ,
                                        CALC_HORIZON,
                                        ACCEL_DATA,
                                        3,
                                        l_pHeapADC};
      // Create execution message
      l_pHeapADC[3] = (uint32_t) CALC_HORIZON;

      message_t l_stExecuteHandler = {ADC_IRQ,
                                      SCHEDULER,
                                      ADD_TO_EXECUTION,
                                      1,
                                      &l_pHeapADC[3]};

      // Send messages
      g_AdcIRQTask.Outgoing.AddMessage(l_stExecuteHandler);
      g_AdcIRQTask.Outgoing.AddMessage(l_stAccelDataMessage);

      //Finished task handling for the current frame
      g_AdcIRQTask.SetFinishedState(true);
    }

    __enable_irq();
}

}

