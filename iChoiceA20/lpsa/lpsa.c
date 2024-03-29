#include "lpsa.h"
/*****************************************************************************
*                               初始化函数
* 输入：NULL;
* 输出：NULL;
*****************************************************************************/
void LPSA_Init (void)
{
	LPSA_TimerInit ();//初始化定时器任务
	LPSA_TaskInit ();//初始化多任务
}

/*****************************************************************************
*                        获取当前系统的内存使用量
* 输入：NULL;
* 输出：16为无符号数，单位BYTE;
*****************************************************************************/
lpsa_u16 LPSA_GetRam (void)
{
	lpsa_u16	rd = 0;
	rd = sizeof (LPSA_TIMER_TASK) + sizeof (LPSA_CREATE_TASK) + sizeof (Q_EVENT) + sizeof (WDT_DATA);
	return rd;
}


#ifdef  ASM
#else
/*****************************************************************************
*                        开始多任务切换
* 输入：NULL;
* 输出：NULL;
*****************************************************************************/
void LPSA_TaskStat (void)
{
	while (1)
		LPSATask.CreateTask[LPSATask.TaskId](LPSATask.parg);
}
#endif