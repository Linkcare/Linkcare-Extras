#include "task_tracker.h"

static uint8  keyType        = 0;
static uint8  dis_index      = 0;
static uint64 time_to_refresh = 0;

void task_test(void *prg)
{
  	(void)prg;
	
  	__disable_interrupt();
	
	System_disablePin();
	
	/*ʱ�ӳ�ʼ��*/
	Clock_init();
	
	/*�ر�IO*/
	System_disablePin();
	
	/*������ʼ��*/
	BLE_closePort();
	
	/*�ر�EEPROM*/
	EEPROM_closeSPI();
	
	/*VBUS���*/
	VBUS_API_Init();
	
	/*�йص�صĲ���*/
	Battery_disable();
	Battery_closeChargePort();
	
	/*�йؼƲ��ĳ�ʼ��*/
	LIS3DH_openSPI();
	LIS3DH_init();
	LIS3DH_FIFO_mode();
	LIS3DH_closeSPI();

	/*��������*/
	LPS331AP_openSPI();
	LPS331AP_init();
	LPS331AP_closeSPI();
	
	/*20ms��ʱ����ʼ��*/
	RTC_enableRTCWakeup();
	
	/*˯���㷨��ʼ��*/
	Init_SLEEP();
	
	Event_registerMode(TASK_TRACKER);
	
	/*��Ļ��س�ʼ��*/
	Graphics_init();
		
	/*���EXTI LINE�ж�����*/
	EXTI_ClearITPendingBit(EXTI_Line0);
	EXTI_ClearITPendingBit(EXTI_Line3);
	EXTI_ClearITPendingBit(EXTI_Line13);
	
	__enable_interrupt();
	/************��ʼ��MCU********************/  
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
				
				/*�������κ��¶Ȳ���*/
				LPS331AP_open();

				/*���㲽��*/
				for(uint8 i = 0;i < FIFO_LENGTH;i++)
				{
					Data_ACCEL = LIS3DSH_getAccData(i);
				}
				
				/*��ȡ���κ��¶�����*/
				LPS331AP_readData();
				
				/*���ݼ���*/
				DataCalculate();
			}
			/*************************************************/
			//LIS3DH ������
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
					/*������Ļ*/
					test_disp[dis_index]();

					Graphics_repaint();
				}
			}
			
			/**************************************************/
			//��������
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
				/*��������*/
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
					  	//���ϵ��־λ��λ
					  	UserConfig_setPowerOnState();
					  
						/*�����л�---->trackerģʽ*/
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
			/*�����л�---->trackerģʽ*/
			LPSA_TaskSwitch(TASK_TRACKER);
			
			return;
		}
  	}
}