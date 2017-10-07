#ifndef TASK_HPP_
#define TASK_HPP_

#include "msp.h"
#include "hi_def.hh"

#define NO_ERR 0
#define RET_ERR 1

class Task
{
	public:
		Task();
		uint8_t m_u8TaskID;
		virtual uint8_t     run(void){return(0);};
		virtual uint8_t     setup(void){return(0);};
		bool                IsTaskFinished(void){return m_bIsFinished;};
		priority_e          GetTaskPriority(void) {return m_ePriority;};
		void                SetTaskPriority(priority_e i_eNewPriority){m_ePriority = i_eNewPriority;};
	private:
	   static uint8_t m_u8NextTaskID;
	   priority_e m_ePriority;
	protected:
	   bool m_bIsFinished;
};

#endif /* TASK_HPP_ */
