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

#include "hd/periph.hh"

namespace periph
{

class GPIO
{
public:
    GPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins);
    GPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins, uint8_t mode);

protected:
    uint8_t GPIO_Port_;
    uint8_t GPIO_Pins_;
    uint8_t mode_;
};

class OutputGPIO: GPIO
{
public:
    OutputGPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins);
    OutputGPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins, uint8_t mode);
    void set(void);
    void reset(void);
    void toggle(void);
};

class InputGPIO: GPIO
{
public:
    InputGPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins);
    InputGPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins, uint8_t mode);
    void configPullUp(void);
    void configPullDown(void);
    void enableInterrupt(uint8_t edgeSelect);
    void disableInterrupt(void);
    bool read(void);
    static bool checkAndCleanIRQ(uint8_t GPIO_Port, uint16_t GPIO_Pin);
};

}

#endif /* INCLUDE_HD_GPIO_HH_ */
