/*
 * periph.cpp
 *
 *  Created on: Aug 27, 2017
 *      Author: ezamoraa
 */

#include "../../include/hd/periph.h"


/*************** Class MicrophoneADC Definition ***************/

periph::MicrophoneADC::MicrophoneADC(uint16_t samplesPerSecond, uint32_t ADC_MEM_Pos)
: samplesPerSecond_(samplesPerSecond), ADC_MEM_Pos_(ADC_MEM_Pos)
{
    //MCLK = 3 MHz
    //ACLK = 32 KHz
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    //Init ADC
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1,0);

    //Set P4.3 as A10 ADC Input
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN3,
                                                   GPIO_TERTIARY_MODULE_FUNCTION);
    //Configure ADC Sampling and Storage
    config(samplesPerSecond_,  ADC_MEM_Pos_);
}


void periph::MicrophoneADC::config(uint16_t samplesPerSecond, uint32_t ADC_MEM_Pos)
{
    //Configure ADC Memory Register
    MAP_ADC14_configureSingleSampleMode(ADC_MEM_Pos_, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM_Pos_, ADC_VREFPOS_AVCC_VREFNEG_VSS,
        ADC_INPUT_A10, false);

    //Configure Timer A for ADC Sampling
    uint32_t TA0_Period =  MAP_CS_getACLK()/samplesPerSecond_;
    assert(TA0_Period > 0 && TA0_Period <= 65535);

    const Timer_A_UpModeConfig TA0_UpModeConfig =
    {
         TIMER_A_CLOCKSOURCE_ACLK,
         TIMER_A_CLOCKSOURCE_DIVIDER_1,
         TA0_Period,
         TIMER_A_TAIE_INTERRUPT_DISABLE,
         TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
         TIMER_A_DO_CLEAR
    };

    const Timer_A_CompareModeConfig TA0_CompareModeConfig =
    {
         TIMER_A_CAPTURECOMPARE_REGISTER_1,
         TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
         TIMER_A_OUTPUTMODE_SET_RESET,
         TA0_Period
    };

    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &TA0_UpModeConfig);
    MAP_Timer_A_initCompare(TIMER_A0_BASE, &TA0_CompareModeConfig);

    MAP_ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false);

    //Enable ADC Interrupts
    //MEM Definition value matches with corresponding INT
    uint32_t ADC_Int = ADC_MEM_Pos_;
    MAP_ADC14_enableInterrupt(ADC_Int);
}


void periph::MicrophoneADC::start(void)
{
    MAP_ADC14_enableConversion();
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}


void periph::MicrophoneADC::stop(void)
{
    MAP_ADC14_disableConversion();
    MAP_Interrupt_disableInterrupt(INT_ADC14);
    MAP_Timer_A_stopTimer(TIMER_A0_BASE);
}


uint16_t periph::MicrophoneADC::read(void)
{
    return MAP_ADC14_getResult(ADC_MEM_Pos_);
}


bool periph::MicrophoneADC::checkAndCleanIRQ(uint32_t ADC_MEM_Interrupt)
{
    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    return (status & ADC_MEM_Interrupt);
}



/*************** Class GPIO Definition ***************/

periph::GPIO::GPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins)
: GPIO_Port_(GPIO_Port), GPIO_Pins_(GPIO_Pins)
{}


periph::GPIO::GPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins, uint8_t mode)
: GPIO_Port_(GPIO_Port), GPIO_Pins_(GPIO_Pins), mode_(mode)
{}



/*************** Class OutputGPIO Definition ***************/

periph::OutputGPIO::OutputGPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins)
: GPIO(GPIO_Port, GPIO_Pins)
{
    MAP_GPIO_setAsOutputPin(GPIO_Port_, GPIO_Pins_);
}


periph::OutputGPIO::OutputGPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins, uint8_t mode)
: GPIO(GPIO_Port, GPIO_Pins, mode)
{
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_Port_, GPIO_Pins_, mode_);
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

periph::InputGPIO::InputGPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins)
: GPIO(GPIO_Port, GPIO_Pins)
{
    MAP_GPIO_setAsInputPin(GPIO_Port_, GPIO_Pins_);
}


periph::InputGPIO::InputGPIO(uint8_t GPIO_Port, uint16_t GPIO_Pins, uint8_t mode)
: GPIO(GPIO_Port, GPIO_Pins, mode)
{
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_Port_, GPIO_Pins_, mode_);
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

    uint32_t INT_Port = (INT_PORT1-GPIO_PORT_P1) + GPIO_Port_;
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
    return (status & GPIO_Pin);
}

