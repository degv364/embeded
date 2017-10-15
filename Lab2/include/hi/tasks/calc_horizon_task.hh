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

#ifndef INCLUDE_HI_TASKS_CALC_HORIZON_TASK_HH_
#define INCLUDE_HI_TASKS_CALC_HORIZON_TASK_HH_

#include <math.h>
#include "hd/periph.hh"
#include "hi/hi_def.hh"
#include "hi/task.hh"

class CalcHorizonTask : public Task
{
public:
    CalcHorizonTask(void);
    virtual return_e run(void);
    virtual return_e setup(Heap* i_Heap);
    float CalcPitchAngle(void);
private:
    struct AccelAxes {
        int16_t x;
        int16_t y;
        int16_t z;
    };

    static constexpr uint8_t HEAP_MEM_SIZE = 1;

    periph::AccelADC m_AccelADC;
    AccelAxes m_stLastAccel;
    uint32_t* m_pHeapMem;
};



#endif /* INCLUDE_HI_TASKS_CALC_HORIZON_TASK_HH_ */
