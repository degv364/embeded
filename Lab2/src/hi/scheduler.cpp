#include "hi/scheduler.hh"

// - Scheduler constructor
Scheduler::Scheduler()
{
    m_u8OpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS);
    m_u8NextSlot = 0;
    for(int index = 0; index < NUMBER_OF_SLOTS; index++)
    {
        m_aSchedule[index].pToAttach = (uintptr_t) 0; // Init to an invalid pointer
    }
    return;
}
// - The attach function, inserts the task into the schedule slots.
return_e Scheduler::attach(Task * i_ToAttach, uint64_t i_u64TickInterval)
{
    return_e l_eErrorCode = RETURN_OK;
    st_TaskInfo l_st_StructToAttach;

    l_st_StructToAttach.pToAttach = i_ToAttach;
    l_st_StructToAttach.u64ticks = this->m_u64ticks;
    l_st_StructToAttach.u64TickInterval = 0;
	l_st_StructToAttach.u64TickIntervalInitValue = i_u64TickInterval;

    if((m_u8OpenSlots>0) && (m_u8NextSlot < NUMBER_OF_SLOTS))
    {
        m_aSchedule[m_u8NextSlot] =  l_st_StructToAttach;
        m_u8OpenSlots--;
        m_u8NextSlot++;
    }
    else
    {
        l_eErrorCode = RETURN_OK;
    }
    return l_eErrorCode;
}
// - Execute the current schedule
return_e Scheduler::run(void)
{

  return_e l_RetVal = RETURN_OK;
  for (uint8_t l_u8Slot=0; l_u8Slot<MAX_TASKS_PER_FRAME; l_u8Slot++){
    if (m_aTaskToExecute[l_u8Slot]){
      return l_RetVal;
    }
    // FIXME: check for return value
    m_aTaskToExecute[l_u8Slot]->pToAttach->run();
  }
  return l_RetVal;
}
// - Execute the setup function for all tasks
return_e Scheduler::setup(void)
{
    int l_iNextTaskSlot = 0U;
    Task * l_pNextTask = (uintptr_t) 0;
    return_e l_eReturnCode = RETURN_OK;

    while(l_iNextTaskSlot < NUMBER_OF_SLOTS)
    {
        l_pNextTask = static_cast<Task *> (m_aSchedule[l_iNextTaskSlot].pToAttach);
        if(l_pNextTask != ((uintptr_t) 0))
        {
            l_pNextTask->setup();
        }
        l_iNextTaskSlot++;
    }
    return l_eReturnCode;
}

return_e Scheduler::CalculateNextSchedule(void)
{

  bool l_bNextTaskPresent = true;
  bool l_bSpaceLeft = true;
  uint8_t l_u8CurrentTaskSlot = 0U;
  uint8_t l_u8TaskExecuteSlot = 0U;
  st_TaskInfo* l_pCurrentTaskInfo;

  // First invalidate all slots in tasks to execute array
  for (uint8_t l_u8Slot=0; l_u8Slot<MAX_TASKS_PER_FRAME; l_u8Slot++){
    m_aTaskToExecute[l_u8Slot]=0;
  }

  // If tasks are not executed on the current frame their tick count will be grater than the
  // defined interval. 
  
  while (l_bNextTaskPresent || l_bSpaceLeft){
    l_pCurrentTaskInfo = &(m_aSchedule[l_u8CurrentTaskSlot]);
    if (l_pCurrentTaskInfo->u64ticks >= l_pCurrentTaskInfo->u64TickInterval){
      m_aTaskToExecute[l_u8TaskExecuteSlot] = l_pCurrentTaskInfo;
      l_pCurrentTaskInfo->u64ticks = 0;
    }
    // Check if it is required to continue
    l_u8TaskExecuteSlot++;
    l_bSpaceLeft = l_u8TaskExecuteSlot<MAX_TASKS_PER_FRAME;
    l_u8CurrentTaskSlot++;
    if (l_u8CurrentTaskSlot< NUMBER_OF_SLOTS){
      l_bNextTaskPresent = (m_aSchedule[l_u8CurrentTaskSlot].pToAttach!= ((uintptr_t) 0));
    }
    else{
      l_bNextTaskPresent = false;
    }
    
  }
    return RETURN_OK;
}
return_e Scheduler::SortScheduleByPriority(Task * i_pSchedule)
{
    return(RETURN_OK);
}


return_e Scheduler::UpdateTasksTicks(void){
  for (uint8_t l_u8Slot=0; l_u8Slot<NUMBER_OF_SLOTS; l_u8Slot++){
    if (m_aSchedule[l_u8Slot].pToAttach!= ((uintptr_t) 0)){
      m_aSchedule[l_u8Slot].u64ticks++;
    }
    else{
      return RETURN_OK;
    }
  }

  return RETURN_OK;
}


return_e Scheduler::PostAmble(void){
  this->UpdateTasksTicks();
  this->CalculateNextSchedule();
  return RETURN_OK;
}
