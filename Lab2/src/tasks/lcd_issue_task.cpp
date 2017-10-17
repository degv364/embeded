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

#include "tasks/lcd_issue_task.hh"

LcdIssueTask::LcdIssueTask(void)
{
    Task::SetTaskName(LCD_ISSUE);
    Task::SetTaskType(PERIODICAL);

    m_u16HorizonLevelY = 63;
}

return_e LcdIssueTask::setup(Heap* i_Heap)
{
    return_e rt;

    //Tasks info setup
    Task::SetTaskExecutionCondition(false);
    this->SetTaskTickInterval(TICKS_INTERVAL);

    //Messages memory allocation
    rt = i_Heap->Allocate(HEAP_MEM_SIZE, &m_pHeapMem);

    return (rt == RETURN_NO_SPACE) ? RETURN_FAIL : RETURN_OK;
}

//static inline



return_e LcdIssueTask::run(void)
{
    return_e rt;
    message_t l_stInputMessage;
    bool l_bGotValidMessage = false;

    uint16_t l_u16NextHorizonLevelY;
    int16_t l_i16NextHorizonSlope;

    uint8_t l_u8NumRectanglesToDraw = 0;


    rt = Task::Incoming.PopMessage(&l_stInputMessage);
    while(rt != RETURN_EMPTY){
        if (l_stInputMessage.message_type == HORIZON_PARAMS) {
            l_u16NextHorizonLevelY = (uint16_t) l_stInputMessage.data[0];
            l_i16NextHorizonSlope = (int16_t) l_stInputMessage.data[1];
            l_bGotValidMessage = true;
        }
        rt = Task::Incoming.PopMessage(&l_stInputMessage);
    }

    if (l_bGotValidMessage) {
        //Define Execute LCD_DRAW for Scheduler message
        m_pHeapMem[0] = (uint32_t) LCD_DRAW;
        message_t l_stTriggerDrawMessage = {LCD_ISSUE,
                                            SCHEDULER,
                                            ADD_TO_EXECUTION,
                                            1,
                                            m_pHeapMem};


        //Define Horizon Data to LCD_DRAW message
        m_pHeapMem[1] = (uint32_t) l_u16NextHorizonLevelY;
        m_pHeapMem[2] = (uint32_t) l_i16NextHorizonSlope;

        message_t l_stHorizonParamsMessage = {LCD_ISSUE,
                                              LCD_DRAW,
                                              HORIZON_PARAMS,
                                              2,
                                              &m_pHeapMem[1]};

        //Check every square

        message_t l_stRectanglesToDrawMessage = {LCD_ISSUE,
                                                 LCD_DRAW,
                                                 RECTANGLES_TO_DRAW,
                                                 l_u8NumRectanglesToDraw,
                                                 &m_pHeapMem[3]};

        //Send messages
        rt = Task::Outgoing.AddMessage(l_stTriggerDrawMessage);
        rt = Task::Outgoing.AddMessage(l_stHorizonParamsMessage);
        rt = Task::Outgoing.AddMessage(l_stRectanglesToDrawMessage);


        m_u16HorizonLevelY = l_u16NextHorizonLevelY;
        m_i16HorizonSlope = l_i16NextHorizonSlope;
    }

    return (rt == RETURN_NO_SPACE) ? RETURN_FAIL : RETURN_OK;
}
