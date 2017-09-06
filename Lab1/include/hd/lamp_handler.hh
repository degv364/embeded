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

#ifndef INCLUDE_HD_LAMP_HANDLER_HH_
#define INCLUDE_HD_LAMP_HANDLER_HH_

#include "hd/periph.hh"

namespace periph
{

/*
 * Class to hide the number of lamps enabled complexity
 */
class LampHandler
{
private:
    periph::OutputGPIO *lamp0_;
    periph::OutputGPIO *lamp1_;
    periph::OutputGPIO *lamp2_;

    uint16_t enabled_lamps_;

public:
    LampHandler(uint16_t initial_enabled_lamps);

    void set_enabled_lamps(uint16_t new_enabled_lamps);

    // Turn ON the number of enabled lamps
    return_e lamps_on(void);
    // Turn OFF all lamps
    return_e lamps_off(void);
    // Toggle the number of enabled lamps
    return_e lamps_toggle(void);

};

}

#endif /* INCLUDE_HD_LAMP_HANDLER_HH_ */
