/*********************************************/
/*modified by liubingyan*/
/*********************************************/
#include "Battery.h"
#include "Event.h"
#include "task.h"
#include "RTC.h"
#include "Clock.h"
#include "stm32l1xx.h"
#include "System.h"
#include "DeviceInfo.h"

/*	﮵�ص�ѹ��Χ��3.6~4.2V								*/
/*	������--4.0 V--������--3.8 V--������--3.6 V--����Σ��	*/

#define VOLTAGE_THRESHOLD_1  	452	//4000 mv
#define VOLTAGE_THRESHOLD_2  	429 //3800 mv
#define VOLTAGE_THRESHOLD_3  	407	//3600 mv

/*��ѹ��Χ*/
#define UPPER_LINIT		486		//4300 mv,��ص�ѹ��Ч��Χ����
#define LOWER_LINIT		282		//2500 mv,��ص�ѹ��Ч��Χ����

//��ص���ˮƽ
uint8 batteryLevel = BATTERY_HIGH;

// �������ܣ�ADC�����˿ڳ�ʼ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
static void initPORT(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// �������ܣ�ADCģ���ʼ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
static void initADC(void)
{
	// 1.���ø�λ
	ADC_DeInit(ADC1);

	// 2.�����ڲ�����������Ҫ����ADC�����뿪�����������Ƶ��Ϊ16MHZ��
	RCC_HSICmd(ENABLE);
	// �ȴ�HSI�ȶ�
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
	  	__NOP();

	// 3.����ADCģ���ʱ�ӷ�ƵΪ4���൱��������4MHZ
	//   ��ʱADCΪ���ٹ���ģʽ
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInit(&ADC_CommonInitStructure);                  

	// 4.����ADCģ���ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	// 5.����ADCģ��  
	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);
	// ADC�ֱ���Ϊ12λ
	ADC_InitStructure.ADC_Resolution           = ADC_Resolution_12b;             
	// ��ֹɨ��
	ADC_InitStructure.ADC_ScanConvMode         = DISABLE;                        
	// ��ֹ����ת�� 
	ADC_InitStructure.ADC_ContinuousConvMode   = DISABLE;                        
	// ��ѡ���ⲿ������
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  
	// ���ݿ��Ҷ���
	ADC_InitStructure.ADC_DataAlign            = ADC_DataAlign_Right;            
	// ת����ͨ����Ϊ1 
	ADC_InitStructure.ADC_NbrOfConversion      = 1;                              
	ADC_Init(ADC1, &ADC_InitStructure);      

	// ����ADC1�Ĺ���ͨ������1������ͨ��Ϊ1����������Ϊ ADC_SampleTime * 2.5 us
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_96Cycles);
	
	// Enables the ADC1 Power Down during Delay 
	ADC_PowerDownCmd(ADC1, ADC_PowerDown_Idle_Delay, ENABLE);
	  
	// Delay until the converted data has been read
	ADC_DelaySelectionConfig(ADC1, ADC_DelayLength_Freeze);
	
	PWR_FastWakeUpCmd(ENABLE);	
	
	// ����ADC1����
	ADC_Cmd(ADC1, ENABLE);     
	  
	// �ȴ�ADC1ģ��׼����
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	  	__NOP();
	
	while(PWR_GetFlagStatus(PWR_FLAG_VREFINTRDY) == RESET)
	  	__NOP();
}

// �������ܣ�������ؼ��ģ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
static void Battery_enable(void)
{
    initPORT();
    initADC();
}

// �������ܣ��رյ�ؼ��ģ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void Battery_disable(void)
{
    ADC_DeInit(ADC1);
    ADC_Cmd(ADC1,DISABLE);

	//�ر��ڲ���������
    RCC_HSICmd(DISABLE);
}

// �������ܣ�ADC����ת��
// �����������
// �����������
// �� �� ֵ����
// ��    ע������ֵ
static uint16 convertBatteryVoltage(void)
{
	//����ADת��
    ADC_SoftwareStartConv(ADC1);
	
	//�ȴ����
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
        __NOP();
	
	//���ز���ֵ
    return ADC1->DR;
}

/***********************************************
* ���ܣ����㵱ǰ��ص�ѹ����
***********************************************/
static void calculateVoltageLevel(void)
{
	uint32 total   = 0;
    uint8  counter = 0;
	uint16 sampleValue = 0;
//	uint16 voltage;

	//�����ɼ�10��
    for(uint8 i = 0;i < 10;i++)
    {
        sampleValue = convertBatteryVoltage();
		
		//���˳���Ч�Ĳ���
        if(sampleValue > LOWER_LINIT && sampleValue < UPPER_LINIT)
        {
		  	//������һ������ֵ����׼ȷ��
		  	if(i > 0)
			{
            	total += sampleValue;
								
            	counter++;
			}
        }
    }
	
	//�������ֵȫ����Ч��Ĭ�ϵ�ص���Ϊ��
    if(counter == 0)
	{		
	  	batteryLevel = BATTERY_HIGH;
	  
		return;
	}
	
	//��ѹ����ƽ��ֵ
    sampleValue = total / counter;	
	
	//��ص�ѹ����
//	voltage = sampleValue * 3300 * 11 / 4096;
	
	if(sampleValue > VOLTAGE_THRESHOLD_1)
	{
		batteryLevel = BATTERY_HIGH;	//��ص�������
	}
	else if(sampleValue > VOLTAGE_THRESHOLD_2 && sampleValue < VOLTAGE_THRESHOLD_1)
	{
		batteryLevel = BATTERY_MIDDLE;	//��ص����е�
	}
	else if(sampleValue > VOLTAGE_THRESHOLD_3 && sampleValue < VOLTAGE_THRESHOLD_2)
	{
		batteryLevel = BATTERY_LOW;		//��ص����ϵ�
	}
	else
	{
		batteryLevel = BATTERY_DANGER;	//��ص�ز���
	}
}


// �������ܣ���ص������
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void Battery_check(void)
{
	Battery_enable();
	calculateVoltageLevel();
	Battery_disable();
}

// �������ܣ����ڼ���ص���
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void Battery_update(void)
{
  	static uint64 battery_timer = 0;
       
	if(RTC_checkTimeout(battery_timer,BATTERY_CHECK) == IS_TIMEOUT)	
	{
	  	RTC_obtainMilliSeconds(&battery_timer);

		Battery_check();
	}
}
/***********************************************
* ���ܣ���ȡ��ص����ȼ�
***********************************************/
uint8 Battery_getBatteryLevel(void)
{
	return  batteryLevel;     
}

// �������ܣ��򿪳����˿�
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void Battery_openChargePort(void)
{
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
	GPIO_InitTypeDef gpioInit;
    gpioInit.GPIO_Pin 	= GPIO_Pin_9;
    gpioInit.GPIO_Speed = GPIO_Speed_400KHz;
    gpioInit.GPIO_Mode	= GPIO_Mode_IN;
    gpioInit.GPIO_OType = GPIO_OType_PP;
    gpioInit.GPIO_PuPd 	= GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gpioInit);
}

// �������ܣ��رճ����˿�
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void Battery_closeChargePort(void)
{
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
	GPIO_InitTypeDef gpioInit;
    gpioInit.GPIO_Pin 	= GPIO_Pin_9;
    gpioInit.GPIO_Speed = GPIO_Speed_400KHz;
    gpioInit.GPIO_Mode 	= GPIO_Mode_OUT;
    gpioInit.GPIO_OType = GPIO_OType_OD;
    gpioInit.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gpioInit);
}

// �������ܣ���ȡ��س��״̬
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
uint8 Battery_isCharging(void)
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9))
	{
		return BATTERY_IS_FULL;
	}
	else
	{
		return BATTERY_IS_CHARGING;
	}
}