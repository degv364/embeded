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

#include "hi/tasks/lcd_draw_task.hh"
#include "hd/optimized_driver.hh"

LcdDrawTask::LcdDrawTask(void)
{
    Task::SetTaskName(LCD_DRAW);
    Task::SetTaskType(ONE_SHOT);
    m_u16SkyColor = (uint16_t)LCDColorTranslate(GRAPHICS_COLOR_DEEP_SKY_BLUE);
    m_u16GroundColor = (uint16_t) LCDColorTranslate(GRAPHICS_COLOR_BROWN);

    m_u16HorizonLevelY = 63;
    m_u16NextHorizonLevelY = 63;
    m_stUpdateRect = {0,0,0,0};
}

return_e LcdDrawTask::setup(Heap* i_Heap)
{
    return_e rt;

    // Initializes display hardware
    Crystalfontz128x128_Init();
    // Set default screen orientation
    LCDSetOrientation();

    // Initializes graphics context
    Graphics_initContext(&m_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    

    //Initializes tasks state booleans
    m_bIsFirstLcdDraw = true;
    m_bIsFirstIteration = true;

    //Tasks info setup
    Task::SetTaskExecutionCondition(false);
    Task::SetTaskTickInterval(0);

    //Messages memory allocation
    rt = i_Heap->Allocate(HEAP_MEM_SIZE, &m_pHeapMem);

    return (rt == RETURN_NO_SPACE) ? RETURN_FAIL : RETURN_OK;
}

return_e LcdDrawTask::run(void)
{
  // Use m_sContext->foreground
    return_e rt;
    message_t l_stInputMessage;
    bool l_bGotValidMessage = false;

    if (m_bIsFirstIteration) {

        //Receive message in first Draw Iteration

        rt = Task::Incoming.PopMessage(&l_stInputMessage);
        while (rt != RETURN_EMPTY) {
            if (l_stInputMessage.message_type == HORIZON_PARAMS
                        && l_stInputMessage.sender == LCD_ISSUE) {
                m_u16NextHorizonLevelY = (uint16_t) l_stInputMessage.data[0];
                l_bGotValidMessage = true;
            }
            rt = Task::Incoming.PopMessage(&l_stInputMessage);
        }

        if (!l_bGotValidMessage){
            return RETURN_OK;
        }

        if (m_bIsFirstLcdDraw) {
            m_u16NextHorizonLevelY -= m_u16NextHorizonLevelY % DRAW_CHUNK_LINES;
            m_u16NextHorizonLevelY = max(m_u16NextHorizonLevelY, 0);
            m_u16CurrentHorizonIterLevelY = 0;
            InitialDrawIteration(128 % DRAW_CHUNK_LINES);
        }
        else {
            m_u16CurrentHorizonIterLevelY = m_u16HorizonLevelY;
            UpdateDrawIteration((abs(m_u16NextHorizonLevelY-m_u16HorizonLevelY)) % DRAW_CHUNK_LINES);
        }

        m_bIsFirstIteration = false;
    }
    else {
        if (m_bIsFirstLcdDraw) {
            InitialDrawIteration(DRAW_CHUNK_LINES);
        }
        else {
            UpdateDrawIteration(DRAW_CHUNK_LINES);
        }
    }

    bool l_bFinishedIterations = m_bIsFirstLcdDraw ?
                                m_u16CurrentHorizonIterLevelY == 128 :
                                m_u16CurrentHorizonIterLevelY == m_u16NextHorizonLevelY;

    if (l_bFinishedIterations) {
        m_u16HorizonLevelY = m_u16NextHorizonLevelY;
	
	m_bIsFirstLcdDraw = false;

        m_bIsFirstIteration = true; //Prepare for next draw iteration chain
    }
    else {
        //Define Execute LCD_DRAW for Scheduler message (Auto-Trigger)
        m_pHeapMem[0] = (uint32_t) LCD_DRAW;
        message_t l_stTriggerDrawMessage = {LCD_ISSUE,
                                            SCHEDULER,
                                            ADD_TO_EXECUTION,
                                            1,
                                            m_pHeapMem};
        //Send message
        rt = Task::Outgoing.AddMessage(l_stTriggerDrawMessage);
    }

    return RETURN_OK;
}


void LcdDrawTask::InitialDrawIteration(uint16_t i_u16CurrentIterationDeltaY)
{
    uint16_t l_u16CurrentColor;
    int16_t l_u16NextHorizonIterLevelY = m_u16CurrentHorizonIterLevelY + i_u16CurrentIterationDeltaY;

    if (l_u16NextHorizonIterLevelY < m_u16NextHorizonLevelY) {
        l_u16CurrentColor = m_u16SkyColor;

    }
    else {
        l_u16CurrentColor = m_u16GroundColor;
    }
    LCDDrawCompleteHorizontalRect(m_u16CurrentHorizonIterLevelY,l_u16NextHorizonIterLevelY, l_u16CurrentColor);
    m_u16CurrentHorizonIterLevelY = l_u16NextHorizonIterLevelY;
}


void LcdDrawTask::UpdateDrawIteration(uint16_t i_u16CurrentIterationDeltaY)
{
    uint16_t l_u16NextHorizonIterLevelY;
    uint16_t l_u16CurrentColor;

    if ((int16_t)m_u16NextHorizonLevelY-(int16_t)m_u16HorizonLevelY >= 0) {
        l_u16NextHorizonIterLevelY = m_u16CurrentHorizonIterLevelY + i_u16CurrentIterationDeltaY;
	l_u16CurrentColor = m_u16SkyColor;
    }
    else {
        l_u16NextHorizonIterLevelY = m_u16CurrentHorizonIterLevelY - i_u16CurrentIterationDeltaY;
	l_u16CurrentColor = m_u16GroundColor;
    }

    LCDDrawCompleteHorizontalRect(m_u16CurrentHorizonIterLevelY,l_u16NextHorizonIterLevelY, l_u16CurrentColor);
    m_u16CurrentHorizonIterLevelY = l_u16NextHorizonIterLevelY;
}
