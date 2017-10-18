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

#include "hi/filters.hh"

MeanFilter::MeanFilter(){
  m_fFilteredResult = 0.0f;
  for (m_u8Index=0; m_u8Index<MEAN_FILTER_BUFFER_SIZE; m_u8Index++){
    m_aValuesBuffer[m_u8Index]= 0U;
  }
  m_u16OperationsDone = 0;
  m_bSetup = false;
}


return_e
MeanFilter::Setup(uint16_t i_u16InitialValue){
  m_bSetup = true;
  for (m_u8Index = 0; m_u8Index<MEAN_FILTER_BUFFER_SIZE; m_u8Index++){
    m_aValuesBuffer[m_u8Index]= i_u16InitialValue;
  }
  m_u8Index -= 1;
  m_fFilteredResult = (float)i_u16InitialValue;
  return RETURN_OK;
}

return_e
MeanFilter::AddValue(uint16_t i_u16NewValue){
  // Do not allow adding new values if the filter is not setup
  if (!m_bSetup) return RETURN_INVALID_VALUE;
  // Update mean
  m_fFilteredResult =  m_fFilteredResult +
    ((((float) i_u16NewValue)-((float) m_aValuesBuffer[m_u8Index]))/MEAN_FILTER_BUFFER_SIZE);
  // Replace value
  m_aValuesBuffer[m_u8Index] = i_u16NewValue;
  m_u8Index = (m_u8Index+1)%MEAN_FILTER_BUFFER_SIZE;
  // Increase count for recalibration
  m_u16OperationsDone++;
  // Because we use deltas to get the mean value, we need recalibration. Every operation adds an
  // error.
  if (m_u16OperationsDone == MEAN_FILTER_CALIBRATION_PERIOD){
    m_u16OperationsDone = 0;
    m_fFilteredResult = 0;
    for (uint8_t l_u8Index=0; l_u8Index<MEAN_FILTER_BUFFER_SIZE; l_u8Index++){
      m_fFilteredResult += (float) m_aValuesBuffer[l_u8Index]/MEAN_FILTER_BUFFER_SIZE;
    }
  }
  return RETURN_OK;
}

return_e
MeanFilter::GetFilteredValue(uint16_t* o_u16FilteredValue){
  // Return if not set up
  if (!m_bSetup) return RETURN_INVALID_VALUE;
  *o_u16FilteredValue = (uint16_t) m_fFilteredResult;
  return RETURN_OK;
}
