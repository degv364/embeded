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

#ifndef INCLUDE_COMM_H_
#define INCLUDE_COMM_H_

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Internal Includes */
#include "common_def.hh"
#include "hd/hd_def.hh"

namespace communication
{
namespace i2c
{
/* Initialize I2C module in master mode at EUSCI_B1
 * and pins P6.5(I2C_SCL), P6.4(I2C_SDA)
 */
void Init(void);

//Set slave address prior to I2C communication
void SetSlave(uint16_t i_u16SlaveAddr);

/* Write 1 byte (writeValue) from master (MSP432) to
 * some slave register indicated by regAddr
 */
void Write8(uint8_t i_u8RegAddr, uint8_t i_u8WriteValue);

/* Write 2 bytes (writeValue) from master (MSP432) to
 * some slave register indicated by regAddr
 */
void Write16(uint8_t i_u8RegAddr, uint16_t i_u16WriteValue);

/* Read 1 byte from some slave register indicated
 * by regAddr to master (MSP432)
 */
uint8_t Read8(uint8_t i_u8RegAddr);

/* Read 2 bytes from some slave register indicated
 * by regAddr to master (MSP432)
 */
uint16_t Read16(uint16_t i_u16RegAddr);
}
}

#endif /* INCLUDE_COMM_H_ */
