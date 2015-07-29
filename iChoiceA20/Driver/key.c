// ----------------------------------------------
// 1.文件说明：按键模块的实现
// ----------------------------------------------

// ----------------------------------------------
// 2.头文件
// ----------------------------------------------
#include "stm32l1xx.h"
#include "string.h"
#include "Event.h"
#include "Task.h"
#include "key.h"
#include "DeviceInfo.h"

// 1. 接口层

// 常量定义
// 1个按键的端口和位的定义
#define KEY_PORT     GPIOC             // KEY对应的GPIO端口
#define KEY_BIT      GPIO_Pin_13       // KEY对应的GPIO位  

#define KeyBufSize   16                // 按键缓冲区的大小，必须是2的N次方

#define KEY_PRESS    0
#define KEY_NOPRESS  1


// 宏定义

// 变量定义
uint8 KeyHead,KeyTail;          // 按键缓冲区的头、尾指针
uint8 KeyState;                 // 按键状态机
uint8 KeyCounter;               // 按键按下的计数器
uint8 KeyBuffer[KeyBufSize];    // 按键缓冲区

uint8 KeyFlag;

// 函数原型

// 函数实现

// 1.接口层的函数实现

// 函数功能：按键端口初始化
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
static void KEY_GPIO_Init(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure;
  
  	// 打开按键KEY对应的GPIO端口的时钟，允许该端口工作
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  	// 初始化按键KEY对应的GPIO端口为带上拉的输入
  	GPIO_InitStructure.GPIO_Pin 	= KEY_BIT;
  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_400KHz;
 	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}

// 函数功能：读入当前按键的状态值
// 输入参数：无
// 输出参数：无
// 返 回 值：KEY_PRESS   -- 表示按键按下
//           KEY_NOPRESS -- 表示按键抬起
// 备    注：无
uint8 KEY_GPIO_GetValue(void)
{
  	if(GPIO_ReadInputDataBit(KEY_PORT,KEY_BIT) == 0x00)
  	{
		return KEY_PRESS;
  	}
  	else
  	{
		return KEY_NOPRESS;
 	}
}

// 2.设备层的函数实现

// 函数功能：按键设备的初始化
// 输入参数：无sss
// 输出参数：无
// 返 回 值：无
// 备    注：无
static void KEY_DEV_Init(void)
{
  	KEY_GPIO_Init();                      // 调用接口层函数，完成GPIO初始化
  
  	// 初始化按键处理状态机和时间计数器 
  	KeyState = 0;
}

// 函数功能：保存当前键值到按键缓冲区中
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：若缓冲区满，则丢弃
void KEY_DEV_SaveKey(uint32 KeyValue)
{
  	if(((KeyTail+1) & (KeyBufSize - 1)) != KeyHead)
  	{
    	// 若尾指针+1不等于头指针，则说明缓冲区未满
    	KeyBuffer[KeyTail] = KeyValue;
    	KeyTail = (KeyTail + 1) & (KeyBufSize - 1);
  	}
}

// 函数功能：按键扫描
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：该函数被定时中断调用，周期是20mS
//           这个函数支持长按键
void Key_DEV_ScanKey(void)
{  
	// 根据按键状态机的当前状态，对当前的按键输入进行处理
	switch(KeyState)
	{
		case 0:
			// 在状态0下，若按键按下，则转入状态1，否则保持状态0
			if(KEY_GPIO_GetValue() == KEY_PRESS)
			{
				KeyState = 1;
			}
			break;
		case 1:
			// 在状态1下，若按键还按下，则说明按键有效，进入状态2，并开始计数
			// 否则说明上次按键无效，回到状态0
			if(KEY_GPIO_GetValue() == KEY_PRESS)
			{
				KeyState = 2;
				KeyCounter = 0;                  // 计数器清0，开始计数
			}
			else
			{ 
				KeyState = 0;
			}
			break;
		case 2:
			// 在状态2下，等待按键抬起，若抬起，进入状态3
			// 否则保持在状态2
			if(KEY_GPIO_GetValue() == KEY_PRESS)
			{
				// 若按下，此时要判断是否为长按键
				if(KeyCounter < (LONGKEY_TIMER / RTC_WORK_TIME))
				{
					// 如果计数器小于长按键的计时值，计数器＋1
					KeyCounter++;
				}
				else
				{
					// 如果超过了长按键的计时值，就发出长键消息
					if(KeyFlag == 0)
					{
						KEY_DEV_SaveKey(LONG_KEY);
						
						KeyFlag = 1;
					}
				}
			}
			else
			{
				// 若按键抬起，则进入状态3
				KeyState = 3;
								
				KeyFlag = 0;
			}
			break;
		case 3:
			// 在状态3下，若按键抬起，则说明按键完全抬起，进入状态0
			// 否则说明上次按键抬起无效，回到状态2
			if(KEY_GPIO_GetValue() == KEY_PRESS)
			{
				// 若按键按下，说明前面的抬起为误操作，回到状态2
				KeyState = 2;
			}
			else
			{ 
				// 若按键抬起，则要根据计数器的值，决定是否发出按键消息
				if(KeyCounter < (LONGKEY_TIMER / RTC_WORK_TIME))
				{
					// 若按键计数器未到最大值，则发出短键消息
					KEY_DEV_SaveKey(SHORT_KEY);
				}
				
				// 回到状态0
				KeyState = 0;
			}
			break;
	}
}

// 函数功能：使能按键中断
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void KEY_enableInt(void)
{  	
	/************************configure pin***************************/  
  	GPIO_InitTypeDef  GPIO_InitStructure;
  
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  	// 将端口设置为带上拉的输入
  	GPIO_InitStructure.GPIO_Pin 	= KEY_BIT;
  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_400KHz;
  	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
	
	/**************configure interrupt function**********************/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	EXTI_InitTypeDef extiInit;
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);
	  
	extiInit.EXTI_Line    	= EXTI_Line13;
	extiInit.EXTI_Mode    	= EXTI_Mode_Interrupt;
	extiInit.EXTI_Trigger 	= EXTI_Trigger_Falling;
	extiInit.EXTI_LineCmd 	= ENABLE;
	EXTI_Init(&extiInit);

	NVIC_InitTypeDef nvicInit;
	nvicInit.NVIC_IRQChannel                   	= EXTI15_10_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority 	= PRI_MCU_KEY;
	nvicInit.NVIC_IRQChannelSubPriority        	= 0;
	nvicInit.NVIC_IRQChannelCmd                	= ENABLE;
	NVIC_Init(&nvicInit);
}

// 函数功能：禁止按键中断
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void KEY_disableInt(void)
{
  	EXTI_InitTypeDef extiInit;
  
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);
	  
	extiInit.EXTI_Line    = EXTI_Line13;          
	extiInit.EXTI_Mode    = EXTI_Mode_Interrupt;  
	extiInit.EXTI_Trigger = EXTI_Trigger_Falling; 
	extiInit.EXTI_LineCmd = DISABLE;                           
	EXTI_Init(&extiInit);

	NVIC_InitTypeDef nvicInit;
	nvicInit.NVIC_IRQChannel                   	= EXTI15_10_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority 	= PRI_MCU_KEY;
	nvicInit.NVIC_IRQChannelSubPriority        	= 0;
	nvicInit.NVIC_IRQChannelCmd                	= DISABLE;
	NVIC_Init(&nvicInit);
}

// 3.应用层
// 函数功能：初始化按键模块
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void KEY_API_Init(void)
{
  	KEY_DEV_Init();                       // 调用设备层初始化
  
  	KeyHead = 0;                          // 初始化按键缓冲区的头指针
  	KeyTail = 0;                          // 初始化按键缓冲区的尾指针
	
	for(uint8 i = 0;i < KeyBufSize;i++)
	  	KeyBuffer[i] = 0;
}

// 函数功能：返回按键缓冲区中的当前按键
// 输入参数：无
// 输出参数：无
// 返 回 值：按键缓冲区的按键键值
// 备    注：无
uint32 KEY_API_GetKey(void)
{
	uint32 key_head,key_tail;
	uint32 key;
	  
	while(1)
	{
		// 禁止中断
		__disable_irq();   
		key_head = KeyHead;
		key_tail = KeyTail;
		// 允许中断
		__enable_irq();

		if(key_head != key_tail)
		{
			// 若头指针不等于尾指针，则说明按键缓冲区中存在按键需要处理
			key = KeyBuffer[KeyHead];
			KeyHead = (KeyHead + 1) & (KeyBufSize - 1);
			return key;
		}
	}
}

// 函数功能：检查按键缓冲区中是否有键需要处理
// 输入参数：无
// 输出参数：无
// 返 回 值：1 -- 按键缓冲区中存在按键需要处理
//           0 -- 无
// 备    注：无
uint32 KEY_API_CheckKey(void)
{
	uint32 key_head,key_tail;

	// 禁止中断
	__disable_irq();

	key_head = KeyHead;
	key_tail = KeyTail;

	// 允许中断
	__enable_irq();

	if(key_head != key_tail)
	{
		// 若头指针不等于尾指针，则说明按键缓冲区中存在按键需要处理

		return 1;
	}
	else
	{
	  	return 0;
	}
}

// 函数功能：按键中断处理函数
// 输入参数：无
// 输出参数：无
// 函数返回：无
// 备    注：该函数仅在系统进入停机状态时才会启动，用于唤醒CPU
void EXTI15_10_IRQHandler(void)
{
  	if(Event_getCurrentMode() == TASK_STOP && Event_getEvent() == NO_EVENT)
	{
		Event_register(KEY_PRESSED);
	}

  	EXTI_ClearFlag(EXTI_Line13);
	EXTI_ClearITPendingBit(EXTI_Line13);
}