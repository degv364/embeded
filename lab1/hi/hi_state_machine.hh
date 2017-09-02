/**
Copyright 2017 Daniel Garcia Vaglio <degv364@gmail.com> Esteban Zamora Alvarado <>

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

#ifdef TESTING
#include <iostream>
// FIXME: check if this can be included in microcontroller
#include <cstdint>
#endif
#include "hi_def.hh"

#ifndef HI_ST
#define HI_ST

//FIXME: Change this value depending on timer frequency
#define TIMEOUT_30 300

#ifdef TESTING
hi_return_e lamp_off();
hi_return_e lamp_on();
hi_return_e lamp_toogle();
bool is_lamp_on();
bool is_lamp_off();
#endif

class Hi_state_machine{
private:
  hi_state_e state;
  hi_state_e stored_state;
  
public:
  Hi_state_machine();
  ~Hi_state_machine();

  hi_return_e
  handle_sensors(hi_sensor_t* input_sensor_data);

  #ifdef TESTING
  hi_state_e get_state(void);
  #endif
  
};

#endif
