#include "hi/scheduler.hh"

// - Scheduler constructor
Scheduler::Scheduler()
{
    m_u8OpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS);
    m_u8NextSlot = 0;
    for (int index = 0; index < NUMBER_OF_SLOTS; index++)
    {
        m_aSchedule[index].pToAttach = (uintptr_t) 0; // Init to an invalid pointer
    }
    InternalMessageQueue = MessageQueue();
    return;
}
// - The attach function, inserts the task into the schedule slots.
return_e Scheduler::attach(Task * i_ToAttach, uint64_t i_u64TickInterval)
{
    return_e l_eErrorCode = RETURN_OK;
    st_TaskInfo l_st_StructToAttach;

    l_st_StructToAttach.pToAttach = i_ToAttach;
    // FIXME: assign a random integer from 0 to tick_interval
    l_st_StructToAttach.u64ticks = this->m_u64ticks;

    if ((m_u8OpenSlots > 0) && (m_u8NextSlot < NUMBER_OF_SLOTS))
    {
        m_aSchedule[m_u8NextSlot] = l_st_StructToAttach;
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
    return_e rt = RETURN_OK;
    uint8_t l_u8ExecutedTasks = 0;
    // Execute all tasks marked for execution.
    for (uint8_t l_u8Slot = 0; l_u8Slot < NUMBER_OF_SLOTS; l_u8Slot++)
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
    int l_iNextTaskSlot = 0U;
    Task * l_pNextTask = (uintptr_t) 0;
    return_e l_eReturnCode = RETURN_OK;

    while (l_iNextTaskSlot < NUMBER_OF_SLOTS)
    {
        l_pNextTask =
                static_cast<Task *>(m_aSchedule[l_iNextTaskSlot].pToAttach);
        if (l_pNextTask != ((uintptr_t) 0))
        {
            l_pNextTask->setup();
        }
        l_iNextTaskSlot++;
    }
    return l_eReturnCode;
}

return_e Scheduler::CalculateNextSchedule(void)
{
    return RETURN_OK;
}
return_e Scheduler::SortScheduleByPriority(Task * i_pSchedule)
{
    return (RETURN_OK);
}

return_e Scheduler::UpdateTasksTicks(void)
{
    for (uint8_t l_u8Slot = 0; l_u8Slot < NUMBER_OF_SLOTS; l_u8Slot++)
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
    for (uint8_t l_u8Slot = 0; l_u8Slot < NUMBER_OF_SLOTS; l_u8Slot++)
    {
        if (m_aSchedule[l_u8Slot].pToAttach != ((uintptr_t) 0))
        {
            if (m_aSchedule[l_u8Slot].pToAttach->GetTaskName() == i_eName)
            {
                o_stTaskInfo = &m_aSchedule[l_u8Slot];
                return RETURN_OK;
            }
        }
    }
    // No task was found to have that name.
    return RETURN_FAIL;
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
                rt = this->FindTaskWithName(l_eCurrentTaskName,
                                            &l_stCurrentTask);
                if (rt != RETURN_OK)
                {
                    // no task was found
                    return RETURN_FAIL;
                }
                if (l_stCurrentTask.pToAttach->GetTaskType() == ONE_SHOT)
                {
                    l_stCurrentTask.bExecute = true;
                }
                else
                {
                    // Trying to execute a task that is not triggered by a message.
                    return RETURN_FAIL;
                }
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
}

return_e Scheduler::PostAmble(void)
{
    this->HandleInternalMessages(); // Side Effect of updating execution of One shot tasks
    this->UpdateTasksTicks(); // Side Effect of updating execution of Periodical tasks
    return RETURN_OK;
}
