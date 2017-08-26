/**
Copyright 2017 Daniel Garcia Vaglio degv364@gmail.com

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

//We must know the size during compilation time, to avoid using dynamic allocation
#define SAMPLES_PER_SECOND 5
#define MEAN_SECONDS 5
#define MAX_SAMPLES (SAMPLES_PER_SECOND*(MEAN_SECONDS+1))
#define MEAN_SAMPLES (SAMPLES_PER_SECOND * MEAN_SECONDS)

#ifndef HI_DEF
#define HI_DEF

// Return values
typedef enum hi_return_e{
  HI_RETURN_OK = 0,    // Execution succesful
  HI_RETURN_FAIL,      // Execution failed
  HI_RETURN_CRITICAL,  // Critical fail
  HI_RETURN_BAD_PARAM  // Execution failed due to invalid parameters
} hi_return_e;

// States
typedef enum hi_state_e{
  HI_STATE_NONE = 0,       // No state
  HI_STATE_INIT,           // Initialize peripherals
  HI_STATE_ALIVE_SEQ,      // Functioning confirmation
  HI_STATE_ON,             // Lamp ON
  HI_STATE_OFF,            // Lamp OFF
  HI_STATE_DEINIT,         // Deinitialize
  HI_STATE_FAIL,           // Failure ocured, handle it
  HI_STATE_MANUAL_CONTROL, // Manual button state
  HI_NOSTATE_LAST          // For range validation
} hi_state_e;


// External events, triggers for state transition
typedef enum hi_ext_event_e{
  HI_EXT_EVENT_NONE = 0,    //NO event
  HI_EXT_EVENT_BOOT,        //Boot
  HI_EXT_EVENT_LIGHTS_ON,   //External lights on
  HI_EXT_EVENT_LIGHTS_OFF,  //External lights off
  HI_EXT_EVENT_SOUND_LOW,   // External sound is below treshhold
  HI_EXT_EVENT_SOUND_HIGH,  //External sound increased
  HI_EXT_EVENT_CONTROL_REQ, // User pusehd button, manual control request
  HI_NO_EXT_EVENT_LAST      // For range validation
} hi_ext_event_e;

#endif
