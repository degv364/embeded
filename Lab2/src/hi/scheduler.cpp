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

#include "hi/scheduler.hh"

// Variable to know if frame is being executed
extern volatile bool g_bDuringFrame;

// Variable to determine timeout condition
extern volatile bool g_bTimeoutCondition;

// - Scheduler constructor
Scheduler::Scheduler()
{
    m_u8NextSlot = 0;
    for (int index = 0; index < NUMBER_OF_SLOTS; index++)
    {
        m_aSchedule[index].pToAttach = (uintptr_t) 0; // Init to an invalid pointer
    }
    InternalMessageQueue = MessageQueue();
    m_InternalHeap = Heap();
    return;
}
// - The attach function, inserts the task into the schedule slots.
return_e Scheduler::attach(Task * i_ToAttach)
{
  st_TaskInfo l_stTempTask;
    if (i_ToAttach == 0){
      return RETURN_BAD_PARAM;
    }
    // Initialize struct
    l_stTempTask.pToAttach = i_ToAttach;
    l_stTempTask.bExecute = false;
    // FIXME: assign a random integer from 0 to tick_interval
    l_stTempTask.u64ticks = this->m_u64ticks;

    // Attach task
    m_aSchedule[l_stTempTask.pToAttach->GetTaskName()] = l_stTempTask;
    return RETURN_OK;
}
// - Execute the current schedule
return_e Scheduler::run(void)
{
    return_e rt = RETURN_OK;
    uint8_t l_u8ExecutedTasks = 0;
    if (g_bTimeoutCondition)
      return RETURN_TIMEOUT;
    g_bDuringFrame = true;
    // Execute all tasks marked for execution.
    for (uint8_t l_u8Slot = 1; l_u8Slot < LAST_TASK; l_u8Slot++)
    {
        if (m_aSchedule[l_u8Slot].pToAttach != ((uintptr_t) 0))
        {
            if (m_aSchedule[l_u8Slot].bExecute)
            {
                rt = m_aSchedule[l_u8Slot].pToAttach->run();
                if (rt != RETURN_OK)
                {
                    return rt;
                }
                // Reset its execution status.
                m_aSchedule[l_u8Slot].bExecute = false;
                m_aSchedule[l_u8Slot].u64ticks = 0;

                l_u8ExecutedTasks++;
                if (l_u8ExecutedTasks >= MAX_TASKS_PER_FRAME)
                {
                    return RETURN_FAIL;
                }
            }
        }
    }
    return rt;
}
// - Execute the setup function for all tasks
return_e Scheduler::setup(void)
{
    return_e l_eReturnCode = RETURN_OK;

    for (uint8_t l_u8Slot = 1; l_u8Slot < LAST_TASK; l_u8Slot++){
       if (m_aSchedule[l_u8Slot].pToAttach != ((uintptr_t) 0))
       {
           l_eReturnCode = m_aSchedule[l_u8Slot].pToAttach->setup(&m_InternalHeap);
           if (l_eReturnCode != RETURN_OK){
               return l_eReturnCode;
           }
       }
    }
    return RETURN_OK;
}



inline return_e Scheduler::HandleExternalMessages(uint8_t i_u8CurrentSlot){
  return_e rt;
  message_t l_stTempMessage;
  rt = m_aSchedule[i_u8CurrentSlot].pToAttach->Outgoing.PopMessage(&l_stTempMessage);
  while (rt != RETURN_EMPTY) {
    // Check for fails (not empty)
      if (rt != RETURN_OK){
          return rt;
      }
      // Check if the message is for the scheduler
      if (l_stTempMessage.receiver == SCHEDULER){
          rt = InternalMessageQueue.AddMessage(l_stTempMessage);
          if (rt != RETURN_OK){
              return rt;
          }
      }
      else {
          // Send to the specific task
          rt =  m_aSchedule[l_stTempMessage.receiver].pToAttach->Incoming.AddMessage(l_stTempMessage);
          if (rt != RETURN_OK){
              return rt;
          }
      }
      // Get next message
      rt = m_aSchedule[i_u8CurrentSlot].pToAttach->Outgoing.PopMessage(&l_stTempMessage);
  }

  return RETURN_OK;
}

inline return_e Scheduler::HandleTimming(uint8_t i_u8CurrentSlot){
  if (m_aSchedule[i_u8CurrentSlot].pToAttach->GetTaskType() == PERIODICAL){
      m_aSchedule[i_u8CurrentSlot].u64ticks++;
      if (m_aSchedule[i_u8CurrentSlot].u64ticks
              >= m_aSchedule[i_u8CurrentSlot].pToAttach->GetTaskTickInterval())
      {
          m_aSchedule[i_u8CurrentSlot].bExecute = true;
      }
  }
  return RETURN_OK;
}


inline return_e Scheduler::FindTaskWithName(task_name_e i_eName,
                                     st_TaskInfo* o_stTaskInfo)
{
  if (i_eName >= LAST_TASK){
    return RETURN_FAIL;
  }
  o_stTaskInfo = &m_aSchedule[i_eName];
  return RETURN_OK;
}

return_e Scheduler::HandleInternalMessages(void)
{
    message_t l_stCurrentMessage;
    st_TaskInfo* l_stCurrentTask;
    task_name_e l_eCurrentTaskName;
    return_e rt;
    if (g_bTimeoutCondition)
      return RETURN_TIMEOUT;

    rt = InternalMessageQueue.PopMessage(&l_stCurrentMessage);
    while (rt != RETURN_EMPTY)
    {
        if (rt == RETURN_OK)
        {
            switch (l_stCurrentMessage.message_type)
            {
            case ADD_TO_EXECUTION:
                // Get the name of the task to execute
                l_eCurrentTaskName = (task_name_e) *l_stCurrentMessage.data;
                if (l_eCurrentTaskName >= LAST_TASK){
                    return RETURN_FAIL;
                }
                l_stCurrentTask = &m_aSchedule[l_eCurrentTaskName];

                // Check the task is valid
                if (l_stCurrentTask->pToAttach == ((uintptr_t) 0)){
                    return RETURN_FAIL;
                }
                // Add execution flag
                if (l_stCurrentTask->pToAttach->GetTaskType() == ONE_SHOT)
                {
                    l_stCurrentTask->bExecute = true;
                }
                else {
                    // Trying to execute a not ONE_SHOT task (triggered by a message)
                    return RETURN_FAIL;
                }
                break;
            default:
                // Unhandled message type
                return RETURN_FAIL;
            }
        }
	else
	  return rt;

        rt = InternalMessageQueue.PopMessage(&l_stCurrentMessage);
    }

    return RETURN_OK;
}



return_e Scheduler::PostAmble(void)
{
    return_e rt;
    for (uint8_t l_u8Slot = 1; l_u8Slot < LAST_TASK; l_u8Slot++){
        if (m_aSchedule[l_u8Slot].pToAttach != ((uintptr_t) 0)){
            // beacuse one does not know when an interrupt hapens. Set that it is always executing.
            if (m_aSchedule[l_u8Slot].pToAttach->GetTaskType() == ISR_HANDLER){
                m_aSchedule[l_u8Slot].pToAttach->SetFinishedState(false);
            }
            rt = HandleExternalMessages(l_u8Slot); // Side effect of populating internal message queue
            if(rt != RETURN_OK){
                return rt;
            }
            rt = HandleTimming(l_u8Slot); // Side effect of udating timming for periodical tasks
            if(rt != RETURN_OK){
                return rt;
            }
        }
    }

    this->HandleInternalMessages(); // Side Effect of updating execution of One shot tasks
    if(rt != RETURN_OK){
        return rt;
    }
  
    if (g_bTimeoutCondition)
        return RETURN_TIMEOUT;

    g_bDuringFrame = false;

    return RETURN_OK;
}
