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

#include "hd/periph.hh"

/*************** Class Timer Definition ***************/

periph::Timer::Timer(uint32_t timer32_base, uint16_t interrupts_per_second):
    timer32_base_(timer32_base),
    interrupts_per_second_(interrupts_per_second)
{
    assert(timer32_base_ == TIMER32_0_BASE || timer32_base_ == TIMER32_1_BASE);
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_16,
                           TIMER32_32BIT, TIMER32_PERIODIC_MODE);
}

void periph::Timer::start(void)
{
    uint32_t T32_MaxCount = MAP_CS_getMCLK()/
            (TIMER32_PRESCALE * interrupts_per_second_);
    assert(T32_MaxCount >= 1);

    MAP_Timer32_setCount(timer32_base_, T32_MaxCount);
    MAP_Timer32_startTimer(timer32_base_, false);
}

void periph::Timer::stop(void)
{
    MAP_Timer32_haltTimer(timer32_base_);
}

uint16_t periph::Timer::getInterruptsPerSecond(void)
{
    return interrupts_per_second_;
}

void periph::Timer::enableInterrupt(void)
{
    uint8_t timer32_interrupt = (timer32_base_ == TIMER32_0_BASE)?
                                    INT_T32_INT1 :
                                (timer32_base_ == TIMER32_1_BASE)?
                                    INT_T32_INT2 : 0;

    MAP_Interrupt_enableInterrupt(timer32_interrupt);
}
