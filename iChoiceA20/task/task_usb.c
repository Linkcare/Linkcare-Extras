#include "task_usb.h"
#include "DataSave.h"

static uint64 Timer_ScreenOn  = 0;
static uint64 keyType = 0;

static uint8  receiveCounter = 0;               // 接收计数器
static uint8  sendCounter = 0;                  // 发送计数器(将要被发送的长度)
static uint8  receiveBuffer[64];                // 接收缓冲区
static uint8  sendBuffer[64];                   // 发送缓冲区

extern uint8 required_to_off;

void task_usb(void *prg)
{
  	(void)prg;//用于任务间数据隐含交换指针
	
	/*************初始化MCU********************/
	__disable_interrupt();
	
	/*时钟初始化*/
	Clock_TaskUSB();
	
	VBUS_enableCable();
	
	/*有关屏幕的初始化*/
	Graphics_init();
	
	/*30ms定时器初始化*/
	RTC_enableRTCWakeup();
	
	/*有关电池的操作*/
	Battery_disable();
	Battery_openChargePort();
		
	/*命令管理器初始化*/
	CommandManager_init();

	// USB中断配置
	USB_Interrupts_Config();
	
	// 设置USB时钟
	Set_USBClock();
	
	// USB接口初始化
	USB_Init();
	
	Timer_ScreenOn = 0;
			
	Event_registerMode(TASK_USB);
	
	__enable_interrupt();
	/************初始化MCU********************/  
  	while (1)
  	{	
	  	/**************************************************/
	  	//运动数据存储
		DataSave_writeToEEPROM();

		/**************************************************/
		if(VBUS_API_GetVBus() == USB_OFF)	//USB拔出信号
		{
		  	Timer_ScreenOn = 0;
			
			Graphics_clearScreen();
			Graphics_displayOff();
			
			//关闭内部上拉
  			PowerOff();
			
  			//关闭USB总线
  			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, DISABLE);
			
			RCC_PLLCmd(DISABLE);
			RCC_HSEConfig(RCC_HSE_OFF);
			
			if(required_to_off == 0)
			{
				/*任务切换---->关机模式*/
				LPSA_TaskSwitch(TASK_STOP);
			}
			else
			{
			  	required_to_off = 0;
				/*任务切换---->关机模式*/
				LPSA_TaskSwitch(TASK_DEEPSLEEP);
			}
			
			return;
		}
		else
		{
			/*USB通信部分*/		
			if(USB_connectionState() == CONFIGURED)
			{
				// 下面要处理USB的数据，这里是USB处理的核心
				receiveCounter = hidReceiveDataInBuffer(receiveBuffer,64,0);    // 接收指令
				CommandManager_saveReceivedCmd(receiveBuffer,receiveCounter);   // 保存指令
				CommandManager_process();                                       // 处理指令
				sendCounter = CommandManager_getSendingCmd(sendBuffer);         // 获取要发送的指令
				hidSendDataInBackground(sendBuffer,sendCounter,0,0xFFFFF);      // 发送指令
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