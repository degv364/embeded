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

#ifndef INCLUDE_HD_LIGHT_SENSOR_HH_
#define INCLUDE_HD_LIGHT_SENSOR_HH_

#include "hd/periph.hh"

namespace periph
{

class LightSensor
{
public:
    LightSensor(uint16_t config_value = CONFIG_DEFAULT_800MS);
    uint64_t read(void);
    uint16_t readRegister(uint8_t regAddr);

    static constexpr uint8_t LIGHT_SENSOR_I2C_ADDR = 0x44;
    static constexpr uint8_t RESULT_REG = 0x00;
    static constexpr uint8_t CONFIG_REG = 0x01;
    static constexpr uint16_t CONFIG_DEFAULT_800MS = 0xCC10; // 800ms
    static constexpr uint16_t CONFIG_DEFAULT_100MS = 0xC410; // 100ms
};

}

#endif /* INCLUDE_HD_LIGHT_SENSOR_HH_ */