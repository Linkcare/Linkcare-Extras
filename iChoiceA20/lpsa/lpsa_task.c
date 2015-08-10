#include "lpsa.h"
#include <intrinsics.h>

LPSA_CREATE_TASK	LPSATask;

/*****************************************************************************
*                        初始化多任务数据结构
* 输入：NULL;
*
* 输出：NULL;
*****************************************************************************/
void LPSA_TaskInit (void)
{
	lpsa_u8 i;
	LPSATask.TaskId = 0x00;
	LPSATask.TaskCount = 0x00;
	LPSATask.TaskError = 0x0000;

	for (i = 0; i < LPSA_CREATE_TASK_MAX; i ++)
		LPSATask.CreateTask[i] = NULL;
}

/*****************************************************************************
*                        创建一个消息队列
* 输入：void task (void *prg);--需要创建的任务
*
* 输出：id;--返回当前任务的id
*       -1;--输入为空
*       -2;--超出任务最大范围
*****************************************************************************/
lpsa_i8	LPSA_TaskCreate (void (*Task)(void *))
{
	lpsa_i8 id;
	if (Task == NULL)
	{
		LPSATask.TaskError += 1;
		return -1;
	}
	id = LPSATask.TaskCount;
	LPSATask.CreateTask[LPSATask.TaskCount] = Task;
	LPSATask.TaskCount += 1;
	if (LPSATask.TaskCount > (LPSA_CREATE_TASK_MAX - 1))
	{
		LPSATask.TaskError += 1;
		return -2;
	}
	return id;
}

#ifdef  ASM
#else
/*****************************************************************************
*                                 任务切换
* 输入：lpsa_u8 Id;--任务id
*
* 输出：NULL;
*****************************************************************************/
void LPSA_TaskSwitch (lpsa_u8 Id)
{
  	__disable_interrupt();
  
	if (Id > (LPSA_CREATE_TASK_MAX - 1))
		return;
	LPSATask.TaskId = Id;
	
	__enable_interrupt();
}
/*****************************************************************************
*                             切换到下一任务
* 输入：NULL;
*
* 输出：NULL;
*****************************************************************************/
void LPSA_TaskSwitchDown (void)
{
	if (LPSATask.TaskId < (LPSA_CREATE_TASK_MAX - 1))
		LPSATask.TaskId += 1;
}
/*****************************************************************************
*                             切换到上一任务
* 输入：NULL;
*
* 输出：NULL;
*****************************************************************************/
void LPSA_TaskSwitchUp (void)
{
	if (LPSATask.TaskId > 0)
		LPSATask.TaskId -= 1;
}

#endif