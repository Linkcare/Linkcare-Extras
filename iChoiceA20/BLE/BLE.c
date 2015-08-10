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


/*����������*/
void BLE_open(void)
{
	Flag.Power = 1;
}
/*�ر���������*/
void BLE_close(void)
{
	Flag.Power = 0;
}
/*********************���ڶ�ʱ���ж���********************/

lpsa_u16 sum = 0;
lpsa_u16 BleDelay(lpsa_u8	k)//0:��0�� 1��������2�����ʱ��
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

/********************BLE���ݽṹ��ʼ��********************************/
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
	//�ڴ��ж������������룬��������1234
	BleApi.BleReadPwd();
	#ifdef	TIME_OUT
		Flag.CommTime 			= 0;
		Flag.PwdTime 			= 0;
        Flag.PwdRnewTime		= 0;
	#endif 
}

/********************״̬����ʼ��********************************/
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

/*******************��״̬�����ڵ͹��Ļ�����ģʽ**********************/
//�Ʋ�����
//�߶ȼ���
//���ܣ��ϴ�15���ӵ����ݣ��������ȴ�500ms��Ӧ����ʱ��Ϊʧ�ܣ�
//�´��ٴ������سɹ������ݲ�������autotime����ʱ��ڵ�
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
	//�򿪴��书�ܣ��������ݴ���
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
	//���������㲥30S�������ʱ�˳�����ͨ��
	case BLE_IDLE_OPEN:
		{
		  
		  	//BLEģ��wakeup���߽������ߣ���ʡ����
		  	BleApi.BleLowPower ();
		  
			if(BleApi.BleCtrl() == 0)
				goto CASE_BLE_IDLE_TIME;
			if(BleDelay(MS_READ) > 3000)//�㲥��ʱ�ر�BLE��    
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
	//BLE��ʼͨ��
	case BLE_IDLE_COMM:
		{
            if(BleApi.BleCtrl() < 2)//�������㲥״̬���������ͨ��
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
					//���¶�ȡ����
					Idle.BLE_IDLE_L = BLE_IDLE_CLOES;
				}break;
				default:
					Idle.BLE_IDLE_L = BLE_IDLE_CLOES;
					break;					
			}		
		}break;
	/*************************BREAK*****************************/
	
	
	/***********************BLE_IDLE_CLOES**********************/
	//�ر�����
	case BLE_IDLE_CLOES:
		{
        	BLE_closePort();
        	AllIdleInit();
		}break;
	/***************************BREAK***************************/

	/***********************BLE_IDLE_DELAY**********************/
	//״̬����ʱ
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
	
	/*��ʾ�������*/
	if(Flag.Pwd == 2)	//�����������
	{	
	  	static uint16 lastpwd = 0;
		
		if(lastpwd != pwd)
		{
			lastpwd = pwd;
			
			//���ϴκͱ������벻һ�£���������ʾ
			if(Graphics_isDisplayOn() == DISPLAY_OFF)
			{
				Graphics_init();
			}
			
			Timer_ScreenOn = 0;
		}
		
		keyType =(uint8)KEY_API_CheckKey();//��ȡ����ֵ
		
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
						Graphics_displayOff();  	//�ر���Ļ
						Timer_ScreenOn = 0;			//��Ļ��ʱ������
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
			
			//��ʾ����	
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
				Graphics_init();	//��Ļ��ʼ��
				Timer_ScreenOn = 0;
			}
		}
		
		/*����ĺ�������MCU���õ�SLEEP״̬���Խ��͹���*/
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
