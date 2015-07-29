#include "type.h"
#include "stdlib.h"
#include "stm32l1xx.h"

#include "RTC.h"
#include "BLE.h"
#include "vbus.h"
#include "Clock.h"
#include "key.h"
#include "Battery.h"
#include "EEPROM.h"

#include "Upload.h"
#include "DataSave.h"
#include "ACCEL.h"
#include "ACCEL-SLEEP.h"
#include "Calculate.h"
#include "Configuration.h"

#include "Event.h"
#include "Task.h"
#include "System.h"
#include "DeviceInfo.h"

/**********************************************/
//A20�豸��Ϣ
const uint8 deviceType  			= A20;					//A20�豸���ͺ�
const uint8 version_issued[13] 		= FIRMWARE_ISSUED;		//A20��������汾��
const uint8 version_developer[13] 	= FIRMWARE_DEVELOPER;	//A20��������汾��
/***********************************************
* ���ܣ���ƽ������
* ������a - ��������
* ���أ����
***********************************************/
uint16 System_sqrt(uint32 a)
{ 
	uint16 s=0;

 	for(uint16 i=0x8000;i>0;i>>=1)
 	{
  		s|=i;
		
  		if(s * s > a)
		{
   			s &= ~i;
		}
 	}
 	return s;
}

// �������ܣ���ʱ
// �����������ʱ�ĺ�����
// �����������
// �� �� ֵ����
// ��    ע����
#pragma optimize = none
void System_delay(uint16 ms)
{
  	if(ms > 60000)
	  	return;
	
	for(uint16 i = 0;i < ms;i++)
	{
		for(uint16 j = 0;j < 500;j++)
		{
		  	__NOP();
		}
	}
}

// �������ܣ�ι��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void System_updateWatchDog(void)
{
    IWDG_ReloadCounter();//���ؿ��Ź�������-ι��
}

// �������ܣ����Ź���ʼ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע������ģʽ
void initWatchDog(void)
{
	// 1.������ʿ��Ź�������
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	// 2.���ÿ��Ź���Ԥ��Ƶ����Ϊ256���൱�ڿ��Ź���������ʱ��Ϊ37000Hz/256=144.5HZ
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	// 3.���ÿ��Ź��������ļ���ֵΪ1500���൱�ڿ��Ź����ʱ��Ϊ��1500/144.5=10.38��
	IWDG_SetReload(1500);
	// 4.���ؿ��Ź���������Ҳ�����൱��������Ź�
	IWDG_ReloadCounter();
	// 5.�������Ź�
	IWDG_Enable();
}
/***********************************************
* ���ܣ��ر�����I/O
***********************************************/
void System_disablePin(void)
{
	//��GPIO�˿ڵ�ʱ�ӣ�����ö˿ڹ���
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, ENABLE);

	GPIO_InitTypeDef gpioInit;

	//�ر�GPIOA
	gpioInit.GPIO_Pin 	= GPIO_Pin_0
						| GPIO_Pin_2
						| GPIO_Pin_3
						| GPIO_Pin_4
						| GPIO_Pin_5
						| GPIO_Pin_6
						| GPIO_Pin_7
						| GPIO_Pin_8
						| GPIO_Pin_9
						| GPIO_Pin_10
						| GPIO_Pin_11
						| GPIO_Pin_12				  
						| GPIO_Pin_15;

	gpioInit.GPIO_Speed = GPIO_Speed_400KHz;
	gpioInit.GPIO_Mode 	= GPIO_Mode_OUT;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &gpioInit);
	  
	GPIO_WriteBit(GPIOA,GPIO_Pin_0, Bit_RESET);     // USB_AD = 0
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_2, Bit_RESET);     // BMA_CS = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_3, Bit_RESET);     // BMA_INT1 = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_4, Bit_RESET);     // BMA_INT2 = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_5, Bit_SET);     	// BMA_SPICLK = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_6, Bit_SET);     	// BMA_SPIMISO = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_7, Bit_SET);       // BMA_SPIMOSI = 0 
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_8, Bit_RESET);     // EERPOM_S = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_9, Bit_RESET);     // CHARGE_STATUS = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_10,Bit_SET);       // BLE_ON = 1
	GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET);     // USB_D- = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_12,Bit_RESET);     // USB_D+ = 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);     // BLE_WAKE = 0

	//�ر�GPIOB
	gpioInit.GPIO_Pin 	= GPIO_Pin_0
						| GPIO_Pin_1
						| GPIO_Pin_2
						| GPIO_Pin_3
						| GPIO_Pin_4
						| GPIO_Pin_5
						| GPIO_Pin_6
						| GPIO_Pin_7
						| GPIO_Pin_8
						| GPIO_Pin_9
						| GPIO_Pin_10
						| GPIO_Pin_11
						| GPIO_Pin_12
						| GPIO_Pin_13
						| GPIO_Pin_14
						| GPIO_Pin_15;

	gpioInit.GPIO_Speed = GPIO_Speed_400KHz;
	gpioInit.GPIO_Mode 	= GPIO_Mode_OUT;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &gpioInit);
	  
	GPIO_WriteBit(GPIOB,GPIO_Pin_0, Bit_RESET);     // BMP_SPIMISO = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_1, Bit_RESET);     // BMP_SPIMOSI = 0  
	GPIO_WriteBit(GPIOB,GPIO_Pin_2, Bit_RESET);     // BMP_CS = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);     // BMP_SPICLK = 0 
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_3, Bit_RESET);     // BLE_STATUS0 = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_4, Bit_RESET);     // BLE_STATUS1 = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_5, Bit_RESET);     // BLE_CTL = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_6, Bit_RESET);     // BLE_RX = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_7, Bit_RESET);     // BLE_TX = 0
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_8, Bit_RESET);     // OLED_RES = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_9, Bit_SET);       // OLED_ON = 1
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);     // OLED_SCL = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_RESET);     // OLED_SDA = 0
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);     // EEPROM_C = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);     // EEPROM_Q = 0
	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET);     // EEPROM_D = 0
	
	//�ر�GPIOH
	gpioInit.GPIO_Pin 	= GPIO_Pin_0 | GPIO_Pin_1;
	gpioInit.GPIO_Speed = GPIO_Speed_400KHz;
	gpioInit.GPIO_Mode 	= GPIO_Mode_OUT;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOH, &gpioInit);
	  
	GPIO_WriteBit(GPIOH,GPIO_Pin_0,Bit_SET);       // PD0 = 0
	GPIO_WriteBit(GPIOH,GPIO_Pin_1,Bit_SET);       // PD1 = 0
}
/***********************************************
*����:ϵͳ��ʼ��
***********************************************/
void System_init(void)
{
    __disable_interrupt();
	
	/***********�������ʼ��***************/	
	
	//ʱ�ӳ�ʼ��
	Clock_init();
	
	//��ֹPVD  
  	PWR_PVDCmd(DISABLE);
  
  	//��ֹ�¶ȴ������ο���Դ
  	ADC_TempSensorVrefintCmd(DISABLE);
    
  	//�����͹�������
  	PWR_UltraLowPowerCmd(ENABLE);
	PWR_FastWakeUpCmd(ENABLE);
	
	//�ж����ȼ�����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	/*���ù���ģ���ʼ��*/
    UserConfig_init();
	
	/*RTC��ʼ��*/
	RTC_init();	
	
	/*KEY��ʼ��*/
	KEY_API_Init();
	
	/*�洢��ʼ��*/
	DataSave_init();
	
	/*�ϴ���ʼ��*/
	Upload_init();
	
	/*���ݼ���ģ���ʼ��*/
    Calculate_init();
	
	/*�ǲ��㷨��ʼ��*/
	Init_ACCEL();
	
	/*���ݳ�ʼ��*/
#if	(BACKUP == ON)
	
	/*RTC��ʼ��*/
	RTC_recover();
	
	/*�洢��ʼ��*/
	DataSave_recover();
	
	/*���ݼ���ģ���ʼ��*/
	Calculate_recover();
	
	/*�ǲ��㷨��ʼ��*/
	Init_ACCEL_recover();	
#endif
	
	/*˯���㷨��ʼ��*/
	Init_SLEEP();
	
	//���Ź���ʼ��
#if	(WATCHDOG == ON)
	
	/*���Ź���ʼ��*/
	initWatchDog();
#endif
    __enable_interrupt();
	
}