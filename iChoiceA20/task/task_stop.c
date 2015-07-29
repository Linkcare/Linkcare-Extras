#include "task_stop.h"
#include "DataSave.h"

static uint8 sysEvent = 0;

volatile uint8 offMode;
uint64 lastDeviceOff = 0;

void task_stop(void *prg)
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

	/*�����ζ�����*/
	LIS3DH_openSPI();
	LIS3DH_init();
	LIS3DH_Motion_mode();
	LIS3DH_closeSPI();
    
	/*�ر�����*/
	BLE_closePort();
	
	/*�رջ�������������*/
	LPS331AP_closeSPI();
	
	/*�ر�EEPROM*/
	EEPROM_closeSPI();
	  
	//����ǰ�������˿ڳ�ʼ�����жϷ�ʽ
  	KEY_enableInt();

	//����ǰ��VBUSON��ʼ�����жϷ�ʽ
	VBUS_enableVBUSON();

	//��RTC�������ڸ�Ϊ5s
	RTC_enableRTCForStop();
	
	//��¼�ϴιػ�ʱ��
	RTC_obtainMilliSeconds(&lastDeviceOff);
	
	//���ϵͳ�¼�
	Event_clearEvent();
	
	Event_registerMode(TASK_STOP);
	
	__enable_interrupt();
	
    /************��ʼ��MCU********************/  
	while (1)
	{	
		/**************************************************/
	  	//�˶����ݴ洢
		DataSave_writeToEEPROM();
#if (A20_DEBUG == OFF)
	 	/*����ĺ�������MCU���õ�STOP״̬���Խ��͹���*/  
	  
    	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
#endif
		/**************************************************/
	  	//�˶����ݴ洢
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
				
				/*�����л�---->USBͨ��ģʽ*/
				LPSA_TaskSwitch (TASK_USB);
				
				__enable_interrupt();
				
				return;
			}
			case USER_SHAKE:
			{
			  	__disable_interrupt();
				
				/*����ص���*/
				Battery_check();
				
				if(Battery_getBatteryLevel()== BATTERY_DANGER)
				{
				  	//��ص������ͣ������ֹ����
				  	__enable_interrupt();
					
					break;	
				}
				else
				{
				  	//��ֹ�����ж�
					KEY_disableInt();
					
					//��ֹVBUS�ж�
					VBUS_disableVBUSON();
				
					if(RTC_checkTimeout(lastDeviceOff,TURNOFF_PERIOD) == IS_TIMEOUT)
					{	
					  	//����豸�ػ�ʱ�䳬��Ԥ������ֵ�����ж�Ϊ��ȹػ����ζ����Ѻ�
						//��ʾ�ʺ���͹����֮�����������ʾģʽ
					  
						/*�й���Ļ�ĳ�ʼ��*/	
						Graphics_init();
				
						/*��ʾ�ζ����ѽ���*/
						infor_disp[SHAKE]();
						
						offMode = DEEPLY_OFF;
					}
					else
					{	
					  	//����豸�ػ�ʱ��δ����Ԥ������ֵ�����ж�Ϊǳ�ȹػ����ζ����Ѻ�
						//�������ģʽ
					  
						offMode = LITTLE_OFF;
					}

					/*�����л�---->����ػ�״̬����ķ�USB״̬*/
					LPSA_TaskSwitch(Event_getLastMode());
					
					__enable_interrupt();
					
					return;
				}
			}
			case KEY_PRESSED:
			{		
			 	__disable_interrupt();
				
				/*����ص���*/
				Battery_check();
				
				if(Battery_getBatteryLevel()== BATTERY_DANGER)
				{
				  	//��ص������ͣ������ֹ����
				 	__enable_interrupt(); 	
				 
					break;	
				}
				else
				{
					/*������*/
					BLE_open();	
					
					/*�˳�stop֮ǰ�رհ�����VBUSon �ж�*/
					KEY_disableInt();
					VBUS_disableVBUSON();
					
					/*�����л�---->����ػ�״̬����ķ�USB״̬*/
					LPSA_TaskSwitch(Event_getLastMode());
					
					__enable_interrupt();
					
					return;		
				}
			}
			default:break;		
		}
  	}
}