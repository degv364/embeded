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

#include <hd/peripherals.hh>

/*************** Class Timer Definition ***************/

peripherals::Timer::Timer(uint32_t i_u32Timer32Base, uint16_t i_u16InterruptsPerSecond):
    m_u32Timer32Base(i_u32Timer32Base),
    m_u16InterruptsPerSecond(i_u16InterruptsPerSecond)
{
    assert(m_u32Timer32Base == TIMER32_0_BASE || m_u32Timer32Base == TIMER32_1_BASE);
    /* Periodic mode allows to set a max timer count, which is required to
     * control the number of interrupts per second
     */
    MAP_Timer32_initModule(m_u32Timer32Base, TIMER32_PRESCALER_16,
                           TIMER32_32BIT, TIMER32_PERIODIC_MODE);
}

void peripherals::Timer::Start(void)
{
    uint32_t l_u32Timer32MaxCount = MAP_CS_getMCLK()/
            (TIMER32_PRESCALE * m_u16InterruptsPerSecond);
    assert(l_u32Timer32MaxCount >= 1);

    /* Configures the max timer count to generate the required
     * number of interrupts per second
     */
    MAP_Timer32_setCount(m_u32Timer32Base, l_u32Timer32MaxCount);
    //Starts the Timer32 module in continuous mode (false)
    MAP_Timer32_startTimer(m_u32Timer32Base, false);
}

void peripherals::Timer::Stop(void)
{
    MAP_Timer32_haltTimer(m_u32Timer32Base);
}

uint16_t peripherals::Timer::GetInterruptsPerSecond(void)
{
    return m_u16InterruptsPerSecond;
}

void peripherals::Timer::EnableInterrupt(void)
{
  uint8_t l_u8Timer32Interrupt = (m_u32Timer32Base == TIMER32_0_BASE)?
    INT_T32_INT1 :
    (m_u32Timer32Base == TIMER32_1_BASE)?
    INT_T32_INT2 : 0;

    MAP_Interrupt_enableInterrupt(l_u8Timer32Interrupt);
}

void peripherals::Timer::CleanIRQ(uint32_t i_u32Timer32Base)
{
    MAP_Timer32_clearInterruptFlag(i_u32Timer32Base);
}
