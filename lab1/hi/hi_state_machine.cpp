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

// FIXME: convert to singleton


#include "hi_state_machine.hh"

Hi_state_machine::Hi_state_machine(){
  this->state        = HI_STATE_INIT;
  this->stored_state = HI_STATE_NONE;
}

Hi_state_machine::~Hi_state_machine(){
  this->state        = HI_STATE_DEINIT;
  this->stored_state = HI_STATE_NONE;
}

hi_return_e
Hi_state_machine::handle_sensors(hi_sensor_t* input_sensor_data){
  if (input_sensor_data == NULL){
    return HI_RETURN_BAD_PARAM;
  }
  
  // create intenal_copy
  hi_return_e rt = HI_RETURN_OK;
  uint16_t time       = input_sensor_data->time;
  bool control_button = input_sensor_data->control_button;
  bool light_sensor   = input_sensor_data->light_sensor;
  bool microphone     = input_sensor_data->microphone;

  if (control_button){
    this->stored_state = this->state;
    this->state = HI_STATE_MANUAL_CONTROL;
    // reset time
    input_sensor_data->time = 0;
    rt = lamp_toogle();
    goto handle_fail;
  }

  switch(this->state) {
  case HI_STATE_INIT:
    this->state = HI_STATE_ALIVE_SEQ;
    // reset time
    input_sensor_data->time = 0;
    rt = HI_RETURN_OK;
    break;
  case HI_STATE_ALIVE_SEQ:
    //FIXME: implement this
    this->state = HI_STATE_OFF;
    rt = HI_RETURN_OK;
    break;
  case HI_STATE_ON:
    rt = lamp_on();
    if (time>TIMEOUT_30 || light_sensor){
      this->state = HI_STATE_OFF;
      break;
    }
    if (microphone){
      //reset timer
      input_sensor_data->time = 0;
    }
    break;
  case HI_STATE_OFF:
    rt = lamp_off();
    if (light_sensor){
      break;
    }
    if (!microphone){
      break;
    }
    //reset time
    input_sensor_data->time = 0;
    this->state = HI_STATE_ON;
    break;
  case HI_STATE_MANUAL_CONTROL:
    if(time>TIMEOUT_30){
      rt = lamp_toogle();
      this->state = this->stored_state;
    }
    break;
  default:
    //FIXME: implement other states
    break;
  }  

 handle_fail:
  if (rt!=HI_RETURN_OK){
    this->state = HI_STATE_FAIL;
  }
  return rt;
    
}


#ifdef TESTING
static bool is_on = false;

hi_state_e
Hi_state_machine::get_state(void){
  return this->state;
}

hi_return_e
lamp_off(){
  is_on = false;
  return HI_RETURN_OK;
}
hi_return_e
lamp_on(){
  is_on = true;
  return HI_RETURN_OK;
}

hi_return_e
lamp_toogle(){
  is_on = !is_on;
  return HI_RETURN_OK;
}

bool
is_lamp_on(){
  return is_on;
}
bool
is_lamp_off(){
  return !is_on;
}
#endif
