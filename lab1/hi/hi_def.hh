/**
Copyright 2017 Daniel Garcia Vaglio degv364@gmail.com

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

//We must know the size during compilation time, to avoid using dynamic allocation
#define SAMPLES_PER_SECOND 5
#define MEAN_SECONDS 5
#define MAX_SAMPLES (SAMPLES_PER_SECOND*(MEAN_SECONDS+1))
#define MEAN_SAMPLES (SAMPLES_PER_SECOND * MEAN_SECONDS)

#ifndef HI_DEF
#define HI_DEF
typedef enum hi_return_e{
HI_RETURN_OK,
  HI_RETURN_FAIL,
  HI_RETURN_CRITICAL,
  HI_RETURN_BAD_PARAM
  } hi_return_e;

#endif
