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
  	(void)prg;//���������������������ָ��
	
    /*************��ʼ��MCU********************/
	__disable_interrupt();
	
	System_disablePin();
	
	/*ʱ�ӳ�ʼ��*/
  	Clock_init(); 

	Graphics_displayOff();
	
	/*�йص�صĲ���*/
	Battery_disable();
	Battery_closeChargePort();

	/*�رռ��ٶȴ�����*/
	LIS3DH_openSPI();
	LIS3DH_init();
	LIS3DH_Motion_mode();
	LIS3DH_powerdown_mode();
	LIS3DH_closeSPI();
    
	/*�ر�����*/
	BLE_closePort();
	
	/*�رջ�������������*/
	LPS331AP_openSPI();
	LPS331AP_init();
	LPS331AP_close();
	LPS331AP_closeSPI();
	
	/*�ر�EEPROM*/
	EEPROM_closeSPI();

	//20ms��ʱ����ʼ��
	RTC_enableRTCWakeup();

	Event_registerMode(TASK_DEEPSLEEP);
	
	__enable_interrupt();
	
    /************��ʼ��MCU********************/  
	while (1)
	{	
	 	/*����ĺ�������MCU���õ�STOP״̬���Խ��͹���*/  
	  
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
			/*���ù���ģ���ʼ��*/
			UserConfig_init();

			/*RTC��ʼ��*/
			RTC_init();	

			/*�洢��ʼ��*/
			DataSave_init();

			/*�ϴ���ʼ��*/
			Upload_init();

			/*���ݼ���ģ���ʼ��*/
			Calculate_init();

			/*�ǲ��㷨��ʼ��*/
			Init_ACCEL();

			/*˯���㷨��ʼ��*/
			Init_SLEEP();
			
			/*�����л�---->�ػ�ģʽ*/
			LPSA_TaskSwitch(TASK_TRACKER);
			
			return;
		}
  	}
}