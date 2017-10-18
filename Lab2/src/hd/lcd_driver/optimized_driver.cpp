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

#include "hd/lcd_driver/optimized_driver.hh"
#include "hd/periph.hh"


uint8_t Lcd_Orientation;
uint16_t Lcd_ScreenWidth, Lcd_ScreenHeigth;
uint8_t Lcd_PenSolid, Lcd_FontSolid, Lcd_FlagRead;
uint16_t Lcd_TouchTrim;


extern "C"
{
void __attribute__((naked))
SysCtlDelay(uint32_t ui32Count)
{
    __asm("    subs    r0, #1\n"
          "    bne     SysCtlDelay\n"
          "    bx      lr");
}
}


static void HAL_LCD_PortInit(void)
{
    // LCD_SCK
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_SCK_PORT, LCD_SCK_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
    // LCD_MOSI
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_MOSI_PORT, LCD_MOSI_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
    // LCD_RST
    GPIO_setAsOutputPin(LCD_RST_PORT, LCD_RST_PIN);
    // LCD_RS
    GPIO_setAsOutputPin(LCD_DC_PORT, LCD_DC_PIN);
    // LCD_CS
    GPIO_setAsOutputPin(LCD_CS_PORT, LCD_CS_PIN);
}

static void HAL_LCD_SpiInit(void)
{
    eUSCI_SPI_MasterConfig config =
        {
            EUSCI_B_SPI_CLOCKSOURCE_SMCLK,
            LCD_SYSTEM_CLOCK_SPEED,
            LCD_SPI_CLOCK_SPEED,
            EUSCI_B_SPI_MSB_FIRST,
            EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT,
            EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW,
            EUSCI_B_SPI_3PIN
        };
    SPI_initMaster(LCD_EUSCI_BASE, &config);
    SPI_enableModule(LCD_EUSCI_BASE);

    GPIO_setOutputLowOnPin(LCD_CS_PORT, LCD_CS_PIN);

    GPIO_setOutputHighOnPin(LCD_DC_PORT, LCD_DC_PIN);
}

static inline void LCDWriteCommand(uint8_t i_u8Command)
{

    GPIO_setOutputLowOnPin(LCD_DC_PORT, LCD_DC_PIN);
    // USCI_B0 Busy? //
    while (UCB0STATW & UCBUSY);

    // Transmit data
    UCB0TXBUF = i_u8Command;

    // USCI_B0 Busy? //
    while (UCB0STATW & UCBUSY);

    GPIO_setOutputHighOnPin(LCD_DC_PORT, LCD_DC_PIN);
}

static inline void LCDWriteData(uint8_t i_u8Data)
{
      // USCI_B0 Busy? //
    while (UCB0STATW & UCBUSY);

    // Transmit data
    UCB0TXBUF = i_u8Data;

    // USCI_B0 Busy? //
    while (UCB0STATW & UCBUSY);
}

#define LCD_ORIENTATION_DOWN  2
static inline void LCDSetDrawFrame(uint16_t i_u16X0, uint16_t i_u16Y0, uint16_t i_u16X1, uint16_t i_u16Y1)
{
    // adjust to orientation up

    i_u16X0 += 2;
    i_u16Y0 += 3;
    i_u16X1 += 2;
    i_u16Y1 += 3;

    // Send frame
    LCDWriteCommand(CM_CASET);
    LCDWriteData((uint8_t) (i_u16X0 >> 8));
    LCDWriteData((uint8_t) (i_u16X0));
    LCDWriteData((uint8_t) (i_u16X1 >> 8));
    LCDWriteData((uint8_t) (i_u16X1));

    LCDWriteCommand(CM_RASET);
    LCDWriteData((uint8_t) (i_u16Y0 >> 8));
    LCDWriteData((uint8_t) (i_u16Y0));
    LCDWriteData((uint8_t) (i_u16Y1 >> 8));
    LCDWriteData((uint8_t) (i_u16Y1));
}

void LCDInit(void)
{
    HAL_LCD_PortInit();
    HAL_LCD_SpiInit();

    GPIO_setOutputLowOnPin(LCD_RST_PORT, LCD_RST_PIN);
    HAL_LCD_delay(50);
    GPIO_setOutputHighOnPin(LCD_RST_PORT, LCD_RST_PIN);
    HAL_LCD_delay(120);

    LCDWriteCommand(CM_SLPOUT);
    HAL_LCD_delay(200);

    LCDWriteCommand(CM_GAMSET);
    LCDWriteData(0x04);

    LCDWriteCommand(CM_SETPWCTR);
    LCDWriteData(0x0A);
    LCDWriteData(0x14);

    LCDWriteCommand(CM_SETSTBA);
    LCDWriteData(0x0A);
    LCDWriteData(0x00);

    LCDWriteCommand(CM_COLMOD);
    LCDWriteData(0x05);
    HAL_LCD_delay(10);

    LCDWriteCommand(CM_MADCTL);
    LCDWriteData(CM_MADCTL_BGR);

    LCDWriteCommand(CM_NORON);

    Lcd_ScreenWidth  = LCD_VERTICAL_MAX;
    Lcd_ScreenHeigth = LCD_HORIZONTAL_MAX;
    Lcd_PenSolid  = 0;
    Lcd_FontSolid = 1;
    Lcd_FlagRead  = 0;
    Lcd_TouchTrim = 0;

    LCDSetDrawFrame(0, 0, LCD_VERTICAL_MAX-1, LCD_HORIZONTAL_MAX-1);
    LCDWriteCommand(CM_RAMWR);
    int i;
    for (i = 0; i < LCD_VERTICAL_MAX*LCD_HORIZONTAL_MAX; i++)
    {
        LCDWriteData(0xFF);
        LCDWriteData(0xFF);
    }

    HAL_LCD_delay(10);
    LCDWriteCommand(CM_DISPON);
}


static inline void LCDDrawHorizontalLine(uint16_t i_u16X0, uint16_t i_u16X1, uint16_t i_u16Y, uint16_t i_u16Color)
{
    LCDSetDrawFrame(i_u16X0, i_u16Y, i_u16X1, i_u16Y);
    //Prepare for writing pixels
    LCDWriteCommand(CM_RAMWR);
    // Send pixels
    for (uint16_t l_u16Index = i_u16X0; l_u16Index <= i_u16X1; l_u16Index++){
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
    }
}

static inline void LCDDrawCompleteHorizontalLine(uint16_t i_u16Y, uint16_t i_u16Color)
{
    LCDSetDrawFrame(0, i_u16Y, 127, i_u16Y);
    //Prepare for writing pixels
    LCDWriteCommand(CM_RAMWR);
    // Send pixels partially unroled, so that inlinning works
    for (uint8_t l_u8Index = 0; l_u8Index < 16; l_u8Index++) {
        //0
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        //1
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        //2
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        //3
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        //4
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        //5
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        //6
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        //7
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        //8
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
    }
}

static inline void LCDDrawDividedHorizontalLine(uint16_t i_u16XDivision, uint16_t i_u16Y,
					 uint16_t i_u16Color0,uint16_t i_u16Color1 )
{
    LCDSetDrawFrame(0, i_u16Y, 127, i_u16Y);
    //Prepare for writing pixels
    LCDWriteCommand(CM_RAMWR);
    // Send pixels
    for (uint8_t l_u8Index = 0; l_u8Index <= 127; l_u8Index++) {
        if (l_u8Index < i_u16XDivision) {
            LCDWriteData(i_u16Color0 >> 8);
            LCDWriteData(i_u16Color0);
        }
        else {
            LCDWriteData(i_u16Color1 >> 8);
            LCDWriteData(i_u16Color1);
        }
    }
}


void LCDSetOrientation(void)
{
    //always set orientation up
    LCDWriteCommand(CM_MADCTL);
    LCDWriteData(CM_MADCTL_MX | CM_MADCTL_MY | CM_MADCTL_BGR);
}


/*uint32_t*/ uint16_t LCDColorTranslate(uint32_t i_u32Value)
{
    return (((((i_u32Value) & 0x00f80000) >> 8)
            | (((i_u32Value) & 0x0000fc00) >> 5)
            | (((i_u32Value) & 0x000000f8) >> 3)));
}



void LCDDrawCompleteHorizontalRect(uint16_t i_u16Y0, uint16_t i_u16Y1, uint16_t i_u16Color)
{
    LCDSetDrawFrame(0, i_u16Y0, 127, i_u16Y1);
    //Prepare for writing pixels
    LCDWriteCommand(CM_RAMWR);
    //Print row by row
    for (uint16_t l_u16Index = i_u16Y0; l_u16Index <= i_u16Y1; l_u16Index++) {
        // 0
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 1
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 2
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 3
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 4
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 5
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 6
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 7
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 8
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 9
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 10
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 11
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 12
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 13
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 14
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 15
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 16
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 17
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 18
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 19
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 20
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 21
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 22
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 23
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 24
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 25
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 26
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 27
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 28
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 29
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 30
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 31
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 32
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 33
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 34
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 35
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 36
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 37
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 38
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 39
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 40
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 41
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 42
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 43
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 44
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 45
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 46
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 47
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 48
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 49
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 50
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 51
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 52
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 53
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 54
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 55
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 56
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 57
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 58
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 59
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 60
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 61
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 62
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 63
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 64
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 65
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 66
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 67
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 68
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 69
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 70
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 71
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 72
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 73
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 74
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 75
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 76
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 77
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 78
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 79
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 80
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 81
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 82
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 83
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 84
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 85
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 86
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 87
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 88
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 89
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 90
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 91
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 92
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 93
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 94
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 95
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 96
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 97
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 98
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 99
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 100
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 101
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 102
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 103
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 104
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 105
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 106
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 107
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 108
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 109
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 110
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 111
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 112
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 113
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 114
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 115
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 116
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 117
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 118
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 119
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 120
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 121
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 122
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 123
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 124
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 125
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 126
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
        // 127
        LCDWriteData(i_u16Color >> 8);
        LCDWriteData(i_u16Color);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// We know that squares are 32x32

void LCDDrawDividedRectangle(uint16_t i_u16XLeft, uint16_t i_u16YTop,
			     uint16_t i_u16Y, float i_fSlope, uint32_t i_u32Colors)
{
    int16_t l_i16B;
    int16_t l_i16RemainingPixelsForShift;
    uint16_t l_u16ShouldShift;
    uint16_t l_u16SelectedColor;
    uint16_t l_u16UpdateValue;

    //FIXME: Check if this is working
    // Limit slope from -127 to +127
//    i_i16Slope += 127;
//    i_i16Slope &= 127;
//    i_i16Slope -= 127;

    // Get line equation parameters
    l_i16B = (int16_t) i_u16Y - i_fSlope * 63;

    //FIXME: Remove after testing hardcoded m and b
    //l_i16B = 30;
    //i_i16Slope = 1;

    // Draw
    LCDSetDrawFrame(i_u16XLeft, i_u16YTop, i_u16XLeft + RECTANGLE_SIZE-1,
                    i_u16YTop + RECTANGLE_SIZE-1);
    LCDWriteCommand(CM_RAMWR);

  // Note that slopes above 64 are slower. Because we draw by row
  for (int16_t l_i16YIndex = (int16_t) i_u16YTop;
       l_i16YIndex <= (int16_t) i_u16YTop+RECTANGLE_SIZE-1; l_i16YIndex++) {

    // Solve equation for when there is a color shift
    if (i_fSlope == 0.0f){
      //Avoid exception
      l_i16RemainingPixelsForShift = INT16_MAX;
      l_u16UpdateValue = -1;
    }
    // Negative slope, start with blue, then turn brown
    else if(i_fSlope<0){
      l_u16UpdateValue = -1;
      l_i16RemainingPixelsForShift = (int16_t) ((float)(l_i16YIndex-l_i16B)/i_fSlope) -
	(int16_t)i_u16XLeft;
    }
    //Positive Slope, start with brown then turn blue
    else{ // i_fSlope>0
      l_u16UpdateValue = 1;
      l_i16RemainingPixelsForShift = (int16_t) ((float)(l_i16YIndex-l_i16B)/i_fSlope) -
	(int16_t)i_u16XLeft;
    }
    
    //FIXME: Unroll when sure draw is working properly

    for(uint16_t i = 0; i < 32; i++) {
        // When Remaining pixels is negative its MSB becomes 1. This bit shifts
        //when the color shift should hapen.

           // Update remaining bits
           l_i16RemainingPixelsForShift+=l_u16UpdateValue;

           l_u16ShouldShift = ((uint16_t)l_i16RemainingPixelsForShift) >> 15;

           // If we should shift, shift 16 bits to select the next color
           l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));


           //Write data
           LCDWriteData(l_u16SelectedColor >> 8);
           LCDWriteData(l_u16SelectedColor);
    }

//    // Send Pixel 0
//
//    // When Remaining pixels becomes negative its MSB becomes 1. This happens when the
//    // Color shift should happen. In that case the color is shifted to select the second one.
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//
//    // If we should shift, shift 16 bits to select the next color
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//
//    // Update remaining bits
//    l_i16RemainingPixelsForShift--;
//    //Write data
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//
//    //1
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//
//    //2
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //3
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //4
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //5
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //6
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //7
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //8
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //9
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //10
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //11
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //12
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //13
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //14
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //15
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //16
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //17
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //18
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //19
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //20
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //21
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //22
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //23
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //24
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //25
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //26
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //27
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //28
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //29
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //30
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
//    //31
//    l_u16ShouldShift = l_i16RemainingPixelsForShift >> 15;
//    l_u16SelectedColor = (uint16_t)(i_u32Colors >> (l_u16ShouldShift<<4));
//    l_i16RemainingPixelsForShift--;
//    LCDWriteData(l_u16SelectedColor >> 8);
//    LCDWriteData(l_u16SelectedColor);
  }
  
}
