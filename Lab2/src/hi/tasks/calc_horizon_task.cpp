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

#include "hi/tasks/calc_horizon_task.hh"

CalcHorizonTask::CalcHorizonTask(void) :
    m_AccelADC(ACCEL_ADC_SAMPLES_PER_SECOND)
{
    Task::SetTaskName(CALC_HORIZON);
    Task::SetTaskType(ONE_SHOT);
    m_stLastAccel = {0,0,0};
}

return_e CalcHorizonTask::setup(void)
{
    m_AccelADC.Setup();
    m_AccelADC.Start();
}


return_e CalcHorizonTask::run(void)
{
    uint16_t l_u16HorizonY = (uint16_t) 63.0*((CalcPitchAngle()/90.0) + 1.0);
    //FIXME: Send horizon level in message to LcdDrawTask

    Task::m_bIsFinished = true;
}


inline float CalcHorizonTask::CalcPitchAngle(void){
    float gy = m_stLastAccel.y;
    float gx2 = m_stLastAccel.x * m_stLastAccel.x;
    float gz2 = m_stLastAccel.z * m_stLastAccel.z;

    float result = atan(gy/sqrt(gx2+gz2))*(180.0f/M_PI);
    return max(min(result, 90.0f),-90.0f);
}
