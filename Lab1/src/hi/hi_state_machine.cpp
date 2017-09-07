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

// FIXME: convert to singleton
#include "hi/hi_state_machine.hh"

// FIXME: change this once lamp_handler is a singleton
extern periph::LampHandler lamp_handler;

Hi_state_machine::Hi_state_machine() :
        state_(HI_STATE_INIT)
{
}

Hi_state_machine::~Hi_state_machine()
{
    state_ = HI_STATE_DEINIT;
}

return_e Hi_state_machine::handle_sensors(hi_sensor_t* input_sensor_data)
{
    if (input_sensor_data == 0)
    {
        return RETURN_BAD_PARAM;
    }

    // create internal_copy
    return_e rt = RETURN_OK;
    uint16_t time = input_sensor_data->time;
    bool control_button = input_sensor_data->control_button;
    bool light_sensor = input_sensor_data->light_sensor;
    bool microphone = input_sensor_data->microphone;

    if (control_button)
    {
        state_ = HI_STATE_MANUAL_CONTROL;
        // reset time
        input_sensor_data->time = 0;
        rt = lamp_handler.lamps_toggle();

        // reset control_button flag
        input_sensor_data->control_button = false;

        goto handle_fail;
    }

    switch (state_)
    {
    case HI_STATE_INIT:
        state_ = HI_STATE_ALIVE_SEQ;
        // reset time
        input_sensor_data->time = 0;
        rt = RETURN_OK;
        break;
    case HI_STATE_ALIVE_SEQ:
        //FIXME: implement this
        state_ = HI_STATE_OFF;
        rt = RETURN_OK;
        break;
    case HI_STATE_ON:
        rt = lamp_handler.lamps_on();
        if (time > TIMEOUT_30 || light_sensor)
        {
            state_ = HI_STATE_OFF;
            break;
        }
        if (microphone)
        {
            //reset timer
            input_sensor_data->time = 0;
        }
        break;
    case HI_STATE_OFF:
        rt = lamp_handler.lamps_off();
        if (light_sensor)
        {
            break;
        }
        if (!microphone)
        {
            break;
        }
        //reset time
        input_sensor_data->time = 0;
        state_ = HI_STATE_ON;
        break;
    case HI_STATE_MANUAL_CONTROL:
        if (time > TIMEOUT_30)
        {
            rt = lamp_handler.lamps_toggle();
            state_ = HI_STATE_OFF;
        }
        break;
    default:
        //FIXME: implement other states
        break;
    }

    handle_fail: if (rt != RETURN_OK)
    {
        state_ = HI_STATE_FAIL;
    }
    return rt;

}

#ifdef TESTING
hi_state_e
Hi_state_machine::get_state(void)
{
    return state_;
}
#endif

