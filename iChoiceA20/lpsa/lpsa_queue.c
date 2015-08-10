// Queue.cpp : Defines the entry point for the console application.
//
#include "lpsa.h"




/*****************************************************************************
*                        创建一个消息队列
* 输入：Q_EVENT   Q1;--一个消息队列
*       lpsa_8    Msg[10];--消息队列静态数组
        lpsa_i16  sizeof (Msg);--消息队列长度
* 输出：NULL;
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
*                        向一个消息队列中写一则消息
* 输入：Q_EVENT   Q1;--一个消息队列结构
*       lpsa_8    data;--写入消息队列的数据
* 输出：NULL;
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
*                       从一个消息队列中读取一则消息
* 输入：Q_EVENT   Q1;--一个消息队列结构
*
* 输出：lpsa_u8   data;--返回一个lpsa_u8数据
*****************************************************************************/
lpsa_u8 QueueGet (Q_EVENT *Queue)
{
	lpsa_u8 Data = 0;
	

	return Data;
}

/*****************************************************************************
*                        从一个消息队列读取一则消息
* 输入：Q_EVENT   Q1;--一个消息队列结构
*
* 输出：lpsa_u8 *data;--返回一个数据指针，如果队列中数据为空，返回null;
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



