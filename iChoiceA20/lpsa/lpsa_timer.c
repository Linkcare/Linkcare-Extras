#include "lpsa.h"

#ifdef CPU_16

#define _disint()  __disable_interrupt()
#define _enint()  __enable_interrupt()

#endif

#if LPSA_TIMER_TASK_ENABLE

LPSA_TIMER_TASK	LPSATimer;


/*****************************************************************************
*                             初始化定时器任务
* 输入：NULL;
*
* 输出：NULL;
*****************************************************************************/
void LPSA_TimerInit (void)
{
	lpsa_u8 i;
	LPSATimer.TaskCount = 0x00;
	LPSATimer.TimerError = 0x00;

	for (i = 0; i < LPSA_TIMER_TASK_MAX; i ++)
	{
		LPSATimer.TimeTask[i] = NULL;
		LPSATimer.Timer[i] = NULL;
		LPSATimer.TimerCount[i] = NULL;
	}
}	
/*****************************************************************************
*                             创建一个定时器任务
*
* 输入：void (*TimerTask)(void);--创建的任务指针
*       lpsa_u32 Ms;--延时时间，单位毫秒
*
* 输出： 0;--成功
        -1;--时间错误
        -2;--任务为空
        -3;--超出任务规定最大值
*****************************************************************************/
lpsa_i8 LPSA_TimerTaskCreate (void (*TimerTask)(void), lpsa_u32 Ms)
{
        Ms /= LPSA_INT_TIME;
	if (Ms == 0)
	{
		LPSATimer.TimerError += 1;
		return -1;
	}
	if (TimerTask == NULL)
	{
		LPSATimer.TimerError += 1;
		return -2;
	}
	LPSATimer.Timer[LPSATimer.TaskCount] = Ms;
	LPSATimer.TimeTask[LPSATimer.TaskCount] = TimerTask;
	LPSATimer.TaskCount += 1;
	if (LPSATimer.TaskCount > (LPSA_TIMER_TASK_MAX-1))
	{
		LPSATimer.TimerError += 1;
		return -3;
	}
	return 0;
}
/*****************************************************************************
*                             删除一个定时器任务
*
* 输入：void (*TimerTask)(void);--需要删除的任务指针
*
* 输出：NULL;
*****************************************************************************/	
void LPSA_TimerTaskDel (void (*TimerTask)(void))
{
        lpsa_u8 i = 0;
        for (i = 0; i < LPSA_TIMER_TASK_MAX; i ++)
        {
          if (LPSATimer.TimeTask[i] == TimerTask)
          {
            LPSATimer.TimeTask[i] = NULL;
            return;
          }
        }
}

/*****************************************************************************
*                             定时任务切换函数
* 输入：NULL;
*
* 输出：NULL;
*
*此函数需要放到定时器中
*****************************************************************************/
void LPSA_TimerRun (void)
{
	lpsa_u8	i;

	for (i = 0; i < LPSATimer.TaskCount; i ++)
	{
                if (LPSATimer.Timer[i] != 0)
                {
#ifdef CPU_16
		  _disint();
#endif
		  						LPSATimer.TimerCount[i] += 1;
#ifdef CPU_16
		  _enint();
#endif
                }
     if (LPSATimer.TimerCount[i] > LPSATimer.Timer[i] - 1)
			{
				LPSATimer.TimerCount[i] = 0;
				if (LPSATimer.TimeTask[i] != NULL)
					{
						LPSATimer.TimeTask[i]();
						}
			}
	}
	
}

#endif