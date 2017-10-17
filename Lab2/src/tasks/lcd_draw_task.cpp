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

#include "tasks/lcd_draw_task.hh"

LcdDrawTask::LcdDrawTask(void)
{
    Task::SetTaskName(LCD_DRAW);
    Task::SetTaskType(ONE_SHOT);
    m_u32Colors = 0;
    //Initialize sky as MSB and ground as LSB
    m_u32Colors = (uint32_t) (LCDColorTranslate(GRAPHICS_COLOR_BROWN)<<16);
    m_u32Colors |= (uint32_t) (LCDColorTranslate(GRAPHICS_COLOR_DEEP_SKY_BLUE));
}

return_e LcdDrawTask::setup(Heap* i_Heap)
{
    return_e rt;

    // Initializes display hardware
    LCDInit();
    // Set default screen orientation
    LCDSetOrientation();

    //Initializes task state boolean
    m_bIsFirstIteration = true;

    //Tasks info setup
    Task::SetTaskExecutionCondition(false);
    Task::SetTaskTickInterval(0);

    //Messages memory allocation
    rt = i_Heap->Allocate(HEAP_MEM_SIZE, &m_pHeapMem);

    return (rt == RETURN_NO_SPACE) ? RETURN_FAIL : RETURN_OK;
}


return_e
LcdDrawTask::run(){
    return_e rt;
    message_t l_stInputMessage;

  //Handle messages in first iteration--------------------------------------------
    if (m_bIsFirstIteration)
    {
        // Receive messages
        rt = Task::Incoming.PopMessage(&l_stInputMessage);
        if (rt == RETURN_EMPTY) {
            // No messages
            return RETURN_OK;
        }
        while (rt != RETURN_EMPTY) {
            if (rt != RETURN_OK)
                return rt;

            if (l_stInputMessage.message_type == HORIZON_PARAMS
                    && l_stInputMessage.sender == LCD_ISSUE)
            {
                m_u16Pitch = (uint16_t) l_stInputMessage.data[0];
                m_i16Slope = (int16_t) l_stInputMessage.data[1];
            }
            else if (l_stInputMessage.message_type == RECTANGLES_TO_DRAW
                    && l_stInputMessage.sender == LCD_ISSUE)
            {
                m_u8RectanglesToDraw = (uint8_t) l_stInputMessage.length;
                m_aCoordinates = (uint16_t*) l_stInputMessage.data;
                m_u8CurrentRectangle = 0;
            }
            else
            {
                //invalid message
                return RETURN_FAIL;
            }

            rt = Task::Incoming.PopMessage(&l_stInputMessage);
        }
        m_bIsFirstIteration = false;
    }

  //Draw the rectangles----------------------------------------------------------
  LCDDrawDividedRectangle(m_aCoordinates[m_u8CurrentRectangle<<1],
			  m_aCoordinates[(m_u8CurrentRectangle<<1)+1],
			  m_u16Pitch, m_i16Slope, m_u32Colors);

  //Check if more squares need to be drawn---------------------------------------
  m_u8CurrentRectangle++;

  if (m_u8CurrentRectangle < m_u8RectanglesToDraw) {
        m_pHeapMem[0] = (uint32_t) LCD_DRAW;
        message_t l_stTriggerDrawMessage = { LCD_ISSUE,
                                             SCHEDULER,
                                             ADD_TO_EXECUTION,
                                             1,
                                             m_pHeapMem };
        //Send message
        rt = Task::Outgoing.AddMessage(l_stTriggerDrawMessage);
        if (rt != RETURN_OK) {
            return rt;
        }
  }
  else {
        //this is the last iteration, set first iteration flag for next cycle
        m_bIsFirstIteration = true;
  }

  return RETURN_OK;
}
