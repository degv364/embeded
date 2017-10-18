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

#ifndef INCLUDE_HD_LCD_DRIVER_OPTIMIZED_HH_
#define INCLUDE_HD_LCD_DRIVER_OPTIMIZED_HH_

#include "hd/periph.hh"

// LCD Screen Dimensions
#define LCD_VERTICAL_MAX                   128
#define LCD_HORIZONTAL_MAX                 128

// ST7735 LCD controller Command Set
#define CM_NOP             0x00
#define CM_SWRESET         0x01
#define CM_RDDID           0x04
#define CM_RDDST           0x09
#define CM_SLPIN           0x10
#define CM_SLPOUT          0x11
#define CM_PTLON           0x12
#define CM_NORON           0x13
#define CM_INVOFF          0x20
#define CM_INVON           0x21
#define CM_GAMSET          0x26
#define CM_DISPOFF         0x28
#define CM_DISPON          0x29
#define CM_CASET           0x2A
#define CM_RASET           0x2B
#define CM_RAMWR           0x2C
#define CM_RGBSET          0x2d
#define CM_RAMRD           0x2E
#define CM_PTLAR           0x30
#define CM_MADCTL          0x36
#define CM_COLMOD          0x3A
#define CM_SETPWCTR        0xB1
#define CM_SETDISPL        0xB2
#define CM_FRMCTR3         0xB3
#define CM_SETCYC          0xB4
#define CM_SETBGP          0xb5
#define CM_SETVCOM         0xB6
#define CM_SETSTBA         0xC0
#define CM_SETID           0xC3
#define CM_GETHID          0xd0
#define CM_SETGAMMA        0xE0
#define CM_MADCTL_MY       0x80
#define CM_MADCTL_MX       0x40
#define CM_MADCTL_MV       0x20
#define CM_MADCTL_ML       0x10
#define CM_MADCTL_BGR      0x08
#define CM_MADCTL_MH       0x04


// System clock speed (in Hz)
#define LCD_SYSTEM_CLOCK_SPEED                 48000000
// SPI clock speed (in Hz)
#define LCD_SPI_CLOCK_SPEED                    16000000

// Ports from MSP432 connected to LCD
#define LCD_SCK_PORT          GPIO_PORT_P1
#define LCD_SCK_PIN_FUNCTION  GPIO_PRIMARY_MODULE_FUNCTION
#define LCD_MOSI_PORT         GPIO_PORT_P1
#define LCD_MOSI_PIN_FUNCTION GPIO_PRIMARY_MODULE_FUNCTION
#define LCD_RST_PORT          GPIO_PORT_P5
#define LCD_CS_PORT           GPIO_PORT_P5
#define LCD_DC_PORT           GPIO_PORT_P3

// Pins from MSP432 connected to LCD
#define LCD_SCK_PIN           GPIO_PIN5
#define LCD_MOSI_PIN          GPIO_PIN6
#define LCD_RST_PIN           GPIO_PIN7
#define LCD_CS_PIN            GPIO_PIN0
#define LCD_DC_PIN            GPIO_PIN7

// Definition of USCI base address to be used for SPI communication
#define LCD_EUSCI_BASE        EUSCI_B0_BASE


#define HAL_LCD_delay(x)  SysCtlDelay(x * 48)


// Initialization
void LCDInit(void);

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
/*uint32_t*/ uint16_t LCDColorTranslate(uint32_t i_u32Value);

#define RECTANGLE_SIZE 32

/**
 * Draw divided rectangle.
 * \param i_u16XLeft: Left of the rectangle (inclusive) 
 * \param i_u16YTop: Top of the rectangle. (inclusive)
 * \param i_u16Y: related to pitch angle. Indicates the position at the center of the screen
 * \param i_i16Slope: line slope (Not angle). Obtained by multiplying float slope by 128
 * \param i_u32Colors: MSB sky color, LSB ground color
 */
void LCDDrawDividedRectangle(uint16_t i_u16XLeft, uint16_t i_u16YTop,
			     uint16_t i_u16Y, float i_fSlope, uint32_t i_u32Colors);
#endif
