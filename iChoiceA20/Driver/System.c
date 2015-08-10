#include "type.h"
#include "stdlib.h"
#include "stm32l1xx.h"

#include "RTC.h"
#include "BLE.h"
#include "vbus.h"
#include "Clock.h"
#include "key.h"
#include "Battery.h"
#include "EEPROM.h"

#include "Upload.h"
#include "DataSave.h"
#include "ACCEL.h"
#include "ACCEL-SLEEP.h"
#include "Calculate.h"
#include "Configuration.h"

#include "Event.h"
#include "Task.h"
#include "System.h"
#include "DeviceInfo.h"

/**********************************************/
//A20设备信息
const uint8 deviceType  			= A20;					//A20设备类型号
const uint8 version_issued[13] 		= FIRMWARE_ISSUED;		//A20软件发布版本号
const uint8 version_developer[13] 	= FIRMWARE_DEVELOPER;	//A20软件开发版本号
/***********************************************
* 功能：开平方运算
* 参数：a - 被开方数
* 返回：结果
***********************************************/
uint16 System_sqrt(uint32 a)
{ 
	uint16 s=0;

 	for(uint16 i=0x8000;i>0;i>>=1)
 	{
  		s|=i;
		
  		if(s * s > a)
		{
   			s &= ~i;
		}
 	}
 	return s;
}

// 函数功能：延时
// 输入参数：延时的毫秒数
// 输出参数：无
// 返 回 值：无
// 备    注：无
#pragma optimize = none
void System_delay(uint16 ms)
{
  	if(ms > 60000)
	  	return;
	
	for(uint16 i = 0;i < ms;i++)
	{
		for(uint16 j = 0;j < 500;j++)
		{
		  	__NOP();
		}
	}
}

// 函数功能：喂狗
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void System_updateWatchDog(void)
{
    IWDG_ReloadCounter();//重载看门狗计数器-喂狗
}

// 函数功能：看门狗初始化
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：正常模式
void initWatchDog(void)
{
	// 1.允许访问看门狗，解锁
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	// 2.设置看门狗的预分频器，为256，相当于看门狗计数器的时钟为37000Hz/256=144.5HZ
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	// 3.设置看门狗计数器的计数值为1500，相当于看门狗溢出时间为：1500/144.5=10.38秒
	IWDG_SetReload(1500);
	// 4.重载看门狗计数器，也就是相当于清除看门狗
	IWDG_ReloadCounter();
	// 5.启动看门狗
	IWDG_Enable();
}
/***********************************************
* 功能：关闭所有I/O
***********************************************/
void System_disablePin(void)
{
	//打开GPIO端口的时钟，允许该端口工作
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, ENABLE);

	GPIO_InitTypeDef gpioInit;

	//关闭GPIOA
	gpioInit.GPIO_Pin 	= GPIO_Pin_0
						| GPIO_Pin_2
						| GPIO_Pin_3
						| GPIO_Pin_4
						| GPIO_Pin_5
						| GPIO_Pin_6
						| GPIO_Pin_7
						| GPIO_Pin_8
						| GPIO_Pin_9
						| GPIO_Pin_10
						| GPIO_Pin_11
						| GPIO_Pin_12				  
						| GPIO_Pin_15;

	gpioInit.GPIO_Speed = GPIO_Speed_400KHz;
	gpioInit.GPIO_Mode 	= GPIO_Mode_OUT;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &gpioInit);
	  
	GPIO_WriteBit(GPIOA,GPIO_Pin_0, Bit_RESET);     // USB_AD = 0
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_2, Bit_RESET);     // BMA_CS = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_3, Bit_RESET);     // BMA_INT1 = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_4, Bit_RESET);     // BMA_INT2 = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_5, Bit_SET);     	// BMA_SPICLK = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_6, Bit_SET);     	// BMA_SPIMISO = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_7, Bit_SET);       // BMA_SPIMOSI = 0 
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_8, Bit_RESET);     // EERPOM_S = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_9, Bit_RESET);     // CHARGE_STATUS = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_10,Bit_SET);       // BLE_ON = 1
	GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET);     // USB_D- = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_12,Bit_RESET);     // USB_D+ = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);     // BLE_WAKE = 0

	//关闭GPIOB
	gpioInit.GPIO_Pin 	= GPIO_Pin_0
						| GPIO_Pin_1
						| GPIO_Pin_2
						| GPIO_Pin_3
						| GPIO_Pin_4
						| GPIO_Pin_5
						| GPIO_Pin_6
						| GPIO_Pin_7
						| GPIO_Pin_8
						| GPIO_Pin_9
						| GPIO_Pin_10
						| GPIO_Pin_11
						| GPIO_Pin_12
						| GPIO_Pin_13
						| GPIO_Pin_14
						| GPIO_Pin_15;

	gpioInit.GPIO_Speed = GPIO_Speed_400KHz;
	gpioInit.GPIO_Mode 	= GPIO_Mode_OUT;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &gpioInit);
	  
	GPIO_WriteBit(GPIOB,GPIO_Pin_0, Bit_RESET);     // BMP_SPIMISO = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_1, Bit_RESET);     // BMP_SPIMOSI = 0  
	GPIO_WriteBit(GPIOB,GPIO_Pin_2, Bit_RESET);     // BMP_CS = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);     // BMP_SPICLK = 0 
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_3, Bit_RESET);     // BLE_STATUS0 = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_4, Bit_RESET);     // BLE_STATUS1 = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_5, Bit_RESET);     // BLE_CTL = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_6, Bit_RESET);     // BLE_RX = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_7, Bit_RESET);     // BLE_TX = 0
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_8, Bit_RESET);     // OLED_RES = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_9, Bit_SET);       // OLED_ON = 1
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);     // OLED_SCL = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_RESET);     // OLED_SDA = 0
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);     // EEPROM_C = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);     // EEPROM_Q = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);     // EEPROM_D = 0
	
	//关闭GPIOH
	gpioInit.GPIO_Pin 	= GPIO_Pin_0 | GPIO_Pin_1;
	gpioInit.GPIO_Speed = GPIO_Speed_400KHz;
	gpioInit.GPIO_Mode 	= GPIO_Mode_OUT;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOH, &gpioInit);
	  
	GPIO_WriteBit(GPIOH,GPIO_Pin_0,Bit_SET);       // PD0 = 0
	GPIO_WriteBit(GPIOH,GPIO_Pin_1,Bit_SET);       // PD1 = 0
}
/***********************************************
*功能:系统初始化
***********************************************/
void System_init(void)
{
    __disable_interrupt();
	
	/***********驱动层初始化***************/	
	
	//时钟初始化
	Clock_init();
	
	//禁止PVD  
  	PWR_PVDCmd(DISABLE);
  
  	//禁止温度传感器参考电源
  	ADC_TempSensorVrefintCmd(DISABLE);
    
  	//允许超低功耗命令
  	PWR_UltraLowPowerCmd(ENABLE);
	PWR_FastWakeUpCmd(ENABLE);
	
	//中断优先级设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	/*设置管理模块初始化*/
    UserConfig_init();
	
	/*RTC初始化*/
	RTC_init();	
	
	/*KEY初始化*/
	KEY_API_Init();
	
	/*存储初始化*/
	DataSave_init();
	
	/*上传初始化*/
	Upload_init();
	
	/*数据计算模块初始化*/
    Calculate_init();
	
	/*记步算法初始化*/
	Init_ACCEL();
	
	/*备份初始化*/
#if	(BACKUP == ON)
	
	/*RTC初始化*/
	RTC_recover();
	
	/*存储初始化*/
	DataSave_recover();
	
	/*数据计算模块初始化*/
	Calculate_recover();
	
	/*记步算法初始化*/
	Init_ACCEL_recover();	
#endif
	
	/*睡眠算法初始化*/
	Init_SLEEP();
	
	//看门狗初始化
#if	(WATCHDOG == ON)
	
	/*看门狗初始化*/
	initWatchDog();
#endif
    __enable_interrupt();
	
}