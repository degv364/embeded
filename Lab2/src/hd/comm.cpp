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

#include "hd/comm.hh"

void communication::i2c::Init(void)
{
    const eUSCI_I2C_MasterConfig i2cConfig = {
    EUSCI_B_I2C_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
            MCLOCK_FREQ,                        // SMCLK = 3MHz
            EUSCI_B_I2C_SET_DATA_RATE_400KBPS,  // Desired I2C Clock of 400khz
            0,                                  // No byte counter threshold
            EUSCI_B_I2C_NO_AUTO_STOP            // No Autostop
            };

    //Setup GPIOs: I2C_SCL(P6.5), I2C_SDA(P6.4)
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P6,
            GPIO_PIN4 | GPIO_PIN5,
            GPIO_PRIMARY_MODULE_FUNCTION);

    MAP_I2C_initMaster(EUSCI_B1_BASE, &i2cConfig);

    MAP_I2C_enableModule(EUSCI_B1_BASE);
}

void communication::i2c::SetSlave(uint16_t i_u16SlaveAddr)
{
    //Config I2C Slave Address
    MAP_I2C_setSlaveAddress(EUSCI_B1_BASE, i_u16SlaveAddr);

    //Enable and clear TX/RX Interrupt Flags
    MAP_I2C_clearInterruptFlag(EUSCI_B1_BASE,
    EUSCI_B_I2C_TRANSMIT_INTERRUPT0 | EUSCI_B_I2C_RECEIVE_INTERRUPT0);
}

void communication::i2c::Write8(uint8_t i_u8RegAddr, uint8_t i_u8WriteValue)
{
    __disable_irq();

    //Set Master to TX mode
    MAP_I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_MODE);

    //Clear any existing TX Interrupt Flag
    MAP_I2C_clearInterruptFlag(EUSCI_B1_BASE,
    EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    //Wait until I2C Bus is Free
    while (I2C_isBusBusy (EUSCI_B1_BASE));

    //------ Send Register Address ------

    MAP_I2C_masterSendMultiByteStart(EUSCI_B1_BASE, i_u8RegAddr);

    //------ Send Data ------

    MAP_I2C_masterSendMultiByteFinish(EUSCI_B1_BASE, i_u8WriteValue);

    __enable_irq();
}

void communication::i2c::Write16(uint8_t i_u8RegAddr, uint16_t i_u16WriteValue)
{
    __disable_irq();

    //Set Master to TX mode
    MAP_I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_MODE);

    //Clear any existing TX Interrupt Flag
    MAP_I2C_clearInterruptFlag(EUSCI_B1_BASE,
    EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    //Wait until I2C Bus is Free
    while (MAP_I2C_isBusBusy (EUSCI_B1_BASE));

    uint8_t firstByte = i_u16WriteValue >> 8;
    uint8_t secondByte = i_u16WriteValue & 0xFF;

    //------ Send Register Address ------

    MAP_I2C_masterSendMultiByteStart(EUSCI_B1_BASE, i_u8RegAddr);

    //------ Send Data ------

    //Send first byte
    MAP_I2C_masterSendMultiByteNext(EUSCI_B1_BASE, firstByte);

    //Send second byte
    MAP_I2C_masterSendMultiByteFinish(EUSCI_B1_BASE, secondByte);

    __enable_irq();
}

uint8_t communication::i2c::Read8(uint8_t i_u8RegAddr)
{
    __disable_irq();

    uint8_t rxData;

    //Set Master to RX mode
    MAP_I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_MODE);

    //Clear any existing RX Interrupt Flag
    MAP_I2C_clearInterruptFlag(EUSCI_B1_BASE,
    EUSCI_B_I2C_RECEIVE_INTERRUPT0);

    //Wait until I2C Bus is Free
    while (I2C_isBusBusy (EUSCI_B1_BASE));

    //------ Send Register Address ------

    MAP_I2C_masterSendSingleByte(EUSCI_B1_BASE, i_u8RegAddr);

    // Wait for Stop to be sent
    while (MAP_I2C_masterIsStopSent(EUSCI_B1_BASE) == EUSCI_B_I2C_SENDING_STOP);

    //------ Receive Data ------

    rxData = I2C_masterReceiveSingleByte(EUSCI_B1_BASE);

    __enable_irq();

    return rxData;
}

uint16_t communication::i2c::Read16(uint16_t i_u16RegAddr)
{
    __disable_irq();

    uint8_t rxData[2];

    //Set Master to RX mode
    MAP_I2C_setMode(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_MODE);

    //Clear any existing RX Interrupt Flag
    MAP_I2C_clearInterruptFlag(EUSCI_B1_BASE,
    EUSCI_B_I2C_RECEIVE_INTERRUPT0);

    //Wait until I2C Bus is Free
    while (MAP_I2C_isBusBusy(EUSCI_B1_BASE));

    //------ Send Register Address ------

    MAP_I2C_masterSendSingleByte(EUSCI_B1_BASE, i_u16RegAddr);

    // Wait for Stop to be sent
    while (MAP_I2C_masterIsStopSent(EUSCI_B1_BASE) == EUSCI_B_I2C_SENDING_STOP);

    //------ Receive Data ------

    MAP_I2C_masterReceiveStart(EUSCI_B1_BASE);

    // Read first byte
    rxData[0] = MAP_I2C_masterReceiveMultiByteNext(EUSCI_B1_BASE);

    // Read second byte
    rxData[1] = MAP_I2C_masterReceiveMultiByteFinish(EUSCI_B1_BASE);

    __enable_irq();

    return (rxData[0] << 8) | rxData[1];
}

