#include "BLE.H"
#include "Key.h"
#include "Type.h"
#include "RTC.h"
#include "Graphics.h"
#include "System.h"
#include "stm32l1xx.h"
#include "DeviceInfo.h"

BLE_TOOLS 			BleTools;
BLE_IDLE_STRUCT		Idle;
STATUS_STRUCT		Flag;
#ifdef TIME_OUT
	TIME_OUT_STRUCT	TimeOut;
#endif
#ifdef DEBUG
	DEBUG_STRUCT Debug;
#endif
lpsa_i8	WrStart = 0;

BLE_STRUCT BleApi =
{
    powerOn,
    powerOff,
    BleCtrl,
    BleBuffCtrl,
    WriteBlock,
    ReadBlock,
    ReadChar,
    BleWakeUp,
	BleLowPower,
	BleSavePwd,
	BleReadPwd
};


/*打开蓝牙开关*/
void BLE_open(void)
{
	Flag.Power = 1;
}
/*关闭蓝牙开关*/
void BLE_close(void)
{
	Flag.Power = 0;
}
/*********************放在定时器中断中********************/

lpsa_u16 sum = 0;
lpsa_u16 BleDelay(lpsa_u8	k)//0:清0， 1：计数，2：获得时间
{
	if(Flag.Power == 0)
		return	0;
		
	switch(k)
	{
	case 0:
		sum = 0;
		break;
	case 1:
		sum ++;
		break;
	case 2:
		return sum;
	}
#ifdef	TIME_OUT
		TimeOut.CommTime += 1;
#endif
	return 0;
}

/********************BLE数据结构初始化********************************/
void BLE_INIT(void)
{
	Flag.PwdSus					=	0;
	Flag.Power					=	0;
	Flag.Off					=	0;
	BleTools.Error				= 	0;
	BleTools.SEND_BLOCK_INFO	=	0;
	Flag.Pwd					=	0;
	Idle.BLE_IDLE_L				=	BLE_IDLE_POWER_ON;
	Idle.BLE_IDLE_C        		= 	COMM_GET_DATA_0;
	Idle.BLE_IDLE_S      		= 	WRITE_BLOCK_BEGIN;
	Idle.BLE_IDLE_D				= 	DATA_SEND_BLOCK_BEGIN;
	//在此判断蓝牙连接密码，出厂密码1234
	BleApi.BleReadPwd();
	#ifdef	TIME_OUT
		Flag.CommTime 			= 0;
		Flag.PwdTime 			= 0;
        Flag.PwdRnewTime		= 0;
	#endif 
}

/********************状态机初始化********************************/
void AllIdleInit(void)
{
	Flag.PwdSus					=	0;
	Flag.Power					= 	0;
	Flag.Off					=	0;
	BleTools.Error				= 	0;
	BleTools.SEND_BLOCK_INFO	=	0;
		Flag.Pwd				=	0;
	Idle.BLE_IDLE_L				=	BLE_IDLE_POWER_ON;
	Idle.BLE_IDLE_C        		= 	COMM_GET_DATA_0;
	Idle.BLE_IDLE_S      		= 	WRITE_BLOCK_BEGIN;
	Idle.BLE_IDLE_D				= 	DATA_SEND_BLOCK_BEGIN;
	#ifdef	TIME_OUT
		Flag.CommTime 			= 0;
		Flag.PwdTime 			= 0;
		Flag.PwdRnewTime		= 0;
	#endif 
}

/*******************此状态运行在低功耗或正常模式**********************/
//计步功能
//高度计算
//功能：上传15分钟的数据，发送完后等待500ms回应，超时认为失败，
//下次再传。返回成功，数据不存增加autotime函数时间节点
static lpsa_u8	 DleyJmp = 0;
static lpsa_u16  DelayMs = 0;

void BLE_IDLE_RUN(void)
{
	if(Flag.Power == 0)
		return;
	switch(Idle.BLE_IDLE_L)
	{
        /********************BLE_IDLE_POWER_ON***********************/
        case BLE_IDLE_POWER_ON:
          {
            BleInit ();
            initSerialPort ();
            Idle.BLE_IDLE_L = BLE_IDLE_DELAY;
            BleDelay(MS_CLEAN);
	    	DleyJmp = BLE_IDLE_TIME;
	    	DelayMs = 10;
          }break;
          

	/********************BLE_IDLE_TIME**************************/
	//打开传输功能，进行数据传输
	case BLE_IDLE_TIME:
	CASE_BLE_IDLE_TIME:
		{
			BleApi.powerOn();
			BleDelay(MS_CLEAN);
			Idle.BLE_IDLE_L = BLE_IDLE_DELAY;
			DleyJmp = BLE_IDLE_OPEN;
			DelayMs = 30;			
		}break;
	/*************************BREAK*****************************/

	/*********************BLE_IDLE_OPEN*************************/
	//打开蓝牙，广播30S，如果超时退出本次通信
	case BLE_IDLE_OPEN:
		{
		  
		  	//BLE模块wakeup拉高进行休眠，节省功耗
		  	BleApi.BleLowPower ();
		  
			if(BleApi.BleCtrl() == 0)
				goto CASE_BLE_IDLE_TIME;
			if(BleDelay(MS_READ) > 3000)//广播超时关闭BLE；    
			{
				BleTools.Error = -1;
				Idle.BLE_IDLE_L = BLE_IDLE_CLOES;	
			}
			if(BleApi.BleCtrl() == 2 || BleApi.BleCtrl() == 3)
			{
				Idle.BLE_IDLE_L = BLE_IDLE_COMM;
				#ifdef	TIME_OUT
					TimeOut.CommTime = 0;
					Flag.PwdTime 	 = 1;
					Flag.CommTime	 = 0;
				#endif
			}
          
		}break;
	/*************************BREAK*****************************/
	
	/*********************BLE_IDLE_COMM*************************/
	//BLE开始通信
	case BLE_IDLE_COMM:
		{
            if(BleApi.BleCtrl() < 2)//如果进入广播状态则结束本次通信
            {
                Idle.BLE_IDLE_L = BLE_IDLE_CLOES;
                break;
            }
			BleTools.Error = BLE_NOMAL_COMM();
			switch(BleTools.Error)
			{
				case 0:				
					break;
				case  1:
				case -1:
				case -2:
				case -3:
				case -4:
				case -5:
					Idle.BLE_IDLE_L = BLE_IDLE_CLOES;
					break;
				case -6:
					BleApi.powerOn();
					BleDelay(MS_CLEAN);
					Idle.BLE_IDLE_L = BLE_IDLE_DELAY;
					DleyJmp = BLE_IDLE_OPEN;
					DelayMs = 50;					
					break;
				case -7:
				{
					//重新读取密码
					Idle.BLE_IDLE_L = BLE_IDLE_CLOES;
				}break;
				default:
					Idle.BLE_IDLE_L = BLE_IDLE_CLOES;
					break;					
			}		
		}break;
	/*************************BREAK*****************************/
	
	
	/***********************BLE_IDLE_CLOES**********************/
	//关闭蓝牙
	case BLE_IDLE_CLOES:
		{
        	BLE_closePort();
        	AllIdleInit();
		}break;
	/***************************BREAK***************************/

	/***********************BLE_IDLE_DELAY**********************/
	//状态机延时
	case BLE_IDLE_DELAY:
		{
			if(BleDelay(MS_READ) > DelayMs)
			{
				BleDelay(MS_CLEAN);
				Idle.BLE_IDLE_L = DleyJmp;
			}
		}break;
	/*************************BREAK*****************************/

	default:
		AllIdleInit();
		goto CASE_BLE_IDLE_TIME;
	}
}

static uint64 Timer_ScreenOn = 0;
static uint8 keyType = 0;

extern uint16 pwd;
void APP_BLE(void)
{
BEGIN:

	BLE_IDLE_RUN();
	
	/*显示随机密码*/
	if(Flag.Pwd == 2)	//产生随机密码
	{	
	  	static uint16 lastpwd = 0;
		
		if(lastpwd != pwd)
		{
			lastpwd = pwd;
			
			//若上次和本次密码不一致，则重新显示
			if(Graphics_isDisplayOn() == DISPLAY_OFF)
			{
				Graphics_init();
			}
			
			Timer_ScreenOn = 0;
		}
		
		keyType =(uint8)KEY_API_CheckKey();//读取按键值
		
		if(Graphics_isDisplayOn() == DISPLAY_ON)
		{
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
						Graphics_displayOff();  	//关闭屏幕
						Timer_ScreenOn = 0;			//屏幕计时器清零
					}
					break;
				}
				case SHORT_KEY:
				case LONG_KEY:
				{
					Timer_ScreenOn = 0;
				}
				default:break;			
			}
			
			//显示密码	
			Graphics_DispStringDec(38, BleTools.PWD % 10000 / 1000);
    		Graphics_DispStringDec(53, BleTools.PWD % 1000 / 100);
    		Graphics_DispStringDec(68, BleTools.PWD % 100 / 10);
    		Graphics_DispStringDec(83, BleTools.PWD % 10);
			Graphics_repaint();
		}
		else
		{
			if(keyType != NO_KEY)
			{
				Graphics_init();	//屏幕初始化
				Timer_ScreenOn = 0;
			}
		}
		
		/*下面的函数，将MCU设置到SLEEP状态，以降低功耗*/
    	PWR_EnterSleepMode(PWR_Regulator_LowPower, PWR_SLEEPEntry_WFI);
		
	  	goto BEGIN;
	}
	else
	{
		return;
	}
}

void BLE_OFF(void)
{
    BleApi.PowerOff();
	AllIdleInit();
}
