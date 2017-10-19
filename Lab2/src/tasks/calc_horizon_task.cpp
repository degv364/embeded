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

#include "tasks/calc_horizon_task.hh"

CalcHorizonTask::CalcHorizonTask(void)
{
    Task::SetTaskName(CALC_HORIZON);
    Task::SetTaskType(ONE_SHOT);

    m_stLastAccel = {0,0,0};
}

return_e CalcHorizonTask::setup(Heap* i_Heap)
{
    return_e rt;

    Task::SetTaskExecutionCondition(false);
    this->SetTaskTickInterval(0);

    //Messages memory allocation
    rt = i_Heap->Allocate(HEAP_MEM_SIZE, &m_pHeapMem);

    return (rt == RETURN_NO_SPACE) ? RETURN_FAIL : RETURN_OK;
}

return_e CalcHorizonTask::run(void)
{
    return_e rt;
    message_t l_stInputMessage;
    Task::m_bIsFinished = false;
    rt = Task::Incoming.PopMessage(&l_stInputMessage);

    while(rt != RETURN_EMPTY){
        if (l_stInputMessage.message_type == ACCEL_DATA) {
            m_stLastAccel.x = (int16_t) l_stInputMessage.data[0];
            m_stLastAccel.y = (int16_t) l_stInputMessage.data[1];
            m_stLastAccel.z = (int16_t) l_stInputMessage.data[2];
        }
        rt = Task::Incoming.PopMessage(&l_stInputMessage);
    }

    uint16_t l_u16HorizonY = (uint16_t) (63.0*((CalcPitchAngle()/90.0) + 1.0));
    float l_fHorizonSlope =  CalcRollAngleSlope();

    m_pHeapMem[0] = (uint32_t) l_u16HorizonY;
    m_pHeapMem[1] = *reinterpret_cast<uint32_t*>(&l_fHorizonSlope); //Cast pointer to not change the bits


    message_t l_stHorizonMessage = {CALC_HORIZON,
                                    LCD_ISSUE,
                                    HORIZON_PARAMS,
                                    HEAP_MEM_SIZE,
                                    m_pHeapMem};

    rt = Task::Outgoing.AddMessage(l_stHorizonMessage);

    Task::m_bIsFinished = true;
    return (rt == RETURN_NO_SPACE) ? RETURN_FAIL : RETURN_OK;
}

inline float CalcHorizonTask::CalcPitchAngle(void){

    //Flipped axes to achieve correct horizon orientation

    float gz = -m_stLastAccel.y;
    float gx = -m_stLastAccel.x;
    float gy = -m_stLastAccel.z;

    float result = atan2(gy,gz)*(180.0f/M_PI);

    return max(min(result, 90.0f),-90.0f);
}

inline float CalcHorizonTask::CalcRollAngleSlope(void){

    //Flipped axes to achieve correct horizon orientation
    float gz = -m_stLastAccel.y;
    float gx = -m_stLastAccel.x;
    float gy = -m_stLastAccel.z;
    float result = gx/sqrt(gz*gz+gy*gy);
    
    return max(min(result, 128.0f),-128.0f);
}

