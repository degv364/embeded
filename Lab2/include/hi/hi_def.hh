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

#ifndef TESTING
#include <stdint.h>
#endif

#ifndef HI_DEF_H_
#define HI_DEF_H_

// User defined. Every task has a unique name. Used for Messages
// Order defines priority
typedef enum task_name_e
{
    SCHEDULER = 0, // Messages to or from scheduler
    ADC_IRQ,       // Allocates heap memory for interrupts
    CALC_HORIZON,  // takes accel data transforms into horizon
    LCD_ISSUE,     // periodic task for triggering lcd
    LCD_DRAW,      // Draws a section of the lcd
    LAST_TASK      // Always last name
} task_name_e;

// User defined message_types
typedef enum message_type_e
{
    ADD_TO_EXECUTION = 0, // Add a task to execution queue (one shot tasks)
    ACCEL_DATA,           // Message with message data
    HORIZON_PARAMS,       // Parameters required to draw horizon
    RECTANGLES_TO_DRAW,   // List of rectangles that need to be drawn
    UNDEFINED_TYPE        // Always last type. For error handling
} message_type_e;

typedef struct message_t
{
    task_name_e m_eSender; // Who sends the message
    task_name_e m_eReceiver; // Who should receive the message
    message_type_e m_eMessageType; // Message type
    uint8_t m_u8Length; // Length of the message
    uint32_t* m_pData; // Pointer to the passed data
} message_t;

typedef enum task_type_e
{
    PERIODICAL = 0,
    ONE_SHOT,
    N_SHOT,
    ISR_HANDLER
} task_type_e;

#endif
