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

#include "tasks/adc_irq_task.hh"

AdcIRQTask::AdcIRQTask(void) :
    m_AccelADC(ACCEL_ADC_SAMPLES_PER_SECOND)
{
    this->SetTaskName(ADC_IRQ);
    this->SetTaskType(ISR_HANDLER);
}


return_e AdcIRQTask::Setup(Heap* i_Heap){

  this->SetTaskExecutionCondition(false);
  this->SetTaskTickInterval(0);

  i_Heap->Allocate(HEAP_MEM_SIZE, &m_pHeapMem);


  m_AccelADC.Setup();
  m_AccelADC.Start();

  return RETURN_OK;
}
