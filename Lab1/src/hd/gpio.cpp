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

/*************** Class GPIO Definition ***************/

periph::GPIO::GPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins) :
        GPIO_Port_(GPIO_Port), GPIO_Pins_(GPIO_Pins)
{
}

periph::GPIO::GPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins, uint8_t mode) :
        GPIO_Port_(GPIO_Port), GPIO_Pins_(GPIO_Pins), mode_(mode)
{
}

/*************** Class OutputGPIO Definition ***************/

periph::OutputGPIO::OutputGPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins) :
        GPIO(GPIO_Port, GPIO_Pins)
{
    MAP_GPIO_setAsOutputPin(GPIO_Port_, GPIO_Pins_);
}

periph::OutputGPIO::OutputGPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins,
                               uint8_t mode) :
        GPIO(GPIO_Port, GPIO_Pins, mode)
{
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_Port_, GPIO_Pins_,
                                                    mode_);
}

void periph::OutputGPIO::set(void)
{
    MAP_GPIO_setOutputHighOnPin(GPIO_Port_, GPIO_Pins_);
}

void periph::OutputGPIO::reset(void)
{
    MAP_GPIO_setOutputLowOnPin(GPIO_Port_, GPIO_Pins_);
}

void periph::OutputGPIO::toggle(void)
{
    MAP_GPIO_toggleOutputOnPin(GPIO_Port_, GPIO_Pins_);
}

/*************** Class InputGPIO Definition ***************/

periph::InputGPIO::InputGPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins) :
        GPIO(GPIO_Port, GPIO_Pins)
{
    MAP_GPIO_setAsInputPin(GPIO_Port_, GPIO_Pins_);
}

periph::InputGPIO::InputGPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins,
                             uint8_t mode) :
        GPIO(GPIO_Port, GPIO_Pins, mode)
{
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_Port_, GPIO_Pins_,
                                                   mode_);
}

void periph::InputGPIO::configPullDown(void)
{
    MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_Port_, GPIO_Pins_);
}

void periph::InputGPIO::configPullUp(void)
{
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_Port_, GPIO_Pins_);
}

void periph::InputGPIO::enableInterrupt(uint8_t edgeSelect)
{
    MAP_GPIO_clearInterruptFlag(GPIO_Port_, GPIO_Pins_);
    MAP_GPIO_enableInterrupt(GPIO_Port_, GPIO_Pins_);
    MAP_GPIO_interruptEdgeSelect(GPIO_Port_, GPIO_Pins_, edgeSelect);

    //Calculates interrupt port from corresponding GPIO Port
    uint32_t INT_Port = (INT_PORT1 - GPIO_PORT_P1) + GPIO_Port_;
    assert(INT_Port >= INT_PORT1 && INT_Port <= INT_PORT6);

    MAP_Interrupt_enableInterrupt(INT_Port);
}

void periph::InputGPIO::disableInterrupt(void)
{
    MAP_GPIO_disableInterrupt(GPIO_Port_, GPIO_Pins_);
}

bool periph::InputGPIO::read(void)
{
    return MAP_GPIO_getInputPinValue(GPIO_Port_, GPIO_Pins_);
}

bool periph::InputGPIO::checkAndCleanIRQ(uint8_t GPIO_Port, uint16_t GPIO_Pin)
{
    uint64_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_Port);
    MAP_GPIO_clearInterruptFlag(GPIO_Port, status);
    //Returns true if Interrupt flag corresponds to specified GPIO_Pin
    return (status & GPIO_Pin);
}

