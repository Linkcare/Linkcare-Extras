// Queue.cpp : Defines the entry point for the console application.
//
#include "lpsa.h"




/*****************************************************************************
*                        ����һ����Ϣ����
* ���룺Q_EVENT   Q1;--һ����Ϣ����
*       lpsa_8    Msg[10];--��Ϣ���о�̬����
        lpsa_i16  sizeof (Msg);--��Ϣ���г���
* �����NULL;
*****************************************************************************/
void QueueCreate (Q_EVENT *Queue, lpsa_u8 *Msg, lpsa_i16 Size)
{
	if (Queue == NULL || Msg == NULL)
		return;

	Queue->End = &Msg[Size - 1];
	Queue->Start = &Msg[0];
        Queue->WritePoint = Queue->Start;
        Queue->ReadPoint = Queue->Start;
        Queue->Count_w = 0;
        Queue->Count_r = 0;
}

/*****************************************************************************
*                        ��һ����Ϣ������дһ����Ϣ
* ���룺Q_EVENT   Q1;--һ����Ϣ���нṹ
*       lpsa_8    data;--д����Ϣ���е�����
* �����NULL;
*****************************************************************************/
void QueuePlpsat (Q_EVENT *Queue, lpsa_u8 Data)
{
  
	if (Queue == NULL)
          return;
         
        if (Queue->WritePoint > Queue->End)
          Queue->WritePoint = Queue->Start;
        
        Queue->Count_w ++;

	*(Queue->WritePoint ++) = Data;
          
}

/*****************************************************************************
*                       ��һ����Ϣ�����ж�ȡһ����Ϣ
* ���룺Q_EVENT   Q1;--һ����Ϣ���нṹ
*
* �����lpsa_u8   data;--����һ��lpsa_u8����
*****************************************************************************/
lpsa_u8 QueueGet (Q_EVENT *Queue)
{
	lpsa_u8 Data = 0;
	

	return Data;
}

/*****************************************************************************
*                        ��һ����Ϣ���ж�ȡһ����Ϣ
* ���룺Q_EVENT   Q1;--һ����Ϣ���нṹ
*
* �����lpsa_u8 *data;--����һ������ָ�룬�������������Ϊ�գ�����null;
*****************************************************************************/
lpsa_u8 *QueueAcc (Q_EVENT *Queue)
{
	static lpsa_u8 Data = 0;	
        
        if (Queue == NULL)
          return NULL;
        
        if (Queue->Count_r < Queue->Count_w)
        {
          Queue->Count_r ++;
          
          if (Queue->ReadPoint > Queue->End)
            Queue->ReadPoint = Queue->Start;
          
          Data = *(Queue->ReadPoint ++);
        }
        else
        {
          Queue->Count_r = 0;
          Queue->Count_w = 0;
          Queue->WritePoint = Queue->Start;
          Queue->ReadPoint = Queue->Start;
          return NULL;
        }

        
	return &Data;
}



