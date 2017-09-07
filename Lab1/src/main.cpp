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

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Project Includes*/
#include "common_def.hh"

/* Hardware independent (hi) */
#include "hi/hi_def.hh"
#include "hi/hi_state_machine.hh"
#include "hi/hi_utils.hh"

/* Hardware dependent (hd) */
#include "hd/periph.hh"

periph::LampHandler lamp_handler(1);
periph::MicrophoneADC mic(ADC_SAMPLES_PER_SECOND, ADC_MEM0);
periph::LightSensor light(periph::LightSensor::CONFIG_DEFAULT_100MS);
periph::InputGPIO button(GPIO_PORT_P5, GPIO_PIN1);

Hi_dual_mean_fifo mic_fifo;

/*
 * Initialize sensor struct at time 0
 * Button not pressed
 * Light sensor detecting darkness
 * microphone detecting silence
 */

//FIXME: Add volatile
hi_sensor_t sensors = { 0, false, false, false };

static return_e hardware_init(void)
{
    //Stop Watchdog timer
    MAP_WDT_A_holdTimer();

    //User Button configuration
    button.configPullUp();
    button.enableInterrupt(GPIO_HIGH_TO_LOW_TRANSITION);

    //FIXME: Move Timer32 to class
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_16,
    TIMER32_32BIT,
                           TIMER32_PERIODIC_MODE);

    MAP_Interrupt_enableInterrupt(INT_T32_INT1);

    uint32_t T32_MaxCount = MAP_CS_getMCLK()
            / (TIMER32_PRESCALE * TIME_SAMPLES_PER_SECOND);

    /* Starting the timer */
    MAP_Timer32_startTimer(TIMER32_0_BASE, false);
    MAP_Timer32_setCount(TIMER32_0_BASE, T32_MaxCount);

    //Start Microphone ADC sampling
    mic.start();

    //Enable interrupts
    MAP_Interrupt_enableMaster();

    // FIXME: change this to contemplate possible
    // errors during initialization
    return RETURN_OK;
}

int main(void)
{
    // Program main finite state machine
    Hi_state_machine fsm;
    return_e rt;

    rt = hardware_init();
    if (rt != RETURN_OK)
        return 1;

    // Main loop counter
    uint64_t count = 0;

    while (1)
    {
        // Store microphone condition
        rt = mic_fifo.is_last_second_loud(&sensors.microphone);
        if (rt != RETURN_OK)
            break;

        // Read light sensor condition
        if (count % LIGHT_SENSOR_MAINLOOP_READ_FREQ == 0)
        {
            sensors.light_sensor = (light.read() >= LIGHT_THRESHOLD);
        }

        rt = fsm.handle_sensors(&sensors);
        if (rt != RETURN_OK)
            break;

        count++;
    }

    return 0;
}
