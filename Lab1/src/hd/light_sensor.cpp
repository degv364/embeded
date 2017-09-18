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
    //Initialize I2C communication
    comm::i2c::init();
    //Configure Light Sensor as I2C slave prior to communication
    comm::i2c::setSlave(LIGHT_SENSOR_I2C_ADDR);
    //Sends specified configuration to OPT3001 I2C Interface
    comm::i2c::write16(CONFIG_REG, config_value);
}

uint16_t periph::LightSensor::readRegister(uint8_t regAddr)
{
    //Configure Light Sensor as I2C slave prior to communication
    comm::i2c::setSlave(LIGHT_SENSOR_I2C_ADDR);
    //Gets the content from the specified register (2 bytes)
    return comm::i2c::read16(regAddr);
}

uint64_t periph::LightSensor::read(void)
{
    //Reads the result register raw value
    uint16_t sensorVal = readRegister(RESULT_REG);

    //Lux value exponent corresponds to the 4 MSBs from sensorVal
    uint8_t exponent = (sensorVal >> 12) & 0x0F;
    assert(exponent >= 0 && exponent <= 11);

    //Lux fraction value corresponds to the 12 LSBs from sensorVal
    uint16_t fraction = sensorVal & 0x0FFF;

    uint64_t result;

    /* Decreasing the exponent by 6 allows to approximate the
     * 0.01 factor in the formula LSB_Size = 0.01 * 2^E[3:0].
     * (2^-6 = 0.015625). This allows to approximate the
     * lux reading (lux = LSB_Size * R[11:0]) by only shifting
     * the fraction shiftedExp times.
     */
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

