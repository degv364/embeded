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
#ifndef TESTING
#include <stdint.h>
#endif
#include "hi_def.hh"

#ifndef HI_UTILS_TEMP
#define HI_UTILS_TEMP

class Hi_dual_mean_fifo
{
private:
    // count to know if one have enough values to have valid results
    uint16_t current_valid_values;
    //array to store values
    uint16_t data[MAX_SAMPLES];
    //limit of the fifo
    uint16_t subs_index;
    // get mean until this member
    uint16_t comp_index;

    // 5 seconds mean
    float mean;
    // last second mean
    float last_mean;

    /*
     * Reset data to a value
     */
    hi_return_e reset_to_value(uint16_t value);
    /*
     * MOve pointer to next sample
     */
    hi_return_e move_next_sample();

public:
    Hi_dual_mean_fifo(void);
    ~Hi_dual_mean_fifo(void);
    /*
     * Adds a new sample
     */
    hi_return_e add_sample(uint16_t sample);

    /*
     * Returns true if the last second mean is more than 5% of the last 5 second mean and,
     * there are enough valid values
     */
    hi_return_e is_last_second_big(bool *is_big);

#ifdef TESTING
    float get_mean();
    float get_last_mean();
#endif
};

#endif //HI_UTILS_TEMP

