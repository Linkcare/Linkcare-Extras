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
    uint8* buf;//队列缓冲区
    uint16 size;//队列缓冲区大小
    uint8* front;//头指针
    uint8* rear;//尾指针
    uint8 elementSize;//用户元素的大小
    uint8 nodeSize;//每个节点的大小,字节对齐
    uint16 num;//当前元素的数量
    int8 isQueueValid;//队列有效标志
}ArrayQueue;

/***********************************************
* 功能：创建一个队列(为循环队列,用数组实现)
* 参数：elementSize - 元素的大小
*       buffer - 队列的缓冲区
*       bufferSize - 队列缓冲区大小
* 返回：创建成功返回一个队列，失败返回空指针
***********************************************/
ArrayQueue* ArrayQueue_construct(ArrayQueue* q);

/***********************************************
* 功能：在一个队列的尾部添加一个元素
* 参数：element - 目标元素
*       queue - 目标队列
* 返回：QUEUE_INVALID - 队列无效
*       QUEUE_FULL - 队列已满
*       QUEUE_SUCCESS - 添加成功
***********************************************/
int8 ArrayQueue_add(void* element,ArrayQueue* queue);

#endif

/***********************************************
* 功能：获取并删除一个队列的首元素
* 参数：element - 从队列获取元素的目标地址
*       queue - 目标队列
* 返回：QUEUE_INVALID - 队列无效
*       QUEUE_EMPTY - 队列为空
***********************************************/
int8 ArrayQueue_poll(void* element,ArrayQueue* queue);

/***********************************************
* 功能：获取一个队列的首元素，但不删除
* 参数：element - 从队列获取元素的目标地址
*       queue - 目标队列
* 返回：QUEUE_INVALID - 队列无效
*       QUEUE_EMPTY - 队列为空
***********************************************/
int8 ArrayQueue_peek(void* element,ArrayQueue* queue);

/***********************************************
* 功能：删除一个队列中的所有元素
* 参数：queue - 要删除元素的队列
* 返回：QUEUE_INVALID - 队列无效
*       QUEUE_SUCCESS - 删除成功
***********************************************/
int8 ArrayQueue_clear(ArrayQueue* queue);

/***********************************************
* 功能：删除一个队列
* 参数：queue - 要删除的队列
* 返回：QUEUE_INVALID - 队列无效
*       QUEUE_SUCCESS - 删除成功
***********************************************/
int8 ArrayQueue_delete(ArrayQueue* queue);

/***********************************************
* 功能：获得一个队列中元素的数量
* 参数：queue - 队列
* 返回：元素数量
***********************************************/
uint16 ArrayQueue_size(ArrayQueue* queue);