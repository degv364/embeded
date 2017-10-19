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

/*************** Class GPIO Definition ***************/

peripherals::GPIO::GPIO(uint8_t i_u8GPIO_Port, uint8_t i_u8GPIO_Pins) :
        m_u8GPIO_Port(i_u8GPIO_Port), m_u8GPIO_Pins(i_u8GPIO_Pins)
{
}

peripherals::GPIO::GPIO(uint8_t i_u8GPIO_Port, uint8_t i_u8GPIO_Pins, uint8_t i_u8Mode) :
        m_u8GPIO_Port(i_u8GPIO_Port), m_u8GPIO_Pins(i_u8GPIO_Pins), m_u8Mode(i_u8Mode)
{
}

/*************** Class OutputGPIO Definition ***************/

peripherals::OutputGPIO::OutputGPIO(uint8_t i_u8GPIO_Port, uint8_t i_u8GPIO_Pins) :
        GPIO(i_u8GPIO_Port, i_u8GPIO_Pins)
{
    MAP_GPIO_setAsOutputPin(m_u8GPIO_Port, m_u8GPIO_Pins);
}

peripherals::OutputGPIO::OutputGPIO(uint8_t i_u8GPIO_Port, uint8_t i_u8GPIO_Pins,
                               uint8_t i_u8Mode) :
        GPIO(i_u8GPIO_Port, i_u8GPIO_Pins, i_u8Mode)
{
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(m_u8GPIO_Port, m_u8GPIO_Pins,
                                                    m_u8Mode);
}

void peripherals::OutputGPIO::Set(void)
{
    MAP_GPIO_setOutputHighOnPin(m_u8GPIO_Port, m_u8GPIO_Pins);
}

void peripherals::OutputGPIO::Reset(void)
{
    MAP_GPIO_setOutputLowOnPin(m_u8GPIO_Port, m_u8GPIO_Pins);
}

void peripherals::OutputGPIO::Toggle(void)
{
    MAP_GPIO_toggleOutputOnPin(m_u8GPIO_Port, m_u8GPIO_Pins);
}

/*************** Class InputGPIO Definition ***************/

peripherals::InputGPIO::InputGPIO(uint8_t i_u8GPIO_Port, uint8_t i_u8GPIO_Pins) :
        GPIO(i_u8GPIO_Port, i_u8GPIO_Pins)
{
    MAP_GPIO_setAsInputPin(m_u8GPIO_Port, m_u8GPIO_Pins);
}

peripherals::InputGPIO::InputGPIO(uint8_t i_u8GPIO_Port, uint8_t i_u8GPIO_Pins,
                             uint8_t i_u8Mode) :
        GPIO(i_u8GPIO_Port, i_u8GPIO_Pins, i_u8Mode)
{
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(m_u8GPIO_Port, m_u8GPIO_Pins,
                                                   m_u8Mode);
}

void peripherals::InputGPIO::ConfigPullDown(void)
{
    MAP_GPIO_setAsInputPinWithPullDownResistor(m_u8GPIO_Port, m_u8GPIO_Pins);
}

void peripherals::InputGPIO::ConfigPullUp(void)
{
    MAP_GPIO_setAsInputPinWithPullUpResistor(m_u8GPIO_Port, m_u8GPIO_Pins);
}

void peripherals::InputGPIO::EnableInterrupt(uint8_t i_u8EdgeSelect)
{
    MAP_GPIO_clearInterruptFlag(m_u8GPIO_Port, m_u8GPIO_Pins);
    MAP_GPIO_enableInterrupt(m_u8GPIO_Port, m_u8GPIO_Pins);
    MAP_GPIO_interruptEdgeSelect(m_u8GPIO_Port, m_u8GPIO_Pins, i_u8EdgeSelect);

    //Calculates interrupt port from corresponding GPIO Port
    uint32_t l_u32INT_Port = (INT_PORT1 - GPIO_PORT_P1) + m_u8GPIO_Port;
    assert(INT_Port >= INT_PORT1 && INT_Port <= INT_PORT6);

    MAP_Interrupt_enableInterrupt(l_u32INT_Port);
}

void peripherals::InputGPIO::DisableInterrupt(void)
{
    MAP_GPIO_disableInterrupt(m_u8GPIO_Port, m_u8GPIO_Pins);
}

bool peripherals::InputGPIO::Read(void)
{
    return MAP_GPIO_getInputPinValue(m_u8GPIO_Port, m_u8GPIO_Pins);
}

bool peripherals::InputGPIO::CheckAndCleanIRQ(uint8_t i_u8GPIO_Port, uint8_t i_u8GPIO_Pin)
{
    uint64_t l_u64Status = MAP_GPIO_getEnabledInterruptStatus(i_u8GPIO_Port);
    MAP_GPIO_clearInterruptFlag(i_u8GPIO_Port, l_u64Status);
    //Returns true if Interrupt flag corresponds to specified GPIO_Pin
    return (l_u64Status & i_u8GPIO_Pin);
}

