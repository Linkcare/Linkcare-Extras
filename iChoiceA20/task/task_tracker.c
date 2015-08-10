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
	����ʹ�ò��ԣ�
	ֻ����TRACKERģʽ����������ͨ
	�ţ����̰���ʱ�����������أ�
	��������ģʽʱ��ǿ�ƹر�������
**********************************/
void task_tracker(void *prg)
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
	
	/*�����û�����*/
	UserConfig_getDispOrder(displayBuf);
	
	if(Display_setDisplay(displayBuf,sizeof(displayBuf)) == ERROR)
	{
	  	UserConfig_initDispOrder();
	}
	
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
	
	Timer_NoMotion = 0;
	Timer_ScreenOn = 0;
	Event_registerMode(TASK_TRACKER);
	
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
		//�͹���
#if(A20_DEBUG == OFF)
		/*�����ر�,����STOP�͹���ģʽ*/
		if(Flag.Power == 0)
		{
			PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
		}
		/*��������,����SLEEP�͹���ģʽ*/
		else if(Flag.Power == 1)
		{
			/*����SLEEPģʽ*/
			PWR_EnterSleepMode(PWR_Regulator_LowPower, PWR_SLEEPEntry_WFI);			
		}
#endif
		
		/**************************************************/
	  	//�˶����ݴ洢
		DataSave_writeToEEPROM();

		/**************************************************/
		Battery_update();
		
		if(Battery_getBatteryLevel() == BATTERY_DANGER)
		{
			Timer_NoMotion = 0;
			Timer_ScreenOn = 0;

			BLE_closePort();
			
			//*�����л�---->�ػ�ģʽ*
			LPSA_TaskSwitch(TASK_STOP);
			
			return;
		}
		
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
				main_ACCEL();
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
			  	/*��ȡ�������*/
				displayCount = Display_getDisplayCount();
			
			  	if(dis_index >= displayCount)
				{
					dis_index = 0;
				}
				
				/*������Ļ*/
				func_disp[dis_index]();

				Graphics_repaint();
			}
		}

		/**************************************************/
		//USB״̬����		
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
		//��������
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
					
					//�̰���ʱ������������
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
							
							/*�л����ػ�����*/
							if(Flag.Power == 0)
							{
							  	BLE_closePort();
								
								/*�����л�---->�ػ�ģʽ*/
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
				  	//����ʱ������������
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
		/*����Ӧ�ò�*/
		
		APP_BLE();
		/***********************************/	
  	}
}