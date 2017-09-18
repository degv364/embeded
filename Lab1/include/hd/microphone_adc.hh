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

#ifndef INCLUDE_HD_MICROPHONE_ADC_HH_
#define INCLUDE_HD_MICROPHONE_ADC_HH_

#include "hd/periph.hh"

namespace periph
{

/*
 * Class that manages the BOOSTXL-EDUMKII microphone reading
 * through ADC module
 */
class MicrophoneADC
{
public:
    /* MicrophoneADC constructor allows to define the number of
     * ADC samples per second and the ADC register to store the
     * captured data
     */
    MicrophoneADC(uint16_t samplesPerSecond, uint32_t ADC_MEM_Pos = ADC_MEM0);

    //Recofigure the number of ADC samples per second and the ADC register
    void config(uint16_t samplesPerSecond, uint32_t ADC_MEM_Pos = ADC_MEM0);

    //Start the ADC periodic sampling
    void start(void);

    //Halts the ADC module
    void stop(void);

    //Read the current value stored in the configured ADC memory position register
    uint16_t read(void);

    //Read the current value stored in some specified ADC memory position register
    static uint16_t read(uint32_t ADC_MEM_Pos);

    /* Check if there is a pending interrupt request from some ADC
     * memory position register and clean the interrupt flag
     */
    static bool checkAndCleanIRQ(uint32_t ADC_MEM_Interrupt);

private:
    uint16_t samplesPerSecond_;
    uint32_t ADC_MEM_Pos_;
};

}

#endif /* INCLUDE_HD_MICROPHONE_ADC_HH_ */
