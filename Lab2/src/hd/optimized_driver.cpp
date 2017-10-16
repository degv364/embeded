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

static inline void LCDWriteCommand(uint8_t i_u8Command){
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

static inline void LCDWriteData(uint8_t i_u8Data){
      // USCI_B0 Busy? //
    while (UCB0STATW & UCBUSY);

    // Transmit data
    UCB0TXBUF = i_u8Data;

    // USCI_B0 Busy? //
    while (UCB0STATW & UCBUSY);
}

void LCDSetOrientation(void){
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


static inline void LCDDrawHorizontalLine(uint16_t i_u16X0, uint16_t i_u16X1, uint16_t i_u16Y, uint16_t i_u16Color){
  LCDSetDrawFrame(i_u16X0,i_u16Y,i_u16X1,i_u16Y);
  //Prepare for writing pixels
  LCDWriteCommand(CM_RAMWR);
  // Send pixels
  for (uint16_t l_u16Index =  i_u16X0; l_u16Index <=i_u16X1; l_u16Index++){
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
  }
}

static inline void LCDDrawCompleteHorizontalLine(uint16_t i_u16Y, uint16_t i_u16Color){
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

static  inline void LCDDrawDevidedHorizontalLine(uint16_t i_u16XDivision, uint16_t i_u16Y,
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


uint32_t LCDColorTranslate(uint32_t i_u32Value)
{
  return(((((i_u32Value) & 0x00f80000) >> 8) |
	  (((i_u32Value) & 0x0000fc00) >> 5) |
	  (((i_u32Value) & 0x000000f8) >> 3)));
}







void LCDDrawCompleteHorizontalRect(uint16_t i_u16Y0, uint16_t i_u16Y1, uint16_t i_u16Color){
  LCDSetDrawFrame(0,i_u16Y0,127,i_u16Y1);
  //Prepare for writing pixels
  LCDWriteCommand(CM_RAMWR);
  //Print row by row
  for (uint16_t l_u16Index =  i_u16Y0; l_u16Index <= i_u16Y1; l_u16Index++){
    // 0
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 1
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 2
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 3
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 4
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 5
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 6
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 7
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 8
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 9
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 10
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 11
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 12
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 13
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 14
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 15
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 16
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 17
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 18
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 19
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 20
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 21
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 22
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 23
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 24
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 25
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 26
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 27
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 28
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 29
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 30
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 31
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 32
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 33
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 34
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 35
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 36
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 37
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 38
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 39
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 40
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 41
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 42
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 43
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 44
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 45
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 46
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 47
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 48
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 49
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 50
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 51
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 52
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 53
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 54
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 55
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 56
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 57
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 58
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 59
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 60
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 61
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 62
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 63
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 64
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 65
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 66
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 67
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 68
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 69
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 70
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 71
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 72
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 73
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 74
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 75
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 76
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 77
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 78
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 79
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 80
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 81
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 82
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 83
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 84
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 85
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 86
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 87
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 88
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 89
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 90
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 91
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 92
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 93
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 94
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 95
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 96
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 97
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 98
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 99
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 100
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 101
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 102
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 103
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 104
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 105
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 106
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 107
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 108
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 109
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 110
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 111
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 112
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 113
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 114
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 115
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 116
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 117
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 118
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 119
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 120
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 121
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 122
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 123
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 124
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 125
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 126
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
    // 127
    LCDWriteData(i_u16Color>>8);
    LCDWriteData(i_u16Color);
  }
}

static int16_t g_i16B;
static int16_t g_i16Slope;

//////////////////////////////////////////////////////////////////////////////////////////
void LCDDrawDividedRectangle(uint16_t i_u16XLeft, uint16_t i_u16XRight,
			     uint16_t i_u16YBottom, uint16_t i_u16YTop,
			     uint16_t i_u16Y, uint16_t i_i16Slope,
			     uint16_t i_u16ColorSky, uint16_t i_u16ColorGround) {
  uint16_t l_i16B;
  // Limit slope from -127 to +127 
  i_i16Slope += 127;
  i_i16Slope &= 127;
  i_i16Slope -= 127;
  
  // Get line equation parameters
  l_i16B = (i_i16Slope == g_i16Slope)? g_i16B: (int16_t)i_u16Y-i_i16Slope*63;
  g_i16B = l_i16B;
  g_i16Slope = i_i16Slope;

  // Draw
  LCDSetDrawFrame(i_u16XLeft,i_u16YBottom,i_u16XRight,i_u16YTop);
  LCDWriteCommand(CM_RAMWR);
  // Note that slopes above 64 are slower. Because we draw by row
  for (int16_t l_i16YIndex = (int16_t) i_u16YBottom ; l_i16YIndex <= (int16_t)i_u16YTop; l_i16YIndex++){
    for (int16_t l_i16XIndex = (int16_t)i_u16XLeft; l_i16XIndex <= (int16_t)i_u16XRight; l_i16XIndex++){
      if (l_i16XIndex*i_i16Slope+l_i16B > l_i16YIndex){
	// Draw ground
	LCDWriteData(i_u16ColorGround>>8);
	LCDWriteData(i_u16ColorGround);	
      }
      else {
	LCDWriteData(i_u16ColorSky>>8);
	LCDWriteData(i_u16ColorSky);	
      }
    } 
  }
  
  
}
