// ----------------------------------------------
// 1.文件说明：与USB总线电源信号VBUS检测相关的函数
// ----------------------------------------------

// ----------------------------------------------
// 2.头文件
// ----------------------------------------------
#include "stm32l1xx_conf.h"
#include "usb_lib.h"
#include "usb_prop.h"
#include "hw_config.h"
#include "Event.h"
#include "Task.h"
#include "vbus.h"
#include "key.h"

// 1. 接口层

// ----------------------------------------------
// 常量定义
// ----------------------------------------------
// VBUS对应的端口和位的定义：PA0
#define VBUS_CTRL     RCC_AHBPeriph_GPIOA   // VBUS对应的控制口
#define VBUS_PORT     GPIOA                 // VBUS对应的GPIO端口
#define VBUS_BIT      GPIO_Pin_0            // VBUS对应的GPIO位  


// ----------------------------------------------
// 宏定义
// ----------------------------------------------

// ----------------------------------------------
// 变量定义
// ----------------------------------------------
static uint8 VBusState;                // VBUS状态机

// ----------------------------------------------
// 函数原型
// ----------------------------------------------

// ----------------------------------------------
// 函数实现
// ----------------------------------------------

// 1.接口层的函数实现

// 函数功能：VBUS端口初始化
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
static void VBUS_GPIO_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
  	// 打开VBUS对应的GPIO端口的时钟，允许该端口工作
  	RCC_AHBPeriphClockCmd(VBUS_CTRL, ENABLE);

  	// 初始化VBUS对应的GPIO端口为输入
  	GPIO_InitStructure.GPIO_Pin   = VBUS_BIT;
  	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
  	GPIO_Init(VBUS_PORT, &GPIO_InitStructure);
}

// 函数功能：读入当前VBUS的状态值
// 输入参数：无
// 输出参数：无
// 返 回 值：VBUS_ON  -- 表示VBUS有电
//           VBUS_OFF -- 表示VBUS没电
// 备    注：无
static uint8 VBUS_GPIO_GetValue()
{
  	if(GPIO_ReadInputDataBit(VBUS_PORT,VBUS_BIT) == 0x00)
  	{
    	return VBUS_OFF;
  	}
  	else
  	{
    	return VBUS_ON;
  	}
}

// 2.设备层的函数实现

// 函数功能：VBUS设备的初始化
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
static void VBUS_DEV_Init()
{
  	VBUS_GPIO_Init();                      // 调用接口层函数，完成GPIO初始化
  
  	// 初始化VBUS处理状态机 
  	VBusState = 0;
}

// 函数功能：VBUS信号扫描
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：该函数被定时中断调用，周期是20mS
void VBUS_DEV_Scan()
{
	// 根据VBUS状态机的当前状态，对当前的VBUS输入进行处理
	switch(VBusState)
	{
		case 0:
		  	// 在状态0下，若VBUS有电，则转入状态1，否则保持状态0
		  	if(VBUS_GPIO_GetValue() == VBUS_ON)
		  	{
				VBusState = 1;
		  	}
		  	break;
		case 1:
		  	// 在状态1下，若VBUS还有电，则说明供电稳定，进入状态2
		  	// 否则说明上次有电无效，回到状态0
		  	if(VBUS_GPIO_GetValue() == VBUS_ON)
		  	{
				VBusState = 2;
		  	}
		  	else
		  	{ 
				VBusState = 0;
		  	}
		  	break;
		case 2:
		  	// 在状态2下，等待VBUS无电，若无电，进入状态3
		  	// 否则保持在状态2
		  	if(VBUS_GPIO_GetValue() == VBUS_OFF)
		  	{
				VBusState = 3;
		  	}
		  	break;
		case 3:
		  	// 在状态3下，若VBUS没电，则说明完成了一个有效的供电到断电过程
		  	// 回到状态0
		  	// 否则，说明无电无效，回到状态2
		  	if(VBUS_GPIO_GetValue() == VBUS_OFF)
		  	{
				// 完成了一个有效供电到断电的过程
				VBusState = 0;
		  	}
		  	else
		  	{ 
				// 如果VBUS断电无效，返回状态2
				VBusState = 2;
		  	}
		  	break;
	}
}

// 3.应用层

// 函数功能：初始化VBUS检测模块
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void VBUS_API_Init()
{
  	VBUS_DEV_Init();                       // 调用设备层初始化
}

// 函数功能：检查当前状态机
// 输入参数：无
// 输出参数：无
// 返 回 值：VBUS_ON    -- VBUS有电
//           VBUS_OFF   -- VBUS没电
// 备    注：无
uint32 VBUS_API_GetVBus()
{
  	uint32 temp;

  	// 禁止中断
  	__disable_irq();
  
  	temp = VBusState;

  	// 允许中断
  	__enable_irq();

  	if((temp == 2) || (temp == 3))
  	{
    	// VBUS状态机处于状态2或3，说明已经处于稳定的供电状态
    	return USB_ON;
  	}
  	else
  	{
    	// 否则VBUS状态机处于不稳定的供电状态
    	return USB_OFF;
  	}
}

/****************************added by liubingyan*******************************/

// 函数功能：使能VBUS ON 检测中断
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void VBUS_enableVBUSON(void)
{
/************************configure pin***************************/    
 	GPIO_InitTypeDef  GPIO_InitStructure;
  
  	RCC_AHBPeriphClockCmd(VBUS_CTRL, ENABLE);

  	GPIO_InitStructure.GPIO_Pin   	= VBUS_BIT;
  	GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_400KHz;
  	GPIO_Init(VBUS_PORT, &GPIO_InitStructure);
 
/**************configure interrupt function**********************/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	EXTI_InitTypeDef extiInit;
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	  
	extiInit.EXTI_Line    	= EXTI_Line0;
	extiInit.EXTI_Mode    	= EXTI_Mode_Interrupt;
	extiInit.EXTI_Trigger 	= EXTI_Trigger_Rising;
	extiInit.EXTI_LineCmd 	= ENABLE;                           
	EXTI_Init(&extiInit);

	NVIC_InitTypeDef nvicInit;
	nvicInit.NVIC_IRQChannel                   	= EXTI0_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority 	= PRI_MCU_VBUS;
	nvicInit.NVIC_IRQChannelSubPriority        	= 0;
	nvicInit.NVIC_IRQChannelCmd                	= ENABLE;
	NVIC_Init(&nvicInit);
}

// 函数功能：禁止VBUS ON 检测中断
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void VBUS_disableVBUSON(void)
{
	EXTI_InitTypeDef extiInit;
  
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	  
	extiInit.EXTI_Line    = EXTI_Line0;          //选择外部中断0
	extiInit.EXTI_Mode    = EXTI_Mode_Interrupt; //中断模式
	extiInit.EXTI_Trigger = EXTI_Trigger_Rising; //选择的是上升沿
	extiInit.EXTI_LineCmd = DISABLE;                           
	EXTI_Init(&extiInit);

	NVIC_InitTypeDef nvicInit;
	nvicInit.NVIC_IRQChannel                   = EXTI0_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority = PRI_MCU_VBUS;
	nvicInit.NVIC_IRQChannelSubPriority        = 0;
	nvicInit.NVIC_IRQChannelCmd                = DISABLE;
	NVIC_Init(&nvicInit);
}

/******************************************************************/
/*使能USB_D- 和USB_D+ 数据线									  */
/******************************************************************/
void VBUS_enableCable(void)
{
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitTypeDef gpioInit;

	//关闭GPIOA
	gpioInit.GPIO_Pin 	= GPIO_Pin_11 | GPIO_Pin_12;
	gpioInit.GPIO_Speed = GPIO_Speed_400KHz;
	gpioInit.GPIO_Mode 	= GPIO_Mode_IN;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &gpioInit);
}

// 函数功能：VBUS中断处理函数
// 输入参数：无
// 输出参数：无
// 函数返回：无
// 备    注：该函数仅在系统进入停机状态时才会启动，用于唤醒CPU
void EXTI0_IRQHandler()
{
  	if(Event_getCurrentMode() == TASK_STOP && Event_getEvent() == NO_EVENT)
	{
		Event_register(USB_VBUSON);
	}

  	// 清除外部中断0的挂起寄存器中的中断请求标志
  	EXTI_ClearFlag(EXTI_Line0);
	EXTI_ClearITPendingBit(EXTI_Line0);
}