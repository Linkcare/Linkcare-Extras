#include "task_stop.h"
#include "DataSave.h"

static uint8 sysEvent = 0;

volatile uint8 offMode;
uint64 lastDeviceOff = 0;

void task_stop(void *prg)
{
  	(void)prg;//用于任务间数据隐含交换指针
	
    /*************初始化MCU********************/
	__disable_interrupt();
	
	System_disablePin();
	
	/*时钟初始化*/
  	Clock_init(); 

	Graphics_displayOff();
	
	/*有关电池的操作*/
	Battery_disable();
	Battery_closeChargePort();

	/*开启晃动唤醒*/
	LIS3DH_openSPI();
	LIS3DH_init();
	LIS3DH_Motion_mode();
	LIS3DH_closeSPI();
    
	/*关闭蓝牙*/
	BLE_closePort();
	
	/*关闭环境测量传感器*/
	LPS331AP_closeSPI();
	
	/*关闭EEPROM*/
	EEPROM_closeSPI();
	  
	//休眠前将按键端口初始化成中断方式
  	KEY_enableInt();

	//休眠前将VBUSON初始化成中断方式
	VBUS_enableVBUSON();

	//将RTC唤醒周期改为5s
	RTC_enableRTCForStop();
	
	//记录上次关机时间
	RTC_obtainMilliSeconds(&lastDeviceOff);
	
	//清除系统事件
	Event_clearEvent();
	
	Event_registerMode(TASK_STOP);
	
	__enable_interrupt();
	
    /************初始化MCU********************/  
	while (1)
	{	
		/**************************************************/
	  	//运动数据存储
		DataSave_writeToEEPROM();
#if (A20_DEBUG == OFF)
	 	/*下面的函数，将MCU设置到STOP状态，以降低功耗*/  
	  
    	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
#endif
		/**************************************************/
	  	//运动数据存储
		DataSave_writeToEEPROM();

		/**************************************************/
		sysEvent = Event_getEvent();

		switch (sysEvent)
		{
			case USB_VBUSON:
			{		 
			 	__disable_interrupt();
			  
				KEY_disableInt();
				VBUS_disableVBUSON();
				
				/*任务切换---->USB通信模式*/
				LPSA_TaskSwitch (TASK_USB);
				
				__enable_interrupt();
				
				return;
			}
			case USER_SHAKE:
			{
			  	__disable_interrupt();
				
				/*检测电池电量*/
				Battery_check();
				
				if(Battery_getBatteryLevel()== BATTERY_DANGER)
				{
				  	//电池电量极低，软件阻止开机
				  	__enable_interrupt();
					
					break;	
				}
				else
				{
				  	//禁止按键中断
					KEY_disableInt();
					
					//禁止VBUS中断
					VBUS_disableVBUSON();
				
					if(RTC_checkTimeout(lastDeviceOff,TURNOFF_PERIOD) == IS_TIMEOUT)
					{	
					  	//如果设备关机时间超过预设门限值，则判断为深度关机，晃动唤醒后
						//显示问候语和鼓励语，之后进入正常显示模式
					  
						/*有关屏幕的初始化*/	
						Graphics_init();
				
						/*显示晃动唤醒界面*/
						infor_disp[SHAKE]();
						
						offMode = DEEPLY_OFF;
					}
					else
					{	
					  	//如果设备关机时间未超过预设门限值，则判断为浅度关机，晃动唤醒后
						//进入关屏模式
					  
						offMode = LITTLE_OFF;
					}

					/*任务切换---->距离关机状态最近的非USB状态*/
					LPSA_TaskSwitch(Event_getLastMode());
					
					__enable_interrupt();
					
					return;
				}
			}
			case KEY_PRESSED:
			{		
			 	__disable_interrupt();
				
				/*检测电池电量*/
				Battery_check();
				
				if(Battery_getBatteryLevel()== BATTERY_DANGER)
				{
				  	//电池电量极低，软件阻止开机
				 	__enable_interrupt(); 	
				 
					break;	
				}
				else
				{
					/*打开蓝牙*/
					BLE_open();	
					
					/*退出stop之前关闭按键和VBUSon 中断*/
					KEY_disableInt();
					VBUS_disableVBUSON();
					
					/*任务切换---->距离关机状态最近的非USB状态*/
					LPSA_TaskSwitch(Event_getLastMode());
					
					__enable_interrupt();
					
					return;		
				}
			}
			default:break;		
		}
  	}
}