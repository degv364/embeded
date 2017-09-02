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

// FIXME: check if one can include c++ auto killing pointer in this microcontroller, if yes
// avoid using normal pointers at all cost :)

#ifdef TESTING
#include <iostream>
// FIXME: check if this can be included in microcontroller
#include <cstdint>
//#endif
#include "hi_def.hh"

#ifndef HI_UTILS_TEMP
#define HI_UTILS_TEMP

//Templated, to increase generality
template<typename INT, typename FLOAT>
class Hi_dual_mean_fifo{
private:
  // count to know if one have enough values to have valid results
  uint16_t current_valid_values;
  //array to store values
  INT data[MAX_SAMPLES];
  //limit of the fifo 
  uint16_t subs_index;
  // get mean until this member
  uint16_t comp_index;
  
  // 5 seconds mean
  FLOAT mean;
  // last second mean
  FLOAT last_mean;

  /*
   * Reset data to a value
   */
  hi_return_e
  reset_to_value(INT value){
    //reset count of valid values
    this->current_valid_values = 0;
    for (uint16_t index; index< MAX_SAMPLES; index++){
      this->data[index] = value;
    }
    this->mean = (FLOAT)value;
    this->last_mean = (FLOAT)value;
    return HI_RETURN_OK;
  }
  /*
   * MOve pointer to next sample
   */
  hi_return_e
  move_next_sample(){
    this->subs_index=(this->subs_index+MAX_SAMPLES-1)%MAX_SAMPLES;
    this->comp_index=(this->comp_index+MAX_SAMPLES-1)%MAX_SAMPLES;
    return HI_RETURN_OK;
  }

public:
  Hi_dual_mean_fifo(void){
    this->subs_index = 0;
    this->comp_index = SAMPLES_PER_SECOND;
    this->reset_to_value(0);
  }
  ~Hi_dual_mean_fifo(void){
    // FIXME:add destructor
    reset_to_value(0);
  }
  /*
   * Adds a new sample
   */
  hi_return_e
  add_sample(INT sample){
    //update 5 second mean
    this->mean+=(FLOAT)this->data[this->comp_index]/MEAN_SAMPLES;
    this->mean-=(FLOAT)this->data[this->subs_index]/MEAN_SAMPLES;
    
    //update last second mean
    this->last_mean+=(FLOAT)sample/SAMPLES_PER_SECOND;
    this->last_mean-=(FLOAT)this->data[this->comp_index]/SAMPLES_PER_SECOND;
    
    //update data
    this->data[this->subs_index] = sample;
    this->move_next_sample();
    //update valid data count
    this->current_valid_values =
      (this->current_valid_values>=MAX_SAMPLES) ? MAX_SAMPLES : this->current_valid_values+1;
    return HI_RETURN_OK;
  }

  /* 
   * Returns true if the last second mean is more than 5% of the last 5 second mean and,
   * there are enough valid values
   */
  hi_return_e
  is_last_second_big(bool *is_big){
    FLOAT five_percent;

    if (is_big == NULL){
      return HI_RETURN_BAD_PARAM;
    }
    
    five_percent = this->mean * 0.05;
    if (this->mean+five_percent<this->last_mean){
      *is_big = (this->current_valid_values >= MAX_SAMPLES);
    }
    else{
      *is_big = false;
    }
    return HI_RETURN_OK;
  }

#ifdef TESTING
  FLOAT
  get_mean(){
    return this->mean;
  }
  FLOAT
  get_last_mean(){
    return this->last_mean;
  }
#endif
};

#endif //HI_UTILS_TEMP
#endif
