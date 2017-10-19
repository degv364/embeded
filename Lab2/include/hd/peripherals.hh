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

#ifndef INCLUDE_PERIPH_H_
#define INCLUDE_PERIPH_H_

/* TI Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

/* Standard Includes */
#include <assert.h>
#include <stdint.h>

/* Internal Includes */
#include "common_def.hh"
#include "hd/hd_def.hh"
#include "hd/comm.hh"

/* Periph Includes */
#include "hd/gpio.hh"
#include "hd/timer.hh"
#include "hd/accel_adc.hh"

/* LCD Driver Includes */
//#include "lcd_driver/Crystalfontz128x128_ST7735.hh"
//#include "lcd_driver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.hh"
#include "lcd_driver/optimized_driver.hh"
#endif /* INCLUDE_PERIPH_H_ */
