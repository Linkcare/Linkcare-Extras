#include "lpsa.h"

#ifdef CPU_16

#define _disint()  __disable_interrupt()
#define _enint()  __enable_interrupt()

#endif

#if LPSA_TIMER_TASK_ENABLE

LPSA_TIMER_TASK	LPSATimer;


/*****************************************************************************
*                             ��ʼ����ʱ������
* ���룺NULL;
*
* �����NULL;
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
*                             ����һ����ʱ������
*
* ���룺void (*TimerTask)(void);--����������ָ��
*       lpsa_u32 Ms;--��ʱʱ�䣬��λ����
*
* ����� 0;--�ɹ�
        -1;--ʱ�����
        -2;--����Ϊ��
        -3;--��������涨���ֵ
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
*                             ɾ��һ����ʱ������
*
* ���룺void (*TimerTask)(void);--��Ҫɾ��������ָ��
*
* �����NULL;
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
*                             ��ʱ�����л�����
* ���룺NULL;
*
* �����NULL;
*
*�˺�����Ҫ�ŵ���ʱ����
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