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



// - This structure defines the Task Information
struct st_TaskInfo {
	Task * pToAttach; // - Pointer to the Task
	uint64_t u64TickInterval; // - How often the task is executed (period)
	uint64_t u64ticks; // - Current tick count
	uint64_t u64TickIntervalInitValue; // - Initial tick value (phase)
};

class Scheduler
{
public:
    Scheduler();
    uint64_t m_u64ticks;
    return_e attach(Task * i_ToAttach, uint64_t i_u64TickInterval);
    return_e run(void);
  return_e PostAmble(void);
    return_e setup(void);
private:
    uint8_t m_u8OpenSlots; // - Available slots
    uint8_t m_u8NextSlot;  // - Next available slot
    st_TaskInfo m_aSchedule[NUMBER_OF_SLOTS]; // - Current schedule to be executed. THis should be
					      // - sorted by priority.
    st_TaskInfo m_aNextSchedule[NUMBER_OF_SLOTS]; // - Next schedule to be executed (not
						  // - implemented)
  st_TaskInfo* m_aTaskToExecute[MAX_TASKS_PER_FRAME]; // array with sorted tasks for execution
    return_e CalculateNextSchedule(void); // - Calculate next schedule tasks (not implemented)
    return_e SortScheduleByPriority(Task * i_pSchedule); // - Sorts a schedule based on priority
							 // - (not implemented)
  return_e UpdateTasksTicks(void); // - Updates timming info
};



#endif /* TASKS_SCHEDULER_HPP_ */
