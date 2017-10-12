#include "hi/scheduler.hh"

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
    // Execute all tasks marked for execution.
    for (uint8_t l_u8Slot = 1; l_u8Slot < LAST_TASK; l_u8Slot++)
    {
        if (m_aSchedule[l_u8Slot].pToAttach != ((uintptr_t) 0))
        {
            if (m_aSchedule[l_u8Slot].bExecute)
            {
                // Execute task
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
        else
        {
            return rt;
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
	  l_eReturnCode = m_aSchedule[l_u8Slot].pToAttach->setup();
	  if (l_eReturnCode != RETURN_OK){
	    return l_eReturnCode;
	  }
	}
    }
    return RETURN_OK;
}


return_e Scheduler::UpdateTasksTicks(void)
{
    for (uint8_t l_u8Slot = 0; l_u8Slot < LAST_TASK; l_u8Slot++)
    {
        if (m_aSchedule[l_u8Slot].pToAttach != ((uintptr_t) 0))
        {
            if (m_aSchedule[l_u8Slot].pToAttach->GetTaskType() == PERIODICAL)
            {
                m_aSchedule[l_u8Slot].u64ticks++;
                if (m_aSchedule[l_u8Slot].u64ticks
                        >= m_aSchedule[l_u8Slot].pToAttach->GetTaskTickInterval())
                {
                    m_aSchedule[l_u8Slot].bExecute = true;
                }
            }
        }
        else
        {
            return RETURN_OK;
        }
    }

    return RETURN_OK;
}

return_e Scheduler::FindTaskWithName(task_name_e i_eName,
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
    st_TaskInfo l_stCurrentTask;
    task_name_e l_eCurrentTaskName;
    bool l_bContinue = true;
    return_e rt;
    while (l_bContinue)
    {
        rt = InternalMessageQueue.PopMessage(&l_stCurrentMessage);
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
		l_stCurrentTask = m_aSchedule[l_eCurrentTaskName];
		// Check the task is valid
		if (l_stCurrentTask.pToAttach == ((uintptr_t) 0)){
		  return RETURN_FAIL;
		}
		// Add execution flag
                if (l_stCurrentTask.pToAttach->GetTaskType() == ONE_SHOT)
                {
                    l_stCurrentTask.bExecute = true;
                }
		// Trying to execute a task that is not triggered by a message.
		return RETURN_FAIL;
            default:
                // Unhandled message type
                return RETURN_FAIL;
            }
        }
        else
        {
            l_bContinue = false;
        }
    }
    return RETURN_OK;
}

return_e Scheduler::PostAmble(void)
{
    this->HandleInternalMessages(); // Side Effect of updating execution of One shot tasks
    this->UpdateTasksTicks(); // Side Effect of updating execution of Periodical tasks
    return RETURN_OK;
}
