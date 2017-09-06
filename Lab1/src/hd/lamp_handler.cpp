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

// FIXME: this is a hack to not use 'new' and avoid using the heap
static periph::OutputGPIO internal_lamp0(GPIO_PORT_P1, GPIO_PIN0);
// FIXME: Use the correct pins
static periph::OutputGPIO internal_lamp1(GPIO_PORT_P2, GPIO_PIN1);
static periph::OutputGPIO internal_lamp2(GPIO_PORT_P3, GPIO_PIN2);

periph::LampHandler::LampHandler(uint16_t initial_enabled_lamps) :
        lamp0_(&internal_lamp0), lamp1_(&internal_lamp1), lamp2_(
                &internal_lamp2)
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

    // FIXME: remove this part once the correct pins are used
    enabled_lamps_ = 1;
}

return_e periph::LampHandler::lamps_on()
{
    switch (enabled_lamps_)
    {
    case 1:
        lamp0_->set();
        lamp1_->reset();
        lamp2_->reset();
        break;
    case 2:
        lamp0_->set();
        lamp1_->set();
        lamp2_->reset();
        break;
    case 3:
        lamp0_->set();
        lamp1_->set();
        lamp2_->set();
        break;
    default:
        lamp0_->reset();
        lamp1_->reset();
        lamp2_->reset();

    }
    return RETURN_OK;
}

return_e periph::LampHandler::lamps_off()
{
    lamp0_->reset();
    lamp1_->reset();
    lamp2_->reset();
    return RETURN_OK;
}

return_e periph::LampHandler::lamps_toggle()
{
    switch (enabled_lamps_)
    {
    case 1:
        lamp0_->toggle();
        lamp1_->reset();
        lamp2_->reset();
        break;
    case 2:
        lamp0_->toggle();
        lamp1_->toggle();
        lamp2_->reset();
        break;
    case 3:
        lamp0_->toggle();
        lamp1_->toggle();
        lamp2_->toggle();
        break;
    default:
        lamp0_->reset();
        lamp1_->reset();
        lamp2_->reset();
    }
    return RETURN_OK;
}
