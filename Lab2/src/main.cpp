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
 ******************************************************************************/

// Standard Includes
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <msp.h>
#include <ti/grlib/grlib.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

//----- Project Includes -----
// Common definitions
#include "common_def.hh"

//----- Hardware independent (hi) -----
#include "hi/hi_def.hh"
#include "hi/scheduler.hh"
#include "hi/filters.hh"
//Tasks related
#include "hi/task.hh"
#include "hi/tasks/adc_irq_task.hh"
#include "hi/tasks/calc_horizon_task.hh"
#include "hi/tasks/lcd_horizon.hh"

//----- Hardware dependent (hd) -----
#include "hd/periph.hh"



//----- Global object declarations -----

//Hardware dependent (hd)
periph::Timer timer(TIMER32_0_BASE, TIME_INTERRUPTS_PER_SECOND);

//Hardware independent (hi)
volatile uint64_t g_SystemTicks = 0; // - The system counter.
Scheduler g_MainScheduler;           // - Instantiate a Scheduler

//IRQ related tasks (global for ISR access)
AdcIRQTask g_AdcIRQTask;


//----- Static main functions -----

static return_e HardwareInit(void)
{
    //Stop Watchdog timer
    MAP_WDT_A_holdTimer();

    /* Initializes Clock System */
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    //Enable FPU unit
    MAP_FPU_enableModule();

    //FIXME: Migrate to LCD Task setup
    //g_LcdHorizon.Setup();

    //Timer32 configuration
    timer.enableInterrupt();
    timer.start();

    //Enable interrupts
    MAP_Interrupt_enableMaster();

    return RETURN_OK;
}

//----- Main program -----

int main(void)
{
    return_e rt;
    // filter
    MeanFilter LCDFilter;
    uint16_t l_u16FilteredHorizon;

    //Initialize hardware peripherals
    rt = HardwareInit();
    if (rt != RETURN_OK)
        return 1;

    // Define tasks
    CalcHorizonTask l_CalcHorizon;

    // Attach and set up tasks
    g_MainScheduler.attach(&g_AdcIRQTask);
    g_MainScheduler.attach(&l_CalcHorizon);

    // Setup tasks
    g_MainScheduler.setup();

    // Prepare schedule before first iteration.
    g_MainScheduler.PostAmble();

    //FIXME: Integrate with calc_horizon_task
    //LCDFilter.Setup(l_u16HorizonY);
    //LCDFilter.GetFilteredValue(&l_u16FilteredHorizon);
    //g_LcdHorizon.InitialDraw(l_u16FilteredHorizon);

    while (1)
    {
        //FIXME: Integrate with calc_horizon_task
        //LCDFilter.AddValue(l_u16HorizonY);
        //LCDFilter.GetFilteredValue(&l_u16FilteredHorizon);
        //g_LcdHorizon.UpdateDraw(l_u16FilteredHorizon);

        if (g_SystemTicks != g_MainScheduler.m_u64ticks)
        {
            //- Only execute the tasks if one tick has passed.
            g_MainScheduler.m_u64ticks = g_SystemTicks;
            g_MainScheduler.run();
            g_MainScheduler.PostAmble();
        }
    }

    return 0;
}
