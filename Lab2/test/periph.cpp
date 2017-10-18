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

#include "periph.hh"

periph::LampHandler::LampHandler(uint16_t consistency){
  (void) consistency;
}

return_e
periph::LampHandler::lamps_on(void){
  this->lamp_state = true;
  return RETURN_OK;
}

return_e
periph::LampHandler::lamps_off(void){
  this->lamp_state = false;
  return RETURN_OK;
}

return_e
periph::LampHandler::lamps_toggle(void){
  this->lamp_state = !this->lamp_state;
  return RETURN_OK;
}

return_e
periph::LampHandler::lamps_alive_sequence(uint32_t wait_parameter){
    return RETURN_OK;
}

bool
periph::LampHandler::is_lamp_on(void){
  return this->lamp_state;
}
bool
periph::LampHandler::is_lamp_off(void){
  return !this->lamp_state;
}

#endif
