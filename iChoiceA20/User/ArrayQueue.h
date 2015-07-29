#ifndef __ARRAYQUEUE_H
#define __ARRAYQUEUE_H

#include "type.h"

#define QUEUE_SUCCESS 0
#define QUEUE_FULL -1
#define QUEUE_EMPTY -2
#define QUEUE_INVALID -3
#define NO_SPASE -4

typedef struct
{
    uint8* buf;//���л�����
    uint16 size;//���л�������С
    uint8* front;//ͷָ��
    uint8* rear;//βָ��
    uint8 elementSize;//�û�Ԫ�صĴ�С
    uint8 nodeSize;//ÿ���ڵ�Ĵ�С,�ֽڶ���
    uint16 num;//��ǰԪ�ص�����
    int8 isQueueValid;//������Ч��־
}ArrayQueue;

/***********************************************
* ���ܣ�����һ������(Ϊѭ������,������ʵ��)
* ������elementSize - Ԫ�صĴ�С
*       buffer - ���еĻ�����
*       bufferSize - ���л�������С
* ���أ������ɹ�����һ�����У�ʧ�ܷ��ؿ�ָ��
***********************************************/
ArrayQueue* ArrayQueue_construct(ArrayQueue* q);

/***********************************************
* ���ܣ���һ�����е�β�����һ��Ԫ��
* ������element - Ŀ��Ԫ��
*       queue - Ŀ�����
* ���أ�QUEUE_INVALID - ������Ч
*       QUEUE_FULL - ��������
*       QUEUE_SUCCESS - ��ӳɹ�
***********************************************/
int8 ArrayQueue_add(void* element,ArrayQueue* queue);

#endif

/***********************************************
* ���ܣ���ȡ��ɾ��һ�����е���Ԫ��
* ������element - �Ӷ��л�ȡԪ�ص�Ŀ���ַ
*       queue - Ŀ�����
* ���أ�QUEUE_INVALID - ������Ч
*       QUEUE_EMPTY - ����Ϊ��
***********************************************/
int8 ArrayQueue_poll(void* element,ArrayQueue* queue);

/***********************************************
* ���ܣ���ȡһ�����е���Ԫ�أ�����ɾ��
* ������element - �Ӷ��л�ȡԪ�ص�Ŀ���ַ
*       queue - Ŀ�����
* ���أ�QUEUE_INVALID - ������Ч
*       QUEUE_EMPTY - ����Ϊ��
***********************************************/
int8 ArrayQueue_peek(void* element,ArrayQueue* queue);

/***********************************************
* ���ܣ�ɾ��һ�������е�����Ԫ��
* ������queue - Ҫɾ��Ԫ�صĶ���
* ���أ�QUEUE_INVALID - ������Ч
*       QUEUE_SUCCESS - ɾ���ɹ�
***********************************************/
int8 ArrayQueue_clear(ArrayQueue* queue);

/***********************************************
* ���ܣ�ɾ��һ������
* ������queue - Ҫɾ���Ķ���
* ���أ�QUEUE_INVALID - ������Ч
*       QUEUE_SUCCESS - ɾ���ɹ�
***********************************************/
int8 ArrayQueue_delete(ArrayQueue* queue);

/***********************************************
* ���ܣ����һ��������Ԫ�ص�����
* ������queue - ����
* ���أ�Ԫ������
***********************************************/
uint16 ArrayQueue_size(ArrayQueue* queue);