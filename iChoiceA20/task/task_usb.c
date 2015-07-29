#include "task_usb.h"
#include "DataSave.h"

static uint64 Timer_ScreenOn  = 0;
static uint64 keyType = 0;

static uint8  receiveCounter = 0;               // ���ռ�����
static uint8  sendCounter = 0;                  // ���ͼ�����(��Ҫ�����͵ĳ���)
static uint8  receiveBuffer[64];                // ���ջ�����
static uint8  sendBuffer[64];                   // ���ͻ�����

extern uint8 required_to_off;

void task_usb(void *prg)
{
  	(void)prg;//���������������������ָ��
	
	/*************��ʼ��MCU********************/
	__disable_interrupt();
	
	/*ʱ�ӳ�ʼ��*/
	Clock_TaskUSB();
	
	VBUS_enableCable();
	
	/*�й���Ļ�ĳ�ʼ��*/
	Graphics_init();
	
	/*30ms��ʱ����ʼ��*/
	RTC_enableRTCWakeup();
	
	/*�йص�صĲ���*/
	Battery_disable();
	Battery_openChargePort();
		
	/*�����������ʼ��*/
	CommandManager_init();

	// USB�ж�����
	USB_Interrupts_Config();
	
	// ����USBʱ��
	Set_USBClock();
	
	// USB�ӿڳ�ʼ��
	USB_Init();
	
	Timer_ScreenOn = 0;
			
	Event_registerMode(TASK_USB);
	
	__enable_interrupt();
	/************��ʼ��MCU********************/  
  	while (1)
  	{	
	  	/**************************************************/
	  	//�˶����ݴ洢
		DataSave_writeToEEPROM();

		/**************************************************/
		if(VBUS_API_GetVBus() == USB_OFF)	//USB�γ��ź�
		{
		  	Timer_ScreenOn = 0;
			
			Graphics_clearScreen();
			Graphics_displayOff();
			
			//�ر��ڲ�����
  			PowerOff();
			
  			//�ر�USB����
  			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, DISABLE);
			
			RCC_PLLCmd(DISABLE);
			RCC_HSEConfig(RCC_HSE_OFF);
			
			if(required_to_off == 0)
			{
				/*�����л�---->�ػ�ģʽ*/
				LPSA_TaskSwitch(TASK_STOP);
			}
			else
			{
			  	required_to_off = 0;
				/*�����л�---->�ػ�ģʽ*/
				LPSA_TaskSwitch(TASK_DEEPSLEEP);
			}
			
			return;
		}
		else
		{
			/*USBͨ�Ų���*/		
			if(USB_connectionState() == CONFIGURED)
			{
				// ����Ҫ����USB�����ݣ�������USB����ĺ���
				receiveCounter = hidReceiveDataInBuffer(receiveBuffer,64,0);    // ����ָ��
				CommandManager_saveReceivedCmd(receiveBuffer,receiveCounter);   // ����ָ��
				CommandManager_process();                                       // ����ָ��
				sendCounter = CommandManager_getSendingCmd(sendBuffer);         // ��ȡҪ���͵�ָ��
				hidSendDataInBackground(sendBuffer,sendCounter,0,0xFFFFF);      // ����ָ��
			}
			
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
				if(Battery_isCharging() == BATTERY_IS_CHARGING)
				{
					infor_disp[CHARGING]();
				}
				else if(Battery_isCharging() == BATTERY_IS_FULL)
				{
					infor_disp[BATTEYFULL]();
				}
				
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
							Graphics_clearScreen();
							Graphics_displayOff(); 
							Timer_ScreenOn = 0;
						}
						break;
					}
					case SHORT_KEY:
					case LONG_KEY:
					{
						Timer_ScreenOn = 0;
						Battery_check();
						break;
					}
					default:break;			
				}
			}
			else
			{
				if(keyType != NO_KEY)
				{
					Graphics_init();
					Timer_ScreenOn = 0;
				}
			}	    
		}
  	}
}