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

#include "hi/tasks/lcd_horizon.hh"

LcdHorizon::LcdHorizon(void)
{
    m_u16HorizonLevelY = 63;
    m_stUpdateRect = {0,0,0,0};
}

void LcdHorizon::Setup(void)
{
    // Initializes display hardware
    Crystalfontz128x128_Init();
    // Set default screen orientation
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // Initializes graphics context
    Graphics_initContext(&m_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setBackgroundColor(&m_sContext, GRAPHICS_COLOR_WHITE);
}

void LcdHorizon::InitialDraw(uint16_t i_u16InitialHorizonLevelY)
{
    m_u16HorizonLevelY = i_u16InitialHorizonLevelY;

    //Draw sky
    Graphics_setForegroundColor(&m_sContext, GRAPHICS_COLOR_DEEP_SKY_BLUE);
    m_stUpdateRect = {0,0,127,m_u16HorizonLevelY};
    Graphics_fillRectangle(&m_sContext, &m_stUpdateRect);

    //Draw ground
    Graphics_setForegroundColor(&m_sContext, GRAPHICS_COLOR_BROWN);
    m_stUpdateRect = {0,m_u16HorizonLevelY+1,127,127};
    Graphics_fillRectangle(&m_sContext, &m_stUpdateRect);
}

void LcdHorizon::UpdateDraw(uint16_t i_u16NewHorizonLevelY)
{
    if (i_u16NewHorizonLevelY-m_u16HorizonLevelY >= 0) {
        Graphics_setForegroundColor(&m_sContext, GRAPHICS_COLOR_DEEP_SKY_BLUE);
        m_stUpdateRect = {0,m_u16HorizonLevelY,127,i_u16NewHorizonLevelY};
        //(*g_sCrystalfontz128x128_funcs.pfnRectFill)(&g_sCrystalfontz128x128, &m_stUpdateRect, 15000);
        Graphics_fillRectangle(&m_sContext, &m_stUpdateRect);

    }
    else {
        Graphics_setForegroundColor(&m_sContext, GRAPHICS_COLOR_BROWN);
        m_stUpdateRect = {0,m_u16HorizonLevelY,127,i_u16NewHorizonLevelY};
        Graphics_fillRectangle(&m_sContext, &m_stUpdateRect);
    }

    m_u16HorizonLevelY = i_u16NewHorizonLevelY;
}

