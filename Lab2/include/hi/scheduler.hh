/*
 * Scheduler.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef TASKS_SCHEDULER_H_
#define TASKS_SCHEDULER_H_
#include "hi/hi_def.hh"
#include "task.hh"
#include "common_def.hh"
#include "message_queue.hh"
#include "heap.hh"



// - This structure defines the Task Information
struct st_TaskInfo
{
    Task * pToAttach; // - Pointer to the Task
    uint64_t u64ticks; // - Current tick count
    bool bExecute; // True if the task should be executed
};

class Scheduler
{
public:
    Scheduler();
    uint64_t m_u64ticks;
    // Public member, so that one can call its methods directly. Also, to make its
    // address accesible for interruption callbacks
    Heap m_InternalHeap;
    return_e attach(Task * i_ToAttach);
    return_e run(void);
    return_e PostAmble(void);
    return_e setup(void);

  // FIXME: this method is public to be able to add messages from the interrupts
  // should have a better way of doing this
  //return_e AddInternalMessage(message_t i_NewMessage);
  
private:
    MessageQueue InternalMessageQueue; // Messages for the scheduler
    uint8_t m_u8NextSlot;  // - Next available slot
    st_TaskInfo m_aSchedule[LAST_TASK]; // - Current schedule to be executed. This is
    // - sorted by priority.
  
    /**
     * Updates the ticks for each task in the schedule. Also finds which Periodical tasks
     * should be executed in the next frame.
     */
    return_e UpdateTasksTimingAndFinishedState(void);
    /**
     * Handles the messages sent to the Schedule. In particular sets tasks to be executen with ADD_TO_EXECUTION
     * message.
     */
    return_e HandleInternalMessages(void);
    /**
     * Message traffic
     */
    return_e HandleExternalMessages(void);


    return_e FindTaskWithName(task_name_e i_eName, st_TaskInfo* o_stTaskInfo);
};

#endif /* TASKS_SCHEDULER_HPP_ */
