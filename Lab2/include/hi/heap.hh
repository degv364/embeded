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

#include "common_def.hh"
#include "hi_def.hh"

#define HEAP_SIZE 1024 // 4KB
#define MAP_SIZE 32    // One bit per uint32

#ifndef HEAP_
#define HEAP_


class Heap{
private:
  uint32_t m_aData[HEAP_SIZE];
  // Not implemented for release one!
  //uint32_t m_aAllocatedMap[32];
  uint32_t m_u32LastAdded;

public:
  Heap(void);
  // Allocate memory, return OK if found space
  return_e Allocate(uint8_t i_u8Size, uint32_t* o_u32HeapPointer);
  // Frees memory
  /* Not implemented for fisrt release!
  return_e Free( uint8_t i_u8Size, uint32_t* i_u32HeapPointer);
  // Returns amount of available memory in bytes
  return_e GetFreeSpace(uint32_t* o_u32FreeSpace);
  */
};

#endif
