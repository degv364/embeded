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


#ifndef INCLUDE_HI_TASKS_LCD_DRAW_TASK_HH_
#define INCLUDE_HI_TASKS_LCD_DRAW_TASK_HH_

#include <stdlib.h>

#include "hd/periph.hh"
#include "hd/lcd_driver/optimized_driver.hh"

#include "hi/hi_def.hh"
#include "hi/task.hh"


class LcdDrawTask : public Task
{
public:
    LcdDrawTask(void);
    virtual return_e run(void);
    virtual return_e setup(Heap* i_Heap);

private:
    void InitialDrawIteration(uint16_t i_u16CurrentIterationDeltaY);
    void UpdateDrawIteration(uint16_t i_u16CurrentIterationDeltaY);

    uint16_t m_u16HorizonLevelY;     //Current HorizonLevelY
    uint16_t m_u16NextHorizonLevelY; //HorizonLevelY after the draw iteration chain

    uint16_t m_u16CurrentHorizonIterLevelY;

    uint16_t m_u16SkyColor;
    uint16_t m_u16GroundColor;

    bool m_bIsFirstLcdDraw;
    bool m_bIsFirstIteration;

    static constexpr uint8_t DRAW_CHUNK_LINES = 6;
    static constexpr uint8_t HEAP_MEM_SIZE = 1;
    uint32_t* m_pHeapMem;
};



#endif /* INCLUDE_HI_TASKS_LCD_DRAW_TASK_HH_ */