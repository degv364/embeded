/*
 * isr.cpp
 *
 *  Created on: Aug 29, 2017
 *      Author: ezamoraa
 */

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "msp.h"

/* Project Includes*/
#include "../../include/common_def.hh"
#include "../../include/hd/periph.h"

/* Hardware independent includes*/
#include "../../include/hi/hi_def.hh"
#include "../../include/hi/hi_utils.hh"


/*Variables defined in other files*/
extern periph::MicrophoneADC mic;

extern periph::LampHandler lamp_handler;
extern hi_sensor_t sensors;
extern Hi_dual_mean_fifo mic_fifo;

/*Interrupt Service Routines (ISR) Definition*/
extern "C"
{

void ADC14_IRQHandler(void)
{
    __disable_irq();

    if (periph::MicrophoneADC::checkAndCleanIRQ(ADC_INT0))
    {
        mic_fifo.add_sample(mic.read());
    }

    __enable_irq();
}

void PORT5_IRQHandler(void)
{
    __disable_irq();

    if (periph::InputGPIO::checkAndCleanIRQ(GPIO_PORT_P5, GPIO_PIN1))
    {
        sensors.control_button = true;
    }

    __enable_irq();
}
}

