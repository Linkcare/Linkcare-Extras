#include "lpsa.h"
#include "task.h"
#include "System.h"
#include "DeviceInfo.h"

void main(void)
{
#if (A20_DEBUG == ON)					  
	#warning "A20_DEBUG enabled"	
#else					  
	#warning "A20_DEBUG disabled"			  
#endif

#if	(BACKUP == ON)
	#warning "BACKUP enabled"
#else
	#warning "BACKUP disabled"
#endif 

#if	(TAP == ON)
	#warning "TAP enabled"
#else
	#warning "TAP disabled"
#endif 
  
#if	(REALTIME == ON)
	#warning "REALTIME enabled"
#else
	#warning "REALTIME disabled"
#endif
  
#if	(WATCHDOG == ON)
	#warning "WATCHDOG enabled"
#else
	#warning "WATCHDOG disabled"
#endif

  	//系统初始化
	System_init();

	//任务初始化
    LPSA_Init();
	
	//建立自检任务
	LPSA_TaskCreate(&task_test);
	
	//建立记步任务
	LPSA_TaskCreate(&task_tracker);
	
	//建立睡眠任务
	LPSA_TaskCreate(&task_sleep);
	
	//建立关机任务
	LPSA_TaskCreate(&task_stop);
	
	//建立USB任务
	LPSA_TaskCreate(&task_usb);
	
	//建立深度睡眠任务
	LPSA_TaskCreate(&task_deepSleep);
	
	//启动任务
    LPSA_TaskStat();
}
