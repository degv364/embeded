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

#ifndef INCLUDE_HD_TIMER_HH_
#define INCLUDE_HD_TIMER_HH_

#include "peripherals.hh"

namespace peripherals
{
/*
 * Class that manages the Timer32 configuration allowing to set
 * a configurable number of interrupts per second
 */
class Timer
{
public:
    /*
     * Timer constructor initializes the specified Timer32 module
     * and configures the timer to issue the indicated number of
     * interrupts per second
     */
    Timer(uint32_t i_u32Timer32Base, uint16_t i_u16InterruptsPerSecond);

    //Start the timer count
    void Start(void);

    //Halts the timer
    void Stop(void);

    //Get the current number of interrupts per second
    uint16_t GetInterruptsPerSecond(void);

    //Enable the interrupt generation for the specified timer
    void EnableInterrupt(void);

    //Clean the interrupt flag for specified Timer32 module
    static void CleanIRQ(uint32_t timer32_base);
private:
    uint32_t m_u32Timer32Base; //Timer32 module
    uint16_t m_u16InterruptsPerSecond;
};

}

#endif /* INCLUDE_HD_TIMER_HH_ */
