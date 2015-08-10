#include "task_stop.h"

#include "Upload.h"
#include "DataSave.h"
#include "ACCEL.h"
#include "ACCEL-SLEEP.h"
#include "Calculate.h"
#include "Configuration.h"

static uint8 keyType = 0;

void task_deepSleep(void *prg)
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

	/*关闭加速度传感器*/
	LIS3DH_openSPI();
	LIS3DH_init();
	LIS3DH_Motion_mode();
	LIS3DH_powerdown_mode();
	LIS3DH_closeSPI();
    
	/*关闭蓝牙*/
	BLE_closePort();
	
	/*关闭环境测量传感器*/
	LPS331AP_openSPI();
	LPS331AP_init();
	LPS331AP_close();
	LPS331AP_closeSPI();
	
	/*关闭EEPROM*/
	EEPROM_closeSPI();

	//20ms定时器初始化
	RTC_enableRTCWakeup();

	Event_registerMode(TASK_DEEPSLEEP);
	
	__enable_interrupt();
	
    /************初始化MCU********************/  
	while (1)
	{	
	 	/*下面的函数，将MCU设置到STOP状态，以降低功耗*/  
	  
    	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
		/*********************************************/
		
		if(KEY_API_CheckKey() == 1)
		{
			keyType = KEY_API_GetKey();
		}
		else
		{
			keyType = NO_KEY;
		}

		if(keyType == LONG_KEY)
		{
			/*设置管理模块初始化*/
			UserConfig_init();

			/*RTC初始化*/
			RTC_init();	

			/*存储初始化*/
			DataSave_init();

			/*上传初始化*/
			Upload_init();

			/*数据计算模块初始化*/
			Calculate_init();

			/*记步算法初始化*/
			Init_ACCEL();

			/*睡眠算法初始化*/
			Init_SLEEP();
			
			/*任务切换---->关机模式*/
			LPSA_TaskSwitch(TASK_TRACKER);
			
			return;
		}
  	}
}