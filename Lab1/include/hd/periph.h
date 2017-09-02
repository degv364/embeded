/*
 * periph_setup.h
 *
 *  Created on: Aug 27, 2017
 *      Author: ezamoraa
 */

#ifndef INCLUDE_PERIPH_H_
#define INCLUDE_PERIPH_H_

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <assert.h>

namespace periph
{
class MicrophoneADC
{
public:
    MicrophoneADC(uint16_t samplesPerSecond, uint32_t ADC_MEM_Pos = ADC_MEM0);
    void config(uint16_t samplesPerSecond, uint32_t ADC_MEM_Pos = ADC_MEM0);
    void start(void);
    void stop(void);
    uint16_t read(void);
    static bool checkAndCleanIRQ(uint32_t ADC_MEM_Interrupt);

private:
    uint16_t samplesPerSecond_;
    uint32_t ADC_MEM_Pos_;
};

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

#endif /* INCLUDE_PERIPH_H_ */
