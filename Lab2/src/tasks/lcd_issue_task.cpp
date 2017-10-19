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

    m_u16HorizonLevelY = m_u16NextHorizonLevelY = 63;
    m_fHorizonSlope = m_fNextHorizonSlope = 0.0f;
}

return_e LcdIssueTask::Setup(Heap* i_Heap)
{
    return_e rt;

    //Tasks info setup
    Task::SetTaskExecutionCondition(false);
    this->SetTaskTickInterval(TICKS_INTERVAL);

    m_bIsInitialDraw = true;

    //Messages memory allocation
    rt = i_Heap->Allocate(HEAP_MEM_SIZE, &m_pHeapMem);

    return (rt == RETURN_NO_SPACE) ? RETURN_FAIL : RETURN_OK;
}


return_e LcdIssueTask::Run(void)
{
    return_e rt;
    message_t l_stInputMessage;
    bool l_bGotValidMessage = false;


    rt = Task::Incoming.PopMessage(&l_stInputMessage);
    while(rt != RETURN_EMPTY){
        if (l_stInputMessage.m_eMessageType == HORIZON_PARAMS) {
            m_u16NextHorizonLevelY = (uint16_t) l_stInputMessage.m_pData[0];
            m_fNextHorizonSlope = *((float*) &l_stInputMessage.m_pData[1]);
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
        m_pHeapMem[1] = (uint32_t) m_u16NextHorizonLevelY;
        m_pHeapMem[2] = *reinterpret_cast<uint32_t*>(&m_fNextHorizonSlope); //cast pointers to not change data


        message_t l_stHorizonParamsMessage = {LCD_ISSUE,
                                              LCD_DRAW,
                                              HORIZON_PARAMS,
                                              2,
                                              &m_pHeapMem[1]};

//        if (m_bIsInitialDraw) {
            SetToDrawAllRectangles();
//            m_bIsInitialDraw = false;
//        }
//        else {
//            CheckRectanglesToDraw();
//        }

        //FIXME: Remove after testing LcdIssue
//        m_u8NumRectanglesToDraw = 0;
//        uint16_t* l_pRectCoord;
//        l_pRectCoord = (uint16_t*) &m_pHeapMem[3+m_u8NumRectanglesToDraw++];
//        l_pRectCoord[0] = 3 << 5;
//        l_pRectCoord[1] = 2 << 5;


        message_t l_stRectanglesToDrawMessage = {LCD_ISSUE,
                                                 LCD_DRAW,
                                                 RECTANGLES_TO_DRAW,
                                                 m_u8NumRectanglesToDraw,
                                                 &m_pHeapMem[3]};

        //Send messages
        rt = Task::Outgoing.AddMessage(l_stTriggerDrawMessage);
        rt = Task::Outgoing.AddMessage(l_stHorizonParamsMessage);
        rt = Task::Outgoing.AddMessage(l_stRectanglesToDrawMessage);


        m_u16HorizonLevelY = m_u16NextHorizonLevelY;
        m_fHorizonSlope = m_fNextHorizonSlope;
    }

    return (rt == RETURN_NO_SPACE) ? RETURN_FAIL : RETURN_OK;
}


inline void LcdIssueTask::SetToDrawAllRectangles(void)
{
    m_u8NumRectanglesToDraw = 0;
    uint16_t* l_pRectCoord;

    //FIXME: Unroll when sure about number of rectangles
    for (uint16_t row = 0; row < 4; row++) {
        for (uint16_t col = 0; col < 4; col++) {
            l_pRectCoord = (uint16_t*) &m_pHeapMem[3+m_u8NumRectanglesToDraw];
            m_u8NumRectanglesToDraw++;
            l_pRectCoord[0] = col << 5; //Rectangles are 32x32 (32 = 2^5)
            l_pRectCoord[1] = row << 5;
        }
    }
}


inline void LcdIssueTask::CheckRectanglesToDraw(void)
{
    m_u8NumRectanglesToDraw = 0;
    uint16_t* l_pRectCoord;

    int16_t l_u16CurrentLineB = (int16_t) ((float) m_u16HorizonLevelY - m_fHorizonSlope * 63.0f);
    int16_t l_u16NextLineB = (int16_t) ((float) m_u16NextHorizonLevelY - m_fNextHorizonSlope * 63.0f);


    //FIXME: Unroll when sure about number of rectangles
    for (uint16_t row = 0; row < 4; row++) {
        for (uint16_t col = 0; col < 4; col++) {
            if (NeedToDrawRectangle(row, col, l_u16CurrentLineB, l_u16NextLineB)) {
                l_pRectCoord = (uint16_t*) &m_pHeapMem[3+m_u8NumRectanglesToDraw++];
                l_pRectCoord[0] = col << 5; //Rectangles are 32x32 (32 = 2^5)
                l_pRectCoord[1] = row << 5;
            }
        }
    }
}

static inline int16_t CalculateLineY(uint16_t i_u16x, float i_fm, int16_t i_u16b)
{
  return (int16_t)(i_fm*(float)i_u16x+(float)i_u16b);
}

static inline int16_t CalculateLineX(uint16_t i_u16y, float i_fm, int16_t i_u16b)
{
  if (i_fm == 0){
    // Avoid exception
    return INT16_MAX;
  }
  return (int16_t) ((float)(i_u16y-i_u16b)/i_fm); 
}

static inline bool ValueInRange(int16_t i_u16Value, int16_t i_u16LimA, int16_t i_u16LimB)
{
    if ((i_u16Value >= i_u16LimA) && (i_u16Value <= i_u16LimB)) return true;
    if ((i_u16Value <= i_u16LimA) && (i_u16Value >= i_u16LimB)) return true;
    return false;
}

inline bool LcdIssueTask::NeedToDrawRectangle(uint16_t row, uint16_t col,
                                              int16_t i_u16CurrentLineB, int16_t i_u16NextLineB)
{
    //Current rectangle corners coordinates (L->lower numerical value)
    int16_t l_u16X_L = row << 5;
    int16_t l_u16X_H = (row+1) << 5;
    int16_t l_u16Y_L = col << 5;
    int16_t l_u16Y_H = (col+1) << 5;


    int16_t l_i16CurrentLineY_X_L = CalculateLineY(l_u16X_L, m_fHorizonSlope, i_u16CurrentLineB);
    int16_t l_i16CurrentLineY_X_H = CalculateLineY(l_u16X_H, m_fHorizonSlope, i_u16CurrentLineB);
    int16_t l_i16NextLineY_X_L    = CalculateLineY(l_u16X_L, m_fNextHorizonSlope, i_u16NextLineB);
    int16_t l_i16NextLineY_X_H    = CalculateLineY(l_u16X_H, m_fNextHorizonSlope, i_u16NextLineB);


    //--------Rectangle between current and next line conditions--------

    //Check if some rectangle corner Y value is between current and next line Y value
    if (ValueInRange(l_u16Y_L, l_i16CurrentLineY_X_L, l_i16NextLineY_X_L)) return true;
    if (ValueInRange(l_u16Y_H, l_i16CurrentLineY_X_L, l_i16NextLineY_X_L)) return true;
    if (ValueInRange(l_u16Y_L, l_i16CurrentLineY_X_H, l_i16NextLineY_X_H)) return true;
    if (ValueInRange(l_u16Y_H, l_i16CurrentLineY_X_H, l_i16NextLineY_X_H)) return true;


    //--------Current or next line touching rectangle conditions--------

    //Current line touches some rectangle border
    if (ValueInRange(l_i16CurrentLineY_X_L, l_u16Y_L, l_u16Y_H)) return true;
    if (ValueInRange(l_i16CurrentLineY_X_H, l_u16Y_L, l_u16Y_H)) return true;
    if (ValueInRange(CalculateLineX(l_u16Y_L, m_u16HorizonLevelY, i_u16CurrentLineB),
                     l_u16X_L, l_u16X_H)) return true;


    //Next line touches some rectangle border
    if (ValueInRange(l_i16NextLineY_X_L, l_u16Y_L, l_u16Y_H)) return true;
    if (ValueInRange(l_i16NextLineY_X_H, l_u16Y_L, l_u16Y_H)) return true;
    if (ValueInRange(CalculateLineX(l_u16Y_L, m_u16NextHorizonLevelY, i_u16NextLineB),
                     l_u16X_L, l_u16X_H)) return true;

    //No condition was true, no need to redraw rectangle
    return false;
}


