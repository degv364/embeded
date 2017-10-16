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

//Scheduler tick count frequency
#define TIME_TICKS_PER_SECOND 135

//ADC number of samples per second
#define ACCEL_ADC_SAMPLES_PER_SECOND 100

#define NUMBER_OF_SLOTS 255

#define MAX_TASKS_PER_FRAME 63

#define MAX_SCHEDULER_INTERNAL_MESSAGES 5


// Size of filter buffers
#define MEAN_FILTER_BUFFER_SIZE 4
// Recalibration period
#define MEAN_FILTER_CALIBRATION_PERIOD 65535

//Return values
typedef enum return_e
{
    RETURN_OK = 0,       // Execution successful
    RETURN_FAIL,         // Execution failed
    RETURN_CRITICAL,     // Critical fail
    RETURN_TIMEOUT,      // Timeout
    RETURN_BAD_PARAM,    // Execution failed due to invalid parameters
    RETURN_EMPTY,        // Structurte is empty
    RETURN_INVALID_VALUE,//Function cant return a valid value yet
    RETURN_NO_SPACE      //Not enough space in structure
} return_e;

#endif
