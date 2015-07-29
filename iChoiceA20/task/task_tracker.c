#include "task_tracker.h"
#include "DataSave.h"

static uint64 Timer_NoMotion = 0;
static uint64 Timer_ScreenOn = 0;
static uint8  keyType        = 0;
static uint8  dis_index      = 0;
static uint8  displayCount   = 0;
static uint64 time_to_refresh = 0;
static uint8  displayBuf[FUNC_DISPLAY_MAX];
/**********************************
	蓝牙使用策略：
	只有在TRACKER模式下允许蓝牙通
	信，当短按键时开启蓝牙开关，
	进入其他模式时，强制关闭蓝牙。
**********************************/
void task_tracker(void *prg)
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
	
	/*定制用户界面*/
	UserConfig_getDispOrder(displayBuf);
	
	if(Display_setDisplay(displayBuf,sizeof(displayBuf)) == ERROR)
	{
	  	UserConfig_initDispOrder();
	}
	
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
	
	Timer_NoMotion = 0;
	Timer_ScreenOn = 0;
	Event_registerMode(TASK_TRACKER);
	
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
		//低功耗
#if(A20_DEBUG == OFF)
		/*蓝牙关闭,进入STOP低功耗模式*/
		if(Flag.Power == 0)
		{
			PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
		}
		/*蓝牙开启,进入SLEEP低功耗模式*/
		else if(Flag.Power == 1)
		{
			/*进入SLEEP模式*/
			PWR_EnterSleepMode(PWR_Regulator_LowPower, PWR_SLEEPEntry_WFI);			
		}
#endif
		
		/**************************************************/
	  	//运动数据存储
		DataSave_writeToEEPROM();

		/**************************************************/
		Battery_update();
		
		if(Battery_getBatteryLevel() == BATTERY_DANGER)
		{
			Timer_NoMotion = 0;
			Timer_ScreenOn = 0;

			BLE_closePort();
			
			//*任务切换---->关机模式*
			LPSA_TaskSwitch(TASK_STOP);
			
			return;
		}
		
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
				main_ACCEL();
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
			  	/*获取界面个数*/
				displayCount = Display_getDisplayCount();
			
			  	if(dis_index >= displayCount)
				{
					dis_index = 0;
				}
				
				/*更新屏幕*/
				func_disp[dis_index]();

				Graphics_repaint();
			}
		}

		/**************************************************/
		//USB状态处理		
		if(VBUS_API_GetVBus() == USB_ON)
		{
			Timer_NoMotion = 0;
			Timer_ScreenOn = 0;
			
			BLE_closePort();
			
			Graphics_displayOff(); 
						
			LPSA_TaskSwitch(TASK_USB);
				
			return;
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
		
#if	(TAP == ON)
		if(keyType == NO_KEY)
		{
			keyType = LIS3DH_getClickState();
		}
#endif
		
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
						Timer_NoMotion = 0;		
					}
					break;
				}
				case SHORT_KEY:
                                case LONG_KEY:
				{
				  	dis_index ++;
					
					Graphics_clearBuffer();
					
					Timer_ScreenOn = 0;
					Timer_NoMotion = 0;	
					
					//短按键时开启蓝牙开关
					BLE_open();

					break;
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
					if(Calculate_getUserState() == NO_MOVING)
					{
						if(Timer_NoMotion == 0)
						{
							RTC_obtainMilliSeconds(&Timer_NoMotion);
						}						
						
						if(RTC_checkTimeout(Timer_NoMotion,DEVICE_ON) == IS_TIMEOUT)
						{
							Timer_NoMotion = 0;
							Timer_ScreenOn = 0;
							
							/*切换到关机任务*/
							if(Flag.Power == 0)
							{
							  	BLE_closePort();
								
								/*任务切换---->关机模式*/
							  	LPSA_TaskSwitch(TASK_STOP);
								
								return;
							}
						//*************************************************
						}
					}
					else
					{
						Timer_NoMotion = 0;
						Timer_ScreenOn = 0;
					}
					break;
				}
				case SHORT_KEY:
				case LONG_KEY:
				{
				  	//按键时开启蓝牙开关
					BLE_open();
					
					Graphics_init();
					
					Timer_NoMotion = 0;
					Timer_ScreenOn = 0;
					
					break;
				}
				default:break;
			}	
		}
		
		/***********************************/
		/*蓝牙应用层*/
		
		APP_BLE();
		/***********************************/	
  	}
}