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

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>

#include "hd/lcd_driver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.hh"
#include "hd/lcd_driver/Crystalfontz128x128_ST7735.hh"

#ifndef INCLUDE_HD_LCD_DRIVER_OPTIMIZED_HH_
#define INCLUDE_HD_LCD_DRIVER_OPTIMIZED_HH_

// Set default orientation Up
void LCDSetOrientation(void);
/**
 * Draw a complete Filled Rectangle.
 * \param i_u16Y0: Bottom of the rectangle
 * \param i_u16Y1: Top of the rectangle.
 * \param i_u16Color: Color to fill
 */
void LCDDrawCompleteHorizontalRect(uint16_t i_u16Y0, uint16_t i_u16Y1, uint16_t i_u16Color);

/**
 * Translate a 24 bit color into 16 bit value
 */
uint32_t LCDColorTranslate(uint32_t i_u32Value);

/**
 * Draw divided rectangle.
 * \param i_u16XLeft: Left of the rectangle (inclusive)
 * \param i_u16XRight: Right if the rectangle (inclusive) 
 * \param i_u16YBottom: Bottom of the rectangle (inclusive)
 * \param i_u16YTop: Top of the rectangle. (inclusive)
 * \param i_u16Y: related to pitch angle. Indicates the position at the center of the screen
 * \param i_i16Slope: line slope (Not angle). Obtained by multiplying float slope by 128
 * \param i_u16ColorSky: color to use for sky
 * \param i_u16ColorGround: color to use for ground
 */
void LCDDrawDividedRectangle(uint16_t i_u16XLeft, uint16_t i_u16XRight,
			     uint16_t i_u16YBottom, uint16_t i_u16YTop,
			     uint16_t i_u16Y, uint16_t i_i16Slope,
			     uint16_t i_u16Color);
#endif
