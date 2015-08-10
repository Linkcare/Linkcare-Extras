// ----------------------------------------------
// 1.�ļ�˵��������ģ���ʵ��
// ----------------------------------------------

// ----------------------------------------------
// 2.ͷ�ļ�
// ----------------------------------------------
#include "stm32l1xx.h"
#include "string.h"
#include "Event.h"
#include "Task.h"
#include "key.h"
#include "DeviceInfo.h"

// 1. �ӿڲ�

// ��������
// 1�������Ķ˿ں�λ�Ķ���
#define KEY_PORT     GPIOC             // KEY��Ӧ��GPIO�˿�
#define KEY_BIT      GPIO_Pin_13       // KEY��Ӧ��GPIOλ  

#define KeyBufSize   16                // �����������Ĵ�С��������2��N�η�

#define KEY_PRESS    0
#define KEY_NOPRESS  1


// �궨��

// ��������
uint8 KeyHead,KeyTail;          // ������������ͷ��βָ��
uint8 KeyState;                 // ����״̬��
uint8 KeyCounter;               // �������µļ�����
uint8 KeyBuffer[KeyBufSize];    // ����������

uint8 KeyFlag;

// ����ԭ��

// ����ʵ��

// 1.�ӿڲ�ĺ���ʵ��

// �������ܣ������˿ڳ�ʼ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
static void KEY_GPIO_Init(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure;
  
  	// �򿪰���KEY��Ӧ��GPIO�˿ڵ�ʱ�ӣ�����ö˿ڹ���
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  	// ��ʼ������KEY��Ӧ��GPIO�˿�Ϊ������������
  	GPIO_InitStructure.GPIO_Pin 	= KEY_BIT;
  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_400KHz;
 	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}

// �������ܣ����뵱ǰ������״ֵ̬
// �����������
// �����������
// �� �� ֵ��KEY_PRESS   -- ��ʾ��������
//           KEY_NOPRESS -- ��ʾ����̧��
// ��    ע����
uint8 KEY_GPIO_GetValue(void)
{
  	if(GPIO_ReadInputDataBit(KEY_PORT,KEY_BIT) == 0x00)
  	{
		return KEY_PRESS;
  	}
  	else
  	{
		return KEY_NOPRESS;
 	}
}

// 2.�豸��ĺ���ʵ��

// �������ܣ������豸�ĳ�ʼ��
// �����������sss
// �����������
// �� �� ֵ����
// ��    ע����
static void KEY_DEV_Init(void)
{
  	KEY_GPIO_Init();                      // ���ýӿڲ㺯�������GPIO��ʼ��
  
  	// ��ʼ����������״̬����ʱ������� 
  	KeyState = 0;
}

// �������ܣ����浱ǰ��ֵ��������������
// �����������
// �����������
// �� �� ֵ����
// ��    ע������������������
void KEY_DEV_SaveKey(uint32 KeyValue)
{
  	if(((KeyTail+1) & (KeyBufSize - 1)) != KeyHead)
  	{
    	// ��βָ��+1������ͷָ�룬��˵��������δ��
    	KeyBuffer[KeyTail] = KeyValue;
    	KeyTail = (KeyTail + 1) & (KeyBufSize - 1);
  	}
}

// �������ܣ�����ɨ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע���ú�������ʱ�жϵ��ã�������20mS
//           �������֧�ֳ�����
void Key_DEV_ScanKey(void)
{  
	// ���ݰ���״̬���ĵ�ǰ״̬���Ե�ǰ�İ���������д���
	switch(KeyState)
	{
		case 0:
			// ��״̬0�£����������£���ת��״̬1�����򱣳�״̬0
			if(KEY_GPIO_GetValue() == KEY_PRESS)
			{
				KeyState = 1;
			}
			break;
		case 1:
			// ��״̬1�£������������£���˵��������Ч������״̬2������ʼ����
			// ����˵���ϴΰ�����Ч���ص�״̬0
			if(KEY_GPIO_GetValue() == KEY_PRESS)
			{
				KeyState = 2;
				KeyCounter = 0;                  // ��������0����ʼ����
			}
			else
			{ 
				KeyState = 0;
			}
			break;
		case 2:
			// ��״̬2�£��ȴ�����̧����̧�𣬽���״̬3
			// ���򱣳���״̬2
			if(KEY_GPIO_GetValue() == KEY_PRESS)
			{
				// �����£���ʱҪ�ж��Ƿ�Ϊ������
				if(KeyCounter < (LONGKEY_TIMER / RTC_WORK_TIME))
				{
					// ���������С�ڳ������ļ�ʱֵ����������1
					KeyCounter++;
				}
				else
				{
					// ��������˳������ļ�ʱֵ���ͷ���������Ϣ
					if(KeyFlag == 0)
					{
						KEY_DEV_SaveKey(LONG_KEY);
						
						KeyFlag = 1;
					}
				}
			}
			else
			{
				// ������̧�������״̬3
				KeyState = 3;
								
				KeyFlag = 0;
			}
			break;
		case 3:
			// ��״̬3�£�������̧����˵��������ȫ̧�𣬽���״̬0
			// ����˵���ϴΰ���̧����Ч���ص�״̬2
			if(KEY_GPIO_GetValue() == KEY_PRESS)
			{
				// ���������£�˵��ǰ���̧��Ϊ��������ص�״̬2
				KeyState = 2;
			}
			else
			{ 
				// ������̧����Ҫ���ݼ�������ֵ�������Ƿ񷢳�������Ϣ
				if(KeyCounter < (LONGKEY_TIMER / RTC_WORK_TIME))
				{
					// ������������δ�����ֵ���򷢳��̼���Ϣ
					KEY_DEV_SaveKey(SHORT_KEY);
				}
				
				// �ص�״̬0
				KeyState = 0;
			}
			break;
	}
}

// �������ܣ�ʹ�ܰ����ж�
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void KEY_enableInt(void)
{  	
	/************************configure pin***************************/  
  	GPIO_InitTypeDef  GPIO_InitStructure;
  
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  	// ���˿�����Ϊ������������
  	GPIO_InitStructure.GPIO_Pin 	= KEY_BIT;
  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_400KHz;
  	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
	
	/**************configure interrupt function**********************/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	EXTI_InitTypeDef extiInit;
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);
	  
	extiInit.EXTI_Line    	= EXTI_Line13;
	extiInit.EXTI_Mode    	= EXTI_Mode_Interrupt;
	extiInit.EXTI_Trigger 	= EXTI_Trigger_Falling;
	extiInit.EXTI_LineCmd 	= ENABLE;
	EXTI_Init(&extiInit);

	NVIC_InitTypeDef nvicInit;
	nvicInit.NVIC_IRQChannel                   	= EXTI15_10_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority 	= PRI_MCU_KEY;
	nvicInit.NVIC_IRQChannelSubPriority        	= 0;
	nvicInit.NVIC_IRQChannelCmd                	= ENABLE;
	NVIC_Init(&nvicInit);
}

// �������ܣ���ֹ�����ж�
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void KEY_disableInt(void)
{
  	EXTI_InitTypeDef extiInit;
  
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);
	  
	extiInit.EXTI_Line    = EXTI_Line13;          
	extiInit.EXTI_Mode    = EXTI_Mode_Interrupt;  
	extiInit.EXTI_Trigger = EXTI_Trigger_Falling; 
	extiInit.EXTI_LineCmd = DISABLE;                           
	EXTI_Init(&extiInit);

	NVIC_InitTypeDef nvicInit;
	nvicInit.NVIC_IRQChannel                   	= EXTI15_10_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority 	= PRI_MCU_KEY;
	nvicInit.NVIC_IRQChannelSubPriority        	= 0;
	nvicInit.NVIC_IRQChannelCmd                	= DISABLE;
	NVIC_Init(&nvicInit);
}

// 3.Ӧ�ò�
// �������ܣ���ʼ������ģ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void KEY_API_Init(void)
{
  	KEY_DEV_Init();                       // �����豸���ʼ��
  
  	KeyHead = 0;                          // ��ʼ��������������ͷָ��
  	KeyTail = 0;                          // ��ʼ��������������βָ��
	
	for(uint8 i = 0;i < KeyBufSize;i++)
	  	KeyBuffer[i] = 0;
}

// �������ܣ����ذ����������еĵ�ǰ����
// �����������
// �����������
// �� �� ֵ�������������İ�����ֵ
// ��    ע����
uint32 KEY_API_GetKey(void)
{
	uint32 key_head,key_tail;
	uint32 key;
	  
	while(1)
	{
		// ��ֹ�ж�
		__disable_irq();   
		key_head = KeyHead;
		key_tail = KeyTail;
		// �����ж�
		__enable_irq();

		if(key_head != key_tail)
		{
			// ��ͷָ�벻����βָ�룬��˵�������������д��ڰ�����Ҫ����
			key = KeyBuffer[KeyHead];
			KeyHead = (KeyHead + 1) & (KeyBufSize - 1);
			return key;
		}
	}
}

// �������ܣ���鰴�����������Ƿ��м���Ҫ����
// �����������
// �����������
// �� �� ֵ��1 -- �����������д��ڰ�����Ҫ����
//           0 -- ��
// ��    ע����
uint32 KEY_API_CheckKey(void)
{
	uint32 key_head,key_tail;

	// ��ֹ�ж�
	__disable_irq();

	key_head = KeyHead;
	key_tail = KeyTail;

	// �����ж�
	__enable_irq();

	if(key_head != key_tail)
	{
		// ��ͷָ�벻����βָ�룬��˵�������������д��ڰ�����Ҫ����

		return 1;
	}
	else
	{
	  	return 0;
	}
}

// �������ܣ������жϴ�����
// �����������
// �����������
// �������أ���
// ��    ע���ú�������ϵͳ����ͣ��״̬ʱ�Ż����������ڻ���CPU
void EXTI15_10_IRQHandler(void)
{
  	if(Event_getCurrentMode() == TASK_STOP && Event_getEvent() == NO_EVENT)
	{
		Event_register(KEY_PRESSED);
	}

  	EXTI_ClearFlag(EXTI_Line13);
	EXTI_ClearITPendingBit(EXTI_Line13);
}