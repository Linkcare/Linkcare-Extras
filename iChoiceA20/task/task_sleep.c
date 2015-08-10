#include "task_tracker.h"
#include "DataSave.h"

static uint64 Timer_ScreenOn = 0;
static uint8  keyType        = 0;

uint8 SleepData = 0;
static uint64 time_to_refresh = 0;

void task_sleep(void *prg)
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
	
	/*有关加速度传感器的初始化*/
	LIS3DH_openSPI();
	LIS3DH_init();
	LIS3DH_FIFO_mode();
	LIS3DH_closeSPI();

	/*关闭气压传感器*/
	LPS331AP_closeSPI();
	
	/*20ms定时器初始化*/
	RTC_enableRTCWakeup();
	
	/*睡眠算法初始化*/
	Init_SLEEP();
	
	Timer_ScreenOn = 0;

	/*更新任务模式*/
	Event_registerMode(TASK_SLEEP);
	
	/*屏幕相关初始化*/
	if(Event_getLastMode() == TASK_STOP)
	{
		/*屏幕初始化*/
		if(lastDeviceOff != 0) //如果本次为非首次开机
		{	
			if(offMode == DEEPLY_OFF)
			{	
				/*如果上次关机为深度关机，打开屏幕*/
				Graphics_init();
			}
		}
		else	//如果本次为首次开机
		{
			/*打开屏幕*/
			Graphics_init();
		}
	}
	else
	{
		/*打开屏幕*/
		Graphics_init();
	}
	
	/*清除EXTI LINE中断请求*/
	EXTI_ClearITPendingBit(EXTI_Line0);
	EXTI_ClearITPendingBit(EXTI_Line3);
	EXTI_ClearITPendingBit(EXTI_Line13);

	__enable_interrupt();
	
	/************初始化MCU********************/  
	while(1)
	{	
	  	/**************************************************/
		//运动数据存储
		DataSave_writeToEEPROM();
		
		/**************************************************/
		#if(A20_DEBUG == OFF)
		//低功耗
		PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
		#endif
		
		/**************************************************/
		//运动数据存储
		DataSave_writeToEEPROM();
		
		/**************************************************/
		Battery_update();
		
		if(Battery_getBatteryLevel() == BATTERY_DANGER)
		{
			Timer_ScreenOn = 0;

			//*任务切换---->关机模式*
			LPSA_TaskSwitch(TASK_STOP);
			
			return;
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
		if(LIS3DH_getDataStatus() == DATA_IS_READY)
		{
		  	LIS3DH_feed();

			for(uint8 i = 0;i < FIFO_LENGTH;i++)
			{
				DateSLEEP = LIS3DSH_getAccData(i);
				main_SLEEP();
				
				if(flag_update == 1)
				{
				  	//缓存睡眠数据
					SleepData =(uint8)Time_Motion[0]; 
				}
			}			
		}
		
		/*************************************************/
		if(RTC_checkTimeout(time_to_refresh,SCREEN_REFRESH) == IS_TIMEOUT)
		{
		  	RTC_obtainMilliSeconds(&time_to_refresh);
			
			if(Graphics_isDisplayOn() == DISPLAY_ON)
			{
				/*更新屏幕*/
				infor_disp[SLEEP]();

				Graphics_repaint();
			}
		}
		
		/**************************************************/		
		if(VBUS_API_GetVBus() == USB_ON)
		{
			Timer_ScreenOn = 0;
			
			Graphics_displayOff(); 
						
			LPSA_TaskSwitch(TASK_USB);
				
			return;
		}
		
		/**************************************************/
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
					if(Timer_ScreenOn == 0)
					{
						RTC_obtainMilliSeconds(&Timer_ScreenOn);
					}
					
					if(RTC_checkTimeout(Timer_ScreenOn,SCREEN_ON) == IS_TIMEOUT)
					{
						Graphics_displayOff();
						
						Timer_ScreenOn = 0;	
					}
					break;
				}
				case SHORT_KEY:
				{
					Timer_ScreenOn = 0;				
					break;
				}
				case LONG_KEY:
				{
					Timer_ScreenOn = 0;
					
					LPSA_TaskSwitch(TASK_TRACKER);
					
					return;
				//*************************************************
				}
				default:break;			
			}
		}
		else
		{
			switch(keyType)
			{ 
				case NO_KEY:
				{
					break;
				}
				case SHORT_KEY:
				case LONG_KEY:
				{
					Graphics_init();
					
					Timer_ScreenOn = 0;
					Timer_ScreenOn = 0;
					
					break;
				}
				default:break;
			}	
		}	
  	}
}