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

#ifndef TESTING
#include <stdint.h>
#endif

#ifndef HI_DEF_H_
#define HI_DEF_H_

// States
typedef enum hi_state_e
{
    HI_STATE_NONE = 0,       // No state
    HI_STATE_INIT,       // Initialize peripherals
    HI_STATE_ALIVE_SEQ,       // Functioning confirmation
    HI_STATE_ON,       // Lamp ON
    HI_STATE_OFF,       // Lamp OFF
    HI_STATE_DEINIT,       // Deinitialize
    HI_STATE_FAIL,       // Failure ocured, handle it
    HI_STATE_MANUAL_CONTROL,       // Manual button state
    HI_NOSTATE_LAST       // For range validation
} hi_state_e;

typedef struct hi_sensor_t
{
    uint16_t time;
    bool control_button;
    bool light_sensor;
    bool microphone;
} hi_sensor_t;

#endif
