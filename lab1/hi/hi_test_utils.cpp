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
#include <cstdint>
#include "hi_def.hh"
#include "hi_utils.hpp"
#include "gtest/gtest.h"
#include <iostream>

namespace{
  TEST(Hi_Dual_Mean_Fifo, Constants){
    ASSERT_EQ(MAX_SAMPLES, 30);
    ASSERT_EQ(MEAN_SAMPLES, 25);
  }

  TEST(Hi_Dual_Mean_Fifo, Invalid_During_Init){
    Hi_dual_mean_fifo<int, float> *dual_fifo = new Hi_dual_mean_fifo<int, float>();
    bool comp_condition;
    (void) dual_fifo->is_last_second_big(&comp_condition);
    EXPECT_FALSE(comp_condition);
  }

  TEST(Hi_Dual_Mean_Fifo, Add__Values){
    Hi_dual_mean_fifo<int, float> *dual_fifo = new Hi_dual_mean_fifo<int, float>();
    bool comp_condition;
    for (int i=0; i<MAX_SAMPLES*2; i++){
      dual_fifo->add_sample(4);
    }
    
    EXPECT_FLOAT_EQ(4, dual_fifo->get_mean());

    (void) dual_fifo->is_last_second_big(&comp_condition);
    EXPECT_FALSE(comp_condition);

    
    for (int i=0; i<=SAMPLES_PER_SECOND; i++){
      dual_fifo->add_sample(20);
    }
    
    (void) dual_fifo->is_last_second_big(&comp_condition);
    EXPECT_TRUE(comp_condition);
    
  }
}



int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
