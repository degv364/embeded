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

#ifndef COMMON_DEF_H_
#define COMMON_DEF_H_

//Percentage of samples SOUND_THRESHOLD_PERCENT higher than
//mean in last second required for microphone loud condition
#define SOUND_LOUD_SAMPLES_PERCENT 95
//Sound percentage higher than mean to consider some sample loud
#define SOUND_THRESHOLD_PERCENT 10.0
//Sound silence approximate ADC level
#define SOUND_SIGNAL_OFFSET 8175

//Number of seconds to wait in ON condition (default=30 min)
#define TIME_WAIT_SECONDS 15
//Number of Timer32 interrupts (software counts) per second
#define TIME_SAMPLES_PER_SECOND 10
//Number of Timer32 interrupts (software counts) to wait in ON condition
#define TIME_WAIT_COUNT (TIME_SAMPLES_PER_SECOND * TIME_WAIT_SECONDS)

//Microphone number of samples per second
#define ADC_SAMPLES_PER_SECOND 50 //20

//Number of enabled lamps
#define ENABLED_LAMPS 3
//Low light lux level threshold
#define LIGHT_THRESHOLD 5
//Light sensor software reading frequency (defined by main loop counter)
#define LIGHT_SENSOR_MAINLOOP_READ_FREQ 100
//Number of for loop cycles to generate a delay in alive sequence
#define LAMP_ALIVE_SEQ_WAIT_CYCLES 25000

//Number of seconds taken into account for samples mean
#define MEAN_SECONDS 5
//Number of samples in MEAN_SECONDS
#define MEAN_SAMPLES (ADC_SAMPLES_PER_SECOND * MEAN_SECONDS)
//Total number of samples (MEAN_SECONDS + 1 Testing second)
#define MAX_SAMPLES (ADC_SAMPLES_PER_SECOND * (MEAN_SECONDS + 1))

// Return values
typedef enum return_e
{
    RETURN_OK = 0,     // Execution successful
    RETURN_FAIL,       // Execution failed
    RETURN_CRITICAL,   // Critical fail
    RETURN_BAD_PARAM   // Execution failed due to invalid parameters
} return_e;

#endif
