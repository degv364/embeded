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


#include "lcd_driver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.hh"
#include "lcd_driver/Crystalfontz128x128_ST7735.hh"
#include <ti/grlib/grlib.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>

inline  void LCDWriteCommand(uint8_t i_u8Command);
inline void LCDWriteData(uint8_t i_u8Data);
inline void LCDSetOrientation(void);
inline void LCDDrawHorizontalLine(uint16_t i_u16X0, uint16_t i_u16X1, uint16_t i_u16Y, uint16_t i_u16Color);
inline void LCDDrawCompleteHorizontalLine(uint16_t i_u16Y, uint16_t i_u16Color);
inline void LCDDrawDevidedHorizontalLine(uint16_t i_u16XDivision, uint16_t i_u16Y,
					 uint16_t i_u16Color0,uint16_t i_u16Color1 );
inline void LCDDrawCompleteHorizontalRect(uint16_t i_u16Y0, uint16_t i_u16Y1, uint16_t i_u16Color);
uint32_t LCDColorTranslate(uint32_t i_u32Value);

