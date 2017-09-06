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

#include "hi/hi_utils.hh"

Hi_dual_mean_fifo::Hi_dual_mean_fifo(void) :
        subs_index_(0), comp_index_(SAMPLES_PER_SECOND)
{
    reset_to_value(0);
}

Hi_dual_mean_fifo::~Hi_dual_mean_fifo(void)
{
    // FIXME:add destructor
    reset_to_value(0);
}

return_e Hi_dual_mean_fifo::reset_to_value(uint16_t value)
{
    //reset count of valid values
    current_valid_values_ = 0;
    for (uint16_t index; index < MAX_SAMPLES; index++)
    {
        data_[index] = value;
    }
    mean_ = (float) value;
    last_mean_ = (float) value;
    return RETURN_OK;
}

return_e Hi_dual_mean_fifo::move_next_sample()
{
    subs_index_ = (subs_index_ + MAX_SAMPLES - 1) % MAX_SAMPLES;
    comp_index_ = (comp_index_ + MAX_SAMPLES - 1) % MAX_SAMPLES;
    return RETURN_OK;
}

return_e Hi_dual_mean_fifo::add_sample(uint16_t sample)
{
    //update 5 second mean
    mean_ += (float) data_[comp_index_] / MEAN_SAMPLES;
    mean_ -= (float) data_[subs_index_] / MEAN_SAMPLES;

    //update last second mean
    last_mean_ += (float) sample / SAMPLES_PER_SECOND;
    last_mean_ -= (float) data_[comp_index_] / SAMPLES_PER_SECOND;

    //update data
    data_[subs_index_] = sample;
    move_next_sample();
    //update valid data count
    current_valid_values_ =
            (current_valid_values_ >= MAX_SAMPLES) ?
            MAX_SAMPLES : current_valid_values_ + 1;
    return RETURN_OK;
}

return_e Hi_dual_mean_fifo::is_last_second_loud(bool *is_loud)
{
    float five_percent;

    if (is_loud == 0)
    {
        return RETURN_BAD_PARAM;
    }

    // Too loud
    if (last_mean_ > SOUND_THRESHOLD)
    {
        *is_loud = true;
    }

    // Fast increase of volume
    five_percent = mean_ * 0.05;
    if (mean_ + five_percent < last_mean_)
    {
        *is_loud = (current_valid_values_ >= MAX_SAMPLES);
    }
    else
    {
        *is_loud = false;
    }
    return RETURN_OK;
}

#ifdef TESTING
float
Hi_dual_mean_fifo::get_mean()
{
    return mean_;
}
float
Hi_dual_mean_fifo::get_last_mean()
{
    return last_mean_;
}
#endif

