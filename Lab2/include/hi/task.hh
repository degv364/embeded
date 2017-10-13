#ifndef TASK_HPP_
#define TASK_HPP_

#include "msp.h"
#include "hi_def.hh"
#include "common_def.hh"
#include "message_queue.hh"

class Task
{

private:
    task_name_e m_eTaskName; // Unique task identifier
    task_type_e m_eTaskType;    // Task type
    uint64_t m_u64TickInterval; // Period of execution for periodic tasks
    bool m_bExecutionCondition; // ExecutionCondition for One shot tasks

    // Messages
  MessageQueue Incoming;
  MessageQueue Outgoing;

protected:
    bool m_bIsFinished;

public:
    Task();
    virtual return_e run(void){return (RETURN_OK);};
    virtual return_e setup(void){return (RETURN_OK);};
    bool IsTaskFinished(void){return m_bIsFinished;};

  // Scheduler will put messages here, for the task to handle
  return_e ReceiveMessage(message_t i_stNewMessage);
  // Scheduler will take messages from here
  return_e PopMessage(message_t* o_stMessage);

    // Gets
    task_type_e GetTaskType(void){return m_eTaskType;};
    uint64_t GetTaskTickInterval(void){return m_u64TickInterval;};
    bool GetTaskExecutionCondition(void){return m_bExecutionCondition;};
    task_name_e GetTaskName(void){return m_eTaskName;};

    // Sets
    void SetTaskType(task_type_e i_eNewTaskType){m_eTaskType = i_eNewTaskType;};
    void SetTaskTickInterval(uint64_t i_u64NewTickInterval){m_u64TickInterval = i_u64NewTickInterval;};
    void SetTaskExecutionCondition(bool i_bNewExecutionCondition){m_bExecutionCondition = i_bNewExecutionCondition;};
    void SetTaskName(task_name_e i_eNewTaskName){m_eTaskName = i_eNewTaskName;};
};

#endif /* TASK_HPP_ */
