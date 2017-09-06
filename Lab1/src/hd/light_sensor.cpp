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

/*************** Class LightSensor Definition ***************/

periph::LightSensor::LightSensor(uint16_t config_value)
{
    comm::i2c::init();
    comm::i2c::setSlave(LIGHT_SENSOR_I2C_ADDR);
    comm::i2c::write16(CONFIG_REG, config_value);
}

uint16_t periph::LightSensor::readRegister(uint8_t regAddr)
{
    comm::i2c::setSlave(LIGHT_SENSOR_I2C_ADDR);
    return comm::i2c::read16(regAddr);
}

uint64_t periph::LightSensor::read(void)
{
    uint16_t sensorVal = readRegister(RESULT_REG);

    uint8_t exponent = (sensorVal >> 12) & 0x0F;
    assert(exponent >= 0 && exponent <= 11);

    uint16_t fraction = sensorVal & 0x0FFF;

    uint64_t result;

    int shiftedExp = exponent - 6;

    if (shiftedExp < 0)
    {
        result = fraction >> -shiftedExp;
    }
    else
    {
        result = fraction << shiftedExp;
    }

    return result;
}

