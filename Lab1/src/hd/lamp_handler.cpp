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

#include "hd/periph.hh"

periph::LampHandler::LampHandler(uint16_t initial_enabled_lamps) :
        lamp0_(GPIO_PORT_P2, GPIO_PIN0), //R
        lamp1_(GPIO_PORT_P2, GPIO_PIN1), //G
        lamp2_(GPIO_PORT_P2, GPIO_PIN2)  //B
{
    set_enabled_lamps(initial_enabled_lamps);
}

void periph::LampHandler::set_enabled_lamps(uint16_t new_enabled_lamps)
{
    // Check for valid inputs
    if (new_enabled_lamps == 0 || new_enabled_lamps > 3)
    {
        enabled_lamps_ = 1;
    }
    // set enabled lamps value
    enabled_lamps_ = new_enabled_lamps;
}

return_e periph::LampHandler::lamps_on(void)
{
    if(enabled_lamps_ >= 1) lamp0_.set();
    else lamp0_.reset();

    if(enabled_lamps_ >= 2) lamp1_.set();
    else lamp1_.reset();

    if(enabled_lamps_ >= 3) lamp2_.set();
    else lamp2_.reset();

    return RETURN_OK;
}

return_e periph::LampHandler::lamps_off(void)
{
    lamp0_.reset();
    lamp1_.reset();
    lamp2_.reset();

    return RETURN_OK;
}

return_e periph::LampHandler::lamps_toggle(void)
{
    if(enabled_lamps_ >= 1) lamp0_.toggle();
    else lamp0_.reset();

    if(enabled_lamps_ >= 2) lamp1_.toggle();
    else lamp1_.reset();

    if(enabled_lamps_ >= 3) lamp2_.toggle();
    else lamp2_.reset();

    return RETURN_OK;
}

return_e periph::LampHandler::lamps_alive_sequence(uint32_t wait_cycles)
{
    lamps_off();

    for (int j = 0; j < 3; j++)
    {
        lamps_toggle(); //Lamps On
        for (uint32_t i = 0; i < wait_cycles; i++);
        lamps_toggle(); //Lamps Off
        for (uint32_t i = 0; i < wait_cycles; i++);
    }
    return RETURN_OK;
}
