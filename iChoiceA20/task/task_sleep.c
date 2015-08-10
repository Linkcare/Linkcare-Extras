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
	
	/*�йؼ��ٶȴ������ĳ�ʼ��*/
	LIS3DH_openSPI();
	LIS3DH_init();
	LIS3DH_FIFO_mode();
	LIS3DH_closeSPI();

	/*�ر���ѹ������*/
	LPS331AP_closeSPI();
	
	/*20ms��ʱ����ʼ��*/
	RTC_enableRTCWakeup();
	
	/*˯���㷨��ʼ��*/
	Init_SLEEP();
	
	Timer_ScreenOn = 0;

	/*��������ģʽ*/
	Event_registerMode(TASK_SLEEP);
	
	/*��Ļ��س�ʼ��*/
	if(Event_getLastMode() == TASK_STOP)
	{
		/*��Ļ��ʼ��*/
		if(lastDeviceOff != 0) //�������Ϊ���״ο���
		{	
			if(offMode == DEEPLY_OFF)
			{	
				/*����ϴιػ�Ϊ��ȹػ�������Ļ*/
				Graphics_init();
			}
		}
		else	//�������Ϊ�״ο���
		{
			/*����Ļ*/
			Graphics_init();
		}
	}
	else
	{
		/*����Ļ*/
		Graphics_init();
	}
	
	/*���EXTI LINE�ж�����*/
	EXTI_ClearITPendingBit(EXTI_Line0);
	EXTI_ClearITPendingBit(EXTI_Line3);
	EXTI_ClearITPendingBit(EXTI_Line13);

	__enable_interrupt();
	
	/************��ʼ��MCU********************/  
	while(1)
	{	
	  	/**************************************************/
		//�˶����ݴ洢
		DataSave_writeToEEPROM();
		
		/**************************************************/
		#if(A20_DEBUG == OFF)
		//�͹���
		PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
		#endif
		
		/**************************************************/
		//�˶����ݴ洢
		DataSave_writeToEEPROM();
		
		/**************************************************/
		Battery_update();
		
		if(Battery_getBatteryLevel() == BATTERY_DANGER)
		{
			Timer_ScreenOn = 0;

			//*�����л�---->�ػ�ģʽ*
			LPSA_TaskSwitch(TASK_STOP);
			
			return;
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
		if(LIS3DH_getDataStatus() == DATA_IS_READY)
		{
		  	LIS3DH_feed();

			for(uint8 i = 0;i < FIFO_LENGTH;i++)
			{
				DateSLEEP = LIS3DSH_getAccData(i);
				main_SLEEP();
				
				if(flag_update == 1)
				{
				  	//����˯������
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
				/*������Ļ*/
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
			/*��������*/
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