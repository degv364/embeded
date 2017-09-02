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

#include "../../include/hi/hi_utils.hh"

return_e Hi_dual_mean_fifo::reset_to_value(uint16_t value)
{
    //reset count of valid values
    this->current_valid_values = 0;
    for (uint16_t index; index < MAX_SAMPLES; index++)
    {
        this->data[index] = value;
    }
    this->mean = (float) value;
    this->last_mean = (float) value;
    return RETURN_OK;
}

return_e Hi_dual_mean_fifo::move_next_sample()
{
    this->subs_index = (this->subs_index + MAX_SAMPLES - 1) % MAX_SAMPLES;
    this->comp_index = (this->comp_index + MAX_SAMPLES - 1) % MAX_SAMPLES;
    return RETURN_OK;
}

Hi_dual_mean_fifo::Hi_dual_mean_fifo(void)
{
    this->subs_index = 0;
    this->comp_index = SAMPLES_PER_SECOND;
    this->reset_to_value(0);
}
Hi_dual_mean_fifo::~Hi_dual_mean_fifo(void)
{
    // FIXME:add destructor
    reset_to_value(0);
}

return_e Hi_dual_mean_fifo::add_sample(uint16_t sample)
{
    //update 5 second mean
    this->mean += (float) this->data[this->comp_index] / MEAN_SAMPLES;
    this->mean -= (float) this->data[this->subs_index] / MEAN_SAMPLES;

    //update last second mean
    this->last_mean += (float) sample / SAMPLES_PER_SECOND;
    this->last_mean -=
            (float) this->data[this->comp_index] / SAMPLES_PER_SECOND;

    //update data
    this->data[this->subs_index] = sample;
    this->move_next_sample();
    //update valid data count
    this->current_valid_values =
            (this->current_valid_values >= MAX_SAMPLES) ?
                    MAX_SAMPLES : this->current_valid_values + 1;
    return RETURN_OK;
}

return_e Hi_dual_mean_fifo::is_last_second_big(bool *is_big)
{
    float five_percent;

    if (is_big == 0)
    {
        return RETURN_BAD_PARAM;
    }

    five_percent = this->mean * 0.05;
    if (this->mean + five_percent < this->last_mean)
    {
        *is_big = (this->current_valid_values >= MAX_SAMPLES);
    }
    else
    {
        *is_big = false;
    }
    return RETURN_OK;
}

#ifdef TESTING
float
Hi_dual_mean_fifo::get_mean()
{
    return this->mean;
}
float
Hi_dual_mean_fifo::get_last_mean()
{
    return this->last_mean;
}
#endif

