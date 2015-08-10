#include "task_tracker.h"

static uint8  keyType        = 0;
static uint8  dis_index      = 0;
static uint64 time_to_refresh = 0;

void task_test(void *prg)
{
  	(void)prg;
	
  	__disable_interrupt();
	
	System_disablePin();
	
	/*时钟初始化*/
	Clock_init();
	
	/*关闭IO*/
	System_disablePin();
	
	/*蓝牙初始化*/
	BLE_closePort();
	
	/*关闭EEPROM*/
	EEPROM_closeSPI();
	
	/*VBUS检测*/
	VBUS_API_Init();
	
	/*有关电池的操作*/
	Battery_disable();
	Battery_closeChargePort();
	
	/*有关计步的初始化*/
	LIS3DH_openSPI();
	LIS3DH_init();
	LIS3DH_FIFO_mode();
	LIS3DH_closeSPI();

	/*环境测量*/
	LPS331AP_openSPI();
	LPS331AP_init();
	LPS331AP_closeSPI();
	
	/*20ms定时器初始化*/
	RTC_enableRTCWakeup();
	
	/*睡眠算法初始化*/
	Init_SLEEP();
	
	Event_registerMode(TASK_TRACKER);
	
	/*屏幕相关初始化*/
	Graphics_init();
		
	/*清除EXTI LINE中断请求*/
	EXTI_ClearITPendingBit(EXTI_Line0);
	EXTI_ClearITPendingBit(EXTI_Line3);
	EXTI_ClearITPendingBit(EXTI_Line13);
	
	__enable_interrupt();
	/************初始化MCU********************/  
	while(1)
	{	
	  	if(UserConfig_getPowerOnState() == FIRST_POWERON)
		{	
			/**************************************************/
			#if (A20_DEBUG == OFF)
			PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
			#endif
			/*************************************************/
			if(LIS3DH_getDataStatus() == DATA_IS_READY)
			{
				LIS3DH_feed();
				
				/*启动海拔和温度测量*/
				LPS331AP_open();

				/*计算步数*/
				for(uint8 i = 0;i < FIFO_LENGTH;i++)
				{
					Data_ACCEL = LIS3DSH_getAccData(i);
				}
				
				/*获取海拔和温度数据*/
				LPS331AP_readData();
				
				/*数据计算*/
				DataCalculate();
			}
			/*************************************************/
			//LIS3DH 纠错处理
			if(RTC_checkTimeout(LIS3DH_getRunTimer(),LIS3DH_MTBF) == IS_TIMEOUT)
			{
				LIS3DH_openSPI();
				LIS3DH_init();
				LIS3DH_FIFO_mode();
				LIS3DH_closeSPI();
				
				LIS3DH_feed();
			}
			
			/*************************************************/
			if(RTC_checkTimeout(time_to_refresh,SCREEN_REFRESH) == IS_TIMEOUT)
			{
				RTC_obtainMilliSeconds(&time_to_refresh);
				
				if(Graphics_isDisplayOn() == DISPLAY_ON)
				{
					/*更新屏幕*/
					test_disp[dis_index]();

					Graphics_repaint();
				}
			}
			
			/**************************************************/
			//按键处理
			if(KEY_API_CheckKey() == 1)
			{
				keyType = KEY_API_GetKey();
			}
			else
			{
				keyType = NO_KEY;
			}
			
			if(Graphics_isDisplayOn() == DISPLAY_ON)
			{	
				/*按键处理*/
				switch(keyType)
				{  
					case NO_KEY:
					{
						break;
					}
					case SHORT_KEY:
					{
						dis_index ++;
						dis_index %= TEST_DISPLAY_MAX;
						
						Graphics_clearBuffer();

						break;
					}
					case LONG_KEY:
					{
					  	//将上电标志位置位
					  	UserConfig_setPowerOnState();
					  
						/*任务切换---->tracker模式*/
						LPSA_TaskSwitch(TASK_TRACKER);
						
						return;
					//*************************************************
					}
					default:break;			
				}
			}
		}
		else
		{
			/*任务切换---->tracker模式*/
			LPSA_TaskSwitch(TASK_TRACKER);
			
			return;
		}
  	}
}