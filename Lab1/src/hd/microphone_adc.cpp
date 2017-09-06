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

/*************** Class MicrophoneADC Definition ***************/

periph::MicrophoneADC::MicrophoneADC(uint16_t samplesPerSecond,
                                     uint32_t ADC_MEM_Pos) :
        samplesPerSecond_(samplesPerSecond), ADC_MEM_Pos_(ADC_MEM_Pos)
{
    //MCLK = 3 MHz
    //ACLK = 32 KHz
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    //Init ADC
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1,
                         0);

    //Set P4.3 as A10 ADC Input
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN3,
    GPIO_TERTIARY_MODULE_FUNCTION);
    //Configure ADC Sampling and Storage
    config(samplesPerSecond_, ADC_MEM_Pos_);
}

void periph::MicrophoneADC::config(uint16_t samplesPerSecond,
                                   uint32_t ADC_MEM_Pos)
{
    //Configure ADC Memory Register
    MAP_ADC14_configureSingleSampleMode(ADC_MEM_Pos_, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM_Pos_,
                                        ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                        ADC_INPUT_A10,
                                        false);

    //Configure Timer A for ADC Sampling
    uint32_t TA0_Period = MAP_CS_getACLK() / samplesPerSecond_;
    assert(TA0_Period > 0 && TA0_Period <= 65535);

    const Timer_A_UpModeConfig TA0_UpModeConfig = {
            TIMER_A_CLOCKSOURCE_ACLK,
            TIMER_A_CLOCKSOURCE_DIVIDER_1,
            TA0_Period,
            TIMER_A_TAIE_INTERRUPT_DISABLE,
            TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
            TIMER_A_DO_CLEAR };

    const Timer_A_CompareModeConfig TA0_CompareModeConfig = {
            TIMER_A_CAPTURECOMPARE_REGISTER_1,
            TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
            TIMER_A_OUTPUTMODE_SET_RESET, TA0_Period };

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

uint16_t periph::MicrophoneADC::read(uint32_t ADC_MEM_Pos)
{
    return MAP_ADC14_getResult(ADC_MEM_Pos);
}

bool periph::MicrophoneADC::checkAndCleanIRQ(uint32_t ADC_MEM_Interrupt)
{
    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    return (status & ADC_MEM_Interrupt);
}

