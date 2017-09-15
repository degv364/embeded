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
#include <cstdint>
#include "common_def.hh"
#include "hi/hi_def.hh"
#include "hi/hi_utils.hh"
#include "hi/hi_state_machine.hh"
#include "gtest/gtest.h"
#include <iostream>

// FIXME: use google mock for this

periph::LampHandler lamp_handler(1);

namespace
{

    TEST(Hi_Dual_Mean_Fifo, Invalid_During_Init)
    {
        Hi_dual_mean_fifo *dual_fifo = new Hi_dual_mean_fifo();
        bool comp_condition;
        (void) dual_fifo->is_last_second_loud(&comp_condition);
        EXPECT_FALSE(comp_condition);
    }

    TEST(Hi_Dual_Mean_Fifo, Add__Values)
    {
        Hi_dual_mean_fifo *dual_fifo = new Hi_dual_mean_fifo();
        bool comp_condition;
        for (int i=0; i<MAX_SAMPLES*2; i++)
        {
            dual_fifo->add_sample(SOUND_SIGNAL_OFFSET+4);
        }

        EXPECT_NEAR(4, dual_fifo->get_mean(), 0.01);

        (void) dual_fifo->is_last_second_loud(&comp_condition);
        EXPECT_FALSE(comp_condition);

        for (int i=0; i<=ADC_SAMPLES_PER_SECOND; i++)
        {
            dual_fifo->add_sample(SOUND_SIGNAL_OFFSET+20);
        }

        (void) dual_fifo->is_last_second_loud(&comp_condition);
        EXPECT_TRUE(comp_condition);

    }
    TEST(State_Machine, State_Transition)
    {
        hi_sensor_t sensor =
        {   0, false, false, false};
        Hi_state_machine *st = new Hi_state_machine();
        // Simulate initialization sequence
        EXPECT_EQ(HI_STATE_INIT, st->get_state());
        st->handle_sensors(&sensor);
        EXPECT_EQ(HI_STATE_ALIVE_SEQ, st->get_state());
        //FIXME this may change when alive sequence is implemented
        st->handle_sensors(&sensor);
        EXPECT_EQ(HI_STATE_OFF, st->get_state());

        // Init done, Push button
        EXPECT_FALSE(lamp_handler.is_lamp_on());

        sensor.control_button = true;
        sensor.time = 50;
        st->handle_sensors(&sensor);
        EXPECT_EQ(HI_STATE_MANUAL_CONTROL, st->get_state());
        EXPECT_EQ(0, sensor.time);

        // button released no timeout
        sensor.control_button = false;
        sensor.time = TIME_WAIT_COUNT-1;
        st->handle_sensors(&sensor);
        EXPECT_EQ(HI_STATE_MANUAL_CONTROL, st->get_state());
        EXPECT_TRUE(lamp_handler.is_lamp_on());

        // timeout
        sensor.time = TIME_WAIT_COUNT+1;
        st->handle_sensors(&sensor);
        EXPECT_EQ(HI_STATE_OFF, st->get_state());
        EXPECT_FALSE(lamp_handler.is_lamp_on());

        // lights on, sound on
        sensor.light_sensor = true;
        sensor.microphone = true;
        st->handle_sensors(&sensor);
        EXPECT_EQ(HI_STATE_OFF, st->get_state());
        EXPECT_FALSE(lamp_handler.is_lamp_on());

        // light off, sound on
        sensor.light_sensor = false;
        st->handle_sensors(&sensor);
        EXPECT_EQ(HI_STATE_ON, st->get_state());
        EXPECT_FALSE(lamp_handler.is_lamp_on());// Changes next tick
        EXPECT_EQ(0, sensor.time);

        // quiet, no timeout
        sensor.microphone = false;
        st->handle_sensors(&sensor);
        EXPECT_EQ(HI_STATE_ON, st->get_state());
        EXPECT_TRUE(lamp_handler.is_lamp_on());

        // timeout
        sensor.time = TIME_WAIT_COUNT+1;
        st->handle_sensors(&sensor);
        EXPECT_EQ(HI_STATE_OFF, st->get_state());

        // FIXME add more cases

    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
