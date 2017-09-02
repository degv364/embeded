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
#include "../../include/hd/definitions.h"
#include "../../include/hd/periph.h"


/*Variables defined in other files*/
extern periph::MicrophoneADC mic;
extern volatile uint_fast16_t micBuffer[UINT8_MAX];
extern volatile uint8_t micPos;

extern periph::OutputGPIO led;


/*Interrupt Service Routines (ISR) Definition*/
extern "C" {

    void ADC14_IRQHandler(void)
    {
        __disable_irq();

        if (periph::MicrophoneADC::checkAndCleanIRQ(ADC_INT0))
        {
            if(micPos == UINT8_MAX) micPos = 0;
            micBuffer[micPos++] = mic.read();
        }

        __enable_irq();
    }

    void PORT5_IRQHandler(void)
    {
        __disable_irq();

        if(periph::InputGPIO::checkAndCleanIRQ(GPIO_PORT_P5, GPIO_PIN1))
        {
            led.toggle();
            for(int j=0; j<100000; j++);
        }

        __enable_irq();
    }
}


