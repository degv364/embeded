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

#ifndef INCLUDE_HD_GPIO_HH_
#define INCLUDE_HD_GPIO_HH_

#include "peripherals.hh"

namespace peripherals
{

/*
 *  Class that manages GPIO pins base configuration
 */

class GPIO
{
public:
    GPIO(uint8_t i_u8GPIO_Port, uint8_t i_u8GPIO_Pins);
    GPIO(uint8_t i_u8GPIO_Port, uint8_t i_u8GPIO_Pins, uint8_t i_u8Mode);

protected:
    uint8_t m_u8GPIO_Port;
    uint8_t m_u8GPIO_Pins;
    uint8_t m_u8Mode; //Primary, secondary or tertiary mode
};

/*
 *  Class that manages Output GPIO functionality.
 *  Derived from GPIO Class.
 */

class OutputGPIO: GPIO
{
public:
    OutputGPIO(uint8_t i_u8GPIO_Port, uint8_t i_u8GPIO_Pins);
    OutputGPIO(uint8_t i_u8GPIO_Port, uint8_t i_u8GPIO_Pins, uint8_t i_u8Mode);

    //Set GPIO Output value to 1
    void Set(void);

    //Set GPIO Output value to 0
    void Reset(void);

    //Toggle GPIO Output value
    void Toggle(void);
};

/*
 * Class that manages Input GPIO functionality including interrupt
 * configuration. Derived from GPIO Class.
 */

class InputGPIO: GPIO
{
public:
    InputGPIO(uint8_t i_u8GPIO_Port, uint8_t i_u8GPIO_Pins);
    InputGPIO(uint8_t i_u8GPIO_Port, uint8_t i_u8GPIO_Pins, uint8_t i_u8Mode);

    //Configure GPIO pins with a Pull Up resistor
    void ConfigPullUp(void);

    //Configure GPIO pins with a Pull Down resistor
    void ConfigPullDown(void);

    //Activate Input GPIO interrupt and set signal edge for trigger
    void EnableInterrupt(uint8_t i_u8EdgeSelect);

    //Deactivate Input GPIO
    void DisableInterrupt(void);

    //Read GPIO Input value as a boolean
    bool Read(void);

    /*Check if there is a pending interrupt request from some
     *GPIO Port/Pins and clean the interrupt flag
     */
    static bool CheckAndCleanIRQ(uint8_t i_u8GPIO_Port, uint8_t i_u8GPIO_Pin);
};

}

#endif /* INCLUDE_HD_GPIO_HH_ */
