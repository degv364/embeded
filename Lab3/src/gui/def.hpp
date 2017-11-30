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

#ifndef DEFINES_QT_APP
#define DEFINES_QT_APP

/*
 * All sizes are normalized to this resolution. Use
 * Window::transformResolution to adapt to current window size
 */

#define DEFAULT_SCREEN_WIDTH  1366
#define DEFAULT_SCREEN_HEIGHT 768

#define BIG_ICON_SIZE (1366/3)

#define BIG_BUTTON_WIDTH (1366/5)
#define BIG_BUTTON_HEIGHT (1366/5)

#define LITTLE_BUTTON_WIDTH 40
#define LITTLE_BUTTON_HEIGHT 40

// FIXME: this default should be overwritten with actual values
#define DEFAULT_SONG_LENGTH 30000 //30s
#define DEFAULT_TIMER_INTERVAL 100 // 100ms

#define DEFAULT_ANIMATION_DURATION 500 //0.5s

#define MAX_PATH_LENGTH 80


#endif
