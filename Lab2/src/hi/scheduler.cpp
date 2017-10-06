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
    int l_iNextTaskSlot = 0U;
    Task * l_pNextTask = (uintptr_t) 0;
    return_e l_eReturnCode = RETURN_OK;

    while(l_iNextTaskSlot < NUMBER_OF_SLOTS)
    {
    	l_pNextTask = static_cast<Task *> (m_aSchedule[l_iNextTaskSlot].pToAttach);
		if(l_pNextTask != ((uintptr_t) 0))
		{
			if(m_aSchedule[l_iNextTaskSlot].u64TickInterval == 0){
				l_pNextTask->run();
			}
			m_aSchedule[l_iNextTaskSlot].u64TickInterval ++;

			if(m_aSchedule[l_iNextTaskSlot].u64TickInterval > m_aSchedule[l_iNextTaskSlot].u64TickIntervalInitValue) {
				m_aSchedule[l_iNextTaskSlot].u64TickInterval = 0;
			}
		}
		l_iNextTaskSlot++;
    }
    CalculateNextSchedule(); // TODO

    return l_eReturnCode;
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
    return(RETURN_OK);
}
return_e Scheduler::SortScheduleByPriority(Task * i_pSchedule)
{
    return(RETURN_OK);
}
