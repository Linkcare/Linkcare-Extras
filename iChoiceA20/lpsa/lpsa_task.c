#include "lpsa.h"
#include <intrinsics.h>

LPSA_CREATE_TASK	LPSATask;

/*****************************************************************************
*                        ��ʼ�����������ݽṹ
* ���룺NULL;
*
* �����NULL;
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
*                        ����һ����Ϣ����
* ���룺void task (void *prg);--��Ҫ����������
*
* �����id;--���ص�ǰ�����id
*       -1;--����Ϊ��
*       -2;--�����������Χ
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
*                                 �����л�
* ���룺lpsa_u8 Id;--����id
*
* �����NULL;
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
*                             �л�����һ����
* ���룺NULL;
*
* �����NULL;
*****************************************************************************/
void LPSA_TaskSwitchDown (void)
{
	if (LPSATask.TaskId < (LPSA_CREATE_TASK_MAX - 1))
		LPSATask.TaskId += 1;
}
/*****************************************************************************
*                             �л�����һ����
* ���룺NULL;
*
* �����NULL;
*****************************************************************************/
void LPSA_TaskSwitchUp (void)
{
	if (LPSATask.TaskId > 0)
		LPSATask.TaskId -= 1;
}

#endif