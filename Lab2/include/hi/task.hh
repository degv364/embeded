#ifndef TASK_HPP_
#define TASK_HPP_

#include "msp.h"
#include "hi_def.hh"
#include "common_def.hh"
#include "message_queue.hh"
#include "heap.hh"

class Task
{

private:
    task_name_e m_eTaskName; // Unique task identifier
    task_type_e m_eTaskType;    // Task type
    uint64_t m_u64TickInterval; // Period of execution for periodic tasks
    bool m_bExecutionCondition; // ExecutionCondition for One shot tasks

protected:
    //Finished state
    bool m_bIsFinished;

public:
    Task();

    // Messages
    MessageQueue Incoming;
    MessageQueue Outgoing;


    virtual return_e run(void){return (RETURN_OK);}
    // Receives a pointer to the heap for memory allocation
    virtual return_e setup(Heap* i_Heap){return (RETURN_OK);}
    bool IsTaskFinished(void){return m_bIsFinished;}

    // Gets
    task_type_e GetTaskType(void){return m_eTaskType;}
    uint64_t GetTaskTickInterval(void){return m_u64TickInterval;}
    bool GetTaskExecutionCondition(void){return m_bExecutionCondition;}
    task_name_e GetTaskName(void){return m_eTaskName;}

    // Sets
    void SetTaskType(task_type_e i_eNewTaskType){m_eTaskType = i_eNewTaskType;}
    void SetTaskTickInterval(uint64_t i_u64NewTickInterval){m_u64TickInterval = i_u64NewTickInterval;}
    void SetTaskExecutionCondition(bool i_bNewExecutionCondition){m_bExecutionCondition = i_bNewExecutionCondition;}
    void SetTaskName(task_name_e i_eNewTaskName){m_eTaskName = i_eNewTaskName;}
    void SetFinishedState(bool i_bFinishedState){m_bIsFinished = i_bFinishedState;}
};

#endif /* TASK_HPP_ */
