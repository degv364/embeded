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
typedef enum task_name_e
{
    SCHEDULER = 0, // Messages to or from scheduler
    IRQ_ALLOCATOR, // Allocates heap memory for interrupts
    ACCEL_HANDLER, // takes accel data transxforms into horizon
    LCD_TRIGGER,   // periodic task for triggering lcd
    LCD_DRAWER,    // Draws a section of the lcd
    LAST_TASK      //Always last name. 
} task_name_e;

// User defined message_types
typedef enum message_type_e
{
    ADD_TO_EXECUTION = 0, // Add a task to execution queue (one shot tasks)
    UNDEFINED_TYPE // Always last type. For error handling
} message_type_e;

typedef struct message_t
{
    task_name_e sender; // Who sends the message
    task_name_e receiver; // Who should receive the message
    message_type_e message_type; // Message type
    uint8_t length; // Length of the message
    uint32_t* data; // Pointer to the passed data
} message_t;

typedef enum task_type_e
{
    PERIODICAL = 0, ONE_SHOT, N_SHOT,
} task_type_e;

// Priorities
typedef enum priority_e
{
    NONE = 0, VERY_LOW, LOW, MEDIUM, HIGH, VERY_HIGH, CRITICAL
} priority_e;

// States
typedef enum hi_state_e
{
    HI_STATE_NONE = 0,   // No state
    HI_STATE_INIT,       // Initialize peripherals
    HI_STATE_ALIVE_SEQ,  // Functioning confirmation
    HI_STATE_ON,         // Lamp ON
    HI_STATE_OFF,        // Lamp OFF
    HI_STATE_DEINIT,     // Deinitialize
    HI_STATE_FAIL,       // Failure occurred
    HI_STATE_MANUAL_ON   // Manual button ON state
} hi_state_e;

typedef struct hi_sensor_t
{
    uint16_t time;       //Software time counter (managed by Timer32)
    bool control_button; //Manual control button activation flag
    bool light_sensor;   //Light sensor condition (high level=True)
    bool microphone;     //Microphone loud flag (loud=True)
} hi_sensor_t;

#endif
