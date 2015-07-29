// ----------------------------------------------
// 1.�ļ�˵������USB���ߵ�Դ�ź�VBUS�����صĺ���
// ----------------------------------------------

// ----------------------------------------------
// 2.ͷ�ļ�
// ----------------------------------------------
#include "stm32l1xx_conf.h"
#include "usb_lib.h"
#include "usb_prop.h"
#include "hw_config.h"
#include "Event.h"
#include "Task.h"
#include "vbus.h"
#include "key.h"

// 1. �ӿڲ�

// ----------------------------------------------
// ��������
// ----------------------------------------------
// VBUS��Ӧ�Ķ˿ں�λ�Ķ��壺PA0
#define VBUS_CTRL     RCC_AHBPeriph_GPIOA   // VBUS��Ӧ�Ŀ��ƿ�
#define VBUS_PORT     GPIOA                 // VBUS��Ӧ��GPIO�˿�
#define VBUS_BIT      GPIO_Pin_0            // VBUS��Ӧ��GPIOλ  


// ----------------------------------------------
// �궨��
// ----------------------------------------------

// ----------------------------------------------
// ��������
// ----------------------------------------------
static uint8 VBusState;                // VBUS״̬��

// ----------------------------------------------
// ����ԭ��
// ----------------------------------------------

// ----------------------------------------------
// ����ʵ��
// ----------------------------------------------

// 1.�ӿڲ�ĺ���ʵ��

// �������ܣ�VBUS�˿ڳ�ʼ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
static void VBUS_GPIO_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
  	// ��VBUS��Ӧ��GPIO�˿ڵ�ʱ�ӣ�����ö˿ڹ���
  	RCC_AHBPeriphClockCmd(VBUS_CTRL, ENABLE);

  	// ��ʼ��VBUS��Ӧ��GPIO�˿�Ϊ����
  	GPIO_InitStructure.GPIO_Pin   = VBUS_BIT;
  	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
  	GPIO_Init(VBUS_PORT, &GPIO_InitStructure);
}

// �������ܣ����뵱ǰVBUS��״ֵ̬
// �����������
// �����������
// �� �� ֵ��VBUS_ON  -- ��ʾVBUS�е�
//           VBUS_OFF -- ��ʾVBUSû��
// ��    ע����
static uint8 VBUS_GPIO_GetValue()
{
  	if(GPIO_ReadInputDataBit(VBUS_PORT,VBUS_BIT) == 0x00)
  	{
    	return VBUS_OFF;
  	}
  	else
  	{
    	return VBUS_ON;
  	}
}

// 2.�豸��ĺ���ʵ��

// �������ܣ�VBUS�豸�ĳ�ʼ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
static void VBUS_DEV_Init()
{
  	VBUS_GPIO_Init();                      // ���ýӿڲ㺯�������GPIO��ʼ��
  
  	// ��ʼ��VBUS����״̬�� 
  	VBusState = 0;
}

// �������ܣ�VBUS�ź�ɨ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע���ú�������ʱ�жϵ��ã�������20mS
void VBUS_DEV_Scan()
{
	// ����VBUS״̬���ĵ�ǰ״̬���Ե�ǰ��VBUS������д���
	switch(VBusState)
	{
		case 0:
		  	// ��״̬0�£���VBUS�е磬��ת��״̬1�����򱣳�״̬0
		  	if(VBUS_GPIO_GetValue() == VBUS_ON)
		  	{
				VBusState = 1;
		  	}
		  	break;
		case 1:
		  	// ��״̬1�£���VBUS���е磬��˵�������ȶ�������״̬2
		  	// ����˵���ϴ��е���Ч���ص�״̬0
		  	if(VBUS_GPIO_GetValue() == VBUS_ON)
		  	{
				VBusState = 2;
		  	}
		  	else
		  	{ 
				VBusState = 0;
		  	}
		  	break;
		case 2:
		  	// ��״̬2�£��ȴ�VBUS�޵磬���޵磬����״̬3
		  	// ���򱣳���״̬2
		  	if(VBUS_GPIO_GetValue() == VBUS_OFF)
		  	{
				VBusState = 3;
		  	}
		  	break;
		case 3:
		  	// ��״̬3�£���VBUSû�磬��˵�������һ����Ч�Ĺ��絽�ϵ����
		  	// �ص�״̬0
		  	// ����˵���޵���Ч���ص�״̬2
		  	if(VBUS_GPIO_GetValue() == VBUS_OFF)
		  	{
				// �����һ����Ч���絽�ϵ�Ĺ���
				VBusState = 0;
		  	}
		  	else
		  	{ 
				// ���VBUS�ϵ���Ч������״̬2
				VBusState = 2;
		  	}
		  	break;
	}
}

// 3.Ӧ�ò�

// �������ܣ���ʼ��VBUS���ģ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void VBUS_API_Init()
{
  	VBUS_DEV_Init();                       // �����豸���ʼ��
}

// �������ܣ���鵱ǰ״̬��
// �����������
// �����������
// �� �� ֵ��VBUS_ON    -- VBUS�е�
//           VBUS_OFF   -- VBUSû��
// ��    ע����
uint32 VBUS_API_GetVBus()
{
  	uint32 temp;

  	// ��ֹ�ж�
  	__disable_irq();
  
  	temp = VBusState;

  	// �����ж�
  	__enable_irq();

  	if((temp == 2) || (temp == 3))
  	{
    	// VBUS״̬������״̬2��3��˵���Ѿ������ȶ��Ĺ���״̬
    	return USB_ON;
  	}
  	else
  	{
    	// ����VBUS״̬�����ڲ��ȶ��Ĺ���״̬
    	return USB_OFF;
  	}
}

/****************************added by liubingyan*******************************/

// �������ܣ�ʹ��VBUS ON ����ж�
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void VBUS_enableVBUSON(void)
{
/************************configure pin***************************/    
 	GPIO_InitTypeDef  GPIO_InitStructure;
  
  	RCC_AHBPeriphClockCmd(VBUS_CTRL, ENABLE);

  	GPIO_InitStructure.GPIO_Pin   	= VBUS_BIT;
  	GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  	= GPIO_PuPd_NOPULL;
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_400KHz;
  	GPIO_Init(VBUS_PORT, &GPIO_InitStructure);
 
/**************configure interrupt function**********************/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	EXTI_InitTypeDef extiInit;
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	  
	extiInit.EXTI_Line    	= EXTI_Line0;
	extiInit.EXTI_Mode    	= EXTI_Mode_Interrupt;
	extiInit.EXTI_Trigger 	= EXTI_Trigger_Rising;
	extiInit.EXTI_LineCmd 	= ENABLE;                           
	EXTI_Init(&extiInit);

	NVIC_InitTypeDef nvicInit;
	nvicInit.NVIC_IRQChannel                   	= EXTI0_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority 	= PRI_MCU_VBUS;
	nvicInit.NVIC_IRQChannelSubPriority        	= 0;
	nvicInit.NVIC_IRQChannelCmd                	= ENABLE;
	NVIC_Init(&nvicInit);
}

// �������ܣ���ֹVBUS ON ����ж�
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void VBUS_disableVBUSON(void)
{
	EXTI_InitTypeDef extiInit;
  
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	  
	extiInit.EXTI_Line    = EXTI_Line0;          //ѡ���ⲿ�ж�0
	extiInit.EXTI_Mode    = EXTI_Mode_Interrupt; //�ж�ģʽ
	extiInit.EXTI_Trigger = EXTI_Trigger_Rising; //ѡ�����������
	extiInit.EXTI_LineCmd = DISABLE;                           
	EXTI_Init(&extiInit);

	NVIC_InitTypeDef nvicInit;
	nvicInit.NVIC_IRQChannel                   = EXTI0_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority = PRI_MCU_VBUS;
	nvicInit.NVIC_IRQChannelSubPriority        = 0;
	nvicInit.NVIC_IRQChannelCmd                = DISABLE;
	NVIC_Init(&nvicInit);
}

/******************************************************************/
/*ʹ��USB_D- ��USB_D+ ������									  */
/******************************************************************/
void VBUS_enableCable(void)
{
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitTypeDef gpioInit;

	//�ر�GPIOA
	gpioInit.GPIO_Pin 	= GPIO_Pin_11 | GPIO_Pin_12;
	gpioInit.GPIO_Speed = GPIO_Speed_400KHz;
	gpioInit.GPIO_Mode 	= GPIO_Mode_IN;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &gpioInit);
}

// �������ܣ�VBUS�жϴ�����
// �����������
// �����������
// �������أ���
// ��    ע���ú�������ϵͳ����ͣ��״̬ʱ�Ż����������ڻ���CPU
void EXTI0_IRQHandler()
{
  	if(Event_getCurrentMode() == TASK_STOP && Event_getEvent() == NO_EVENT)
	{
		Event_register(USB_VBUSON);
	}

  	// ����ⲿ�ж�0�Ĺ���Ĵ����е��ж������־
  	EXTI_ClearFlag(EXTI_Line0);
	EXTI_ClearITPendingBit(EXTI_Line0);
}