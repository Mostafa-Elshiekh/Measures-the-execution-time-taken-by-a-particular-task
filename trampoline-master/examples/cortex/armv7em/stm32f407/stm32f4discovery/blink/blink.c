#include "tp.h"
#include "tpl_os.h"
#include "stdio.h"
#include <stdbool.h>


#define APP_Task_blink_Measures_EXC_Tim_START_SEC_CODE
#include "tpl_memmap.h"



/**********Global Variables*******/
uint32_t CycleTime_ms       =0;
uint32_t EXC_Time_ms        =0;

TickType CycleTime_t;
TickType Rem_Time_ToEXP_t;
TickType End_time_t;
TickType EXC_time_t;


/************Functions*************/
void delayfun(long int DelayCounts)
{
	 for (long int i = 0; i < DelayCounts; ++i);
}



FUNC(int, OS_APPL_CODE) main(void)
{
  initBoard();
  StartOS(OSDEFAULTAPPMODE);
  return 0;
}

/***********TASKS************/
TASK(blink_Measures_EXC_Tim)
{
	int var = 0;
	for (var = 0; var < 4; ++var)
	{
		 ledOn(RED);
		 delayfun(1000);
		 ledOff(RED);
		 delayfun(1000);
	}
	 TerminateTask();
}

/****************************/


#define APP_Task_blink_Measures_EXC_Tim_STOP_SEC_CODE
#include "tpl_memmap.h"

#define OS_START_SEC_CODE
#include "tpl_memmap.h"
/*
 *  * This is necessary for ST libraries
 *   */
FUNC(void, OS_CODE) assert_failed(uint8_t* file, uint32_t line)
{
}


/**************HOOKS****************/
FUNC(void, OS_CODE) StartupHook(void)
{
	int i =0;
	for(i=0;i<4;i++)
	{
		  ledOn(ORANGE);
		  delayfun(500);
		  ledOff(ORANGE);
		  delayfun(500);
	}
	 ledOn(ORANGE);
}


FUNC(void, OS_CODE) PreTaskHook()
{
		  int i=0;
		  TaskType task_id = 0;

		  GetTaskID(&task_id);
		  if (task_id == blink_Measures_EXC_Tim)
		  {
			  ledOn(BLUE);
			  delayfun(10);
			  ledOff(BLUE);

				// Record the time before the task started
				GetAlarm(ALARM_blink_Measures_EXC_Tim, &Rem_Time_ToEXP_t);
				//Get the CycleTime for blink_Measures_EXC_Tim task
				CycleTime_t = (uint32_t)Rem_Time_ToEXP_t;
		  }
}


FUNC(void, OS_CODE) PostTaskHook()
{
		int i=0;
		TaskType task_id = 0;

		GetTaskID(&task_id);
		if (task_id == blink_Measures_EXC_Tim)
		{
		  ledOn(GREEN);
		  delayfun(10);
		  ledOff(GREEN);

		  GetAlarm(ALARM_blink_Measures_EXC_Tim, &Rem_Time_ToEXP_t);
		  //Get the counted tick EX:500 - 100 = 400
		  End_time_t = CycleTime_t - Rem_Time_ToEXP_t;

		  //Calculate the time difference
		  EXC_time_t = End_time_t;
		  EXC_Time_ms = (uint32_t)EXC_time_t; //tick time = 1ms
		}
}


#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

