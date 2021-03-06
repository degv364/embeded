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

/* Project Includes */
#include "common_def.hh"
/* Hardware independent (hi) */
#include "hi/hi_def.hh"
#include "hi/hi_state_machine.hh"
#include "hi/hi_utils.hh"
/* Hardware dependent (hd) */
#include "hd/periph.hh"


//Global object declarations

//Hardware dependent (hd)
periph::LampHandler lamp_handler(ENABLED_LAMPS);
periph::MicrophoneADC mic(ADC_SAMPLES_PER_SECOND, ADC_MEM0);
periph::LightSensor light(periph::LightSensor::CONFIG_DEFAULT_100MS);
periph::InputGPIO button(GPIO_PORT_P4, GPIO_PIN1);
periph::Timer timer(TIMER32_0_BASE, TIME_INTERRUPTS_PER_SECOND);

//Hardware independent (hi)
Hi_dual_mean_fifo mic_fifo;
hi_sensor_t sensors = {
                        0,     // Initialize software timer to 0
                        false, // Button not pressed
                        false, // Light sensor detecting darkness
                        false  // Microphone detecting not loud condition
                      };


static return_e hardware_init(void)
{
    //Stop Watchdog timer
    MAP_WDT_A_holdTimer();

    //User Button configuration
    button.configPullUp();
    button.enableInterrupt(GPIO_HIGH_TO_LOW_TRANSITION);

    //Time Measurement configuration
    timer.enableInterrupt();
    timer.start();

    //Start Microphone ADC sampling
    mic.start();

    //Enable interrupts
    MAP_Interrupt_enableMaster();

    return RETURN_OK;
}


int main(void)
{
    //Program main FSM
    Hi_state_machine fsm;
    return_e rt;

    //Initialize hardware peripherals
    rt = hardware_init();
    if (rt != RETURN_OK) return 1;

    //Main loop counter
    uint64_t count = 0;

    while (1)
    {
        //Determine microphone condition
        rt = mic_fifo.is_last_second_loud(&sensors.microphone);
        if (rt != RETURN_OK) break;

        //Read light sensor condition
        if (count % LIGHT_SENSOR_MAINLOOP_READ_PERIOD == 0) {
            sensors.light_sensor = (light.read() >= LIGHT_THRESHOLD);
        }

        //Execute FSM
        rt = fsm.handle_sensors(&sensors);
        if (rt != RETURN_OK) break;

        count++;
    }

    return 0;
}
