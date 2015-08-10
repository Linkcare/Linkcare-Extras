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

  	//ϵͳ��ʼ��
	System_init();

	//�����ʼ��
    LPSA_Init();
	
	//�����Լ�����
	LPSA_TaskCreate(&task_test);
	
	//�����ǲ�����
	LPSA_TaskCreate(&task_tracker);
	
	//����˯������
	LPSA_TaskCreate(&task_sleep);
	
	//�����ػ�����
	LPSA_TaskCreate(&task_stop);
	
	//����USB����
	LPSA_TaskCreate(&task_usb);
	
	//�������˯������
	LPSA_TaskCreate(&task_deepSleep);
	
	//��������
    LPSA_TaskStat();
}
