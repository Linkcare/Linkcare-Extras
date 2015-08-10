#include "Event.h"
#include "Task.h"
#include "stm32l1xx.h"

//系统当前事件
volatile static uint8 system_event = NO_EVENT;

//系统当前工作模式
volatile static uint8 current_task;

//系统上次工作模式
volatile static uint8 former_task;

// 函数功能：获取系统事件
// 输入参数：无
// 输出参数：无
// 返 回 值：事件
// 备    注：无
uint8 Event_getEvent(void)
{
    __disable_interrupt();
    
    uint16 tempEvent = system_event;
	
    system_event = NO_EVENT;
    
    __enable_interrupt();
    
    return tempEvent;
}

// 函数功能：注册系统事件
// 输入参数：待注册事件
// 输出参数：无
// 返 回 值：事件
// 备    注：无
void Event_register(const uint8 event)
{
  	if(system_event == NO_EVENT)
	{
    	system_event = event;
	}
}

// 函数功能：清空系统事件
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Event_clearEvent(void)
{
    system_event = NO_EVENT;
}

// 函数功能：更新系统工作模式
// 输入参数：待更新事件
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Event_registerMode(const uint8 mode)
{
    __disable_interrupt();
	
    if(mode == TASK_USB || current_task == mode)
    {
    	__enable_interrupt();

        return;
    }

    former_task = current_task;
    current_task = mode;

    __enable_interrupt();

    return;
}

// 函数功能：获取系统当前工作模式
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
uint8 Event_getCurrentMode(void)
{
    return current_task;
}

// 函数功能：获取系统上一次工作模式
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
uint8 Event_getLastMode(void)
{
    return former_task;
}

