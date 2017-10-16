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


#include "hd/optimized_driver.hh"

inline void LCDWriteCommand(uint8_t i_u8Command){
  // FIXME: inline this
  GPIO_setOutputLowOnPin(LCD_DC_PORT, LCD_DC_PIN);
  // USCI_B0 Busy? //
  while (UCB0STATW & UCBUSY);
  
  // Transmit data
  UCB0TXBUF = i_u8Command;
  
  // USCI_B0 Busy? //
  while (UCB0STATW & UCBUSY);
  
  // FIXME: inline this
  GPIO_setOutputHighOnPin(LCD_DC_PORT, LCD_DC_PIN);
}

inline void LCDWriteData(uint8_t i_u8Data){
      // USCI_B0 Busy? //
    while (UCB0STATW & UCBUSY);

    // Transmit data
    UCB0TXBUF = i_u8Data;

    // USCI_B0 Busy? //
    while (UCB0STATW & UCBUSY);
}

inline void LCDSetOrientation(void){
  //always set orientation up
  LCDWriteData(CM_MADCTL_MX | CM_MADCTL_MY | CM_MADCTL_BGR);
}

static inline void LCDSetDrawFrame(uint16_t i_u16X0, uint16_t i_u16Y0, uint16_t i_u16X1, uint16_t i_u16Y1){
  // adjust to orientation up
  
  i_u16X0 += 2;
  i_u16Y0 += 3;
  i_u16X1 += 2;
  i_u16Y1 += 3;

  // Send frame
  LCDWriteCommand(CM_CASET);
  LCDWriteData((uint8_t)(i_u16X0 >> 8));
  LCDWriteData((uint8_t)(i_u16X0));
  LCDWriteData((uint8_t)(i_u16X1 >> 8));
  LCDWriteData((uint8_t)(i_u16X1));

  LCDWriteCommand(CM_RASET);
  LCDWriteData((uint8_t)(i_u16Y0 >> 8));
  LCDWriteData((uint8_t)(i_u16Y0));
  LCDWriteData((uint8_t)(i_u16Y1 >> 8));
  LCDWriteData((uint8_t)(i_u16Y1));
  
}


inline void LCDDrawHorizontalLine(uint16_t i_u16X0, uint16_t i_u16X1, uint16_t i_u16Y, uint16_t i_u16Color){
  LCDSetDrawFrame(i_u16X0,i_u16Y,i_u16X1,i_u16Y);
  //Prepare for writing pixels
  LCDWriteCommand(CM_RAMWR);
  // Send pixels
  for (uint16_t l_u16Index =  i_u16X0; l_u16Index <=i_u16X1; l_u16Index++){
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
  }
}

inline void LCDDrawCompleteHorizontalLine(uint16_t i_u16Y, uint16_t i_u16Color){
  LCDSetDrawFrame(0,i_u16Y,127,i_u16Y);
  //Prepare for writing pixels
  LCDWriteCommand(CM_RAMWR);
  // Send pixels partially unroled, so that inlinning works
  for (uint8_t l_u8Index = 0; l_u8Index<16; l_u8Index++){
    //0
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    //1
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    //2
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    //3
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    //4
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    //5
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    //6
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    //7
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    //8
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
  }
}

inline void LCDDrawDevidedHorizontalLine(uint16_t i_u16XDivision, uint16_t i_u16Y,
					 uint16_t i_u16Color0,uint16_t i_u16Color1 ){
  LCDSetDrawFrame(0,i_u16Y,127,i_u16Y);
  //Prepare for writing pixels
  LCDWriteCommand(CM_RAMWR);
  // Send pixels
  for (uint8_t l_u8Index =  0; l_u8Index <=127; l_u8Index++){
    if (l_u8Index < i_u16XDivision){
      LCDWriteData(i_u16Color0>>8);
      LCDWriteData(i_u16Color0);
    }
    else {
      LCDWriteData(i_u16Color1>>8);
      LCDWriteData(i_u16Color1);
    }
  }
}

inline void LCDDrawCompleteHorizontalRect(uint16_t i_u16Y0, uint16_t i_u16Y1, uint16_t i_u16Color){
  LCDSetDrawFrame(0,i_u16Y0,127,i_u16Y1);
  //Prepare for writing pixels
  LCDWriteCommand(CM_RAMWR);
  uint16_t l_u16TotalPixels = 128*(i_u16Y1-i_u16Y0+1);
  for (uint16_t l_u16Index =  0; l_u16Index <= l_u16TotalPixels; l_u16Index++){
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
  }
}


uint32_t LCDColorTranslate(uint32_t i_u32Value)
{
  //
  // Translate from a 24-bit RGB color to a 5-6-5 RGB color.
  //
  return(((((i_u32Value) & 0x00f80000) >> 8) |
	  (((i_u32Value) & 0x0000fc00) >> 5) |
	  (((i_u32Value) & 0x000000f8) >> 3)));
}
