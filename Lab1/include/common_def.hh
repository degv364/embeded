/**
Copyright 2017 Daniel Garcia Vaglio degv364@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and
associated documentation files (the "Software"), to deal in the Software without
restriction,
including without limitation the rights to use, copy, modify, merge, publish,
distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom
the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**/

#ifndef COMMON_DEF_H_
#define COMMON_DEF_H_

// FIXME: hack for waiting some time in main loop
#define HACK_WAIT 5000000

// FIXME: Change this value depending on timer frequency
#define TIMEOUT_30 300

// FIXME: Change to a more apropiate value
#define SOUND_TRESHOLD 8300

#define ADC_SAMPLES_PER_SECOND 20

// FIXME: this is a hack for hi/hd compatibility..
#define SAMPLES_PER_SECOND (ADC_SAMPLES_PER_SECOND)

#define MEAN_SECONDS 5
#define MAX_SAMPLES (SAMPLES_PER_SECOND * (MEAN_SECONDS + 1))
#define MEAN_SAMPLES (SAMPLES_PER_SECOND * MEAN_SECONDS)

// Return values
typedef enum return_e
{
    RETURN_OK = 0,   // Execution succesful
    RETURN_FAIL,   // Execution failed
    RETURN_CRITICAL,   // Critical fail
    RETURN_BAD_PARAM   // Execution failed due to invalid parameters
} return_e;


#endif
