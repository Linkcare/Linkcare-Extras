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

/*	锂电池电压范围是3.6~4.2V								*/
/*	电量高--4.0 V--电量中--3.8 V--电量低--3.6 V--电量危险	*/

#define VOLTAGE_THRESHOLD_1  	452	//4000 mv
#define VOLTAGE_THRESHOLD_2  	429 //3800 mv
#define VOLTAGE_THRESHOLD_3  	407	//3600 mv

/*电压范围*/
#define UPPER_LINIT		486		//4300 mv,电池电压有效范围上限
#define LOWER_LINIT		282		//2500 mv,电池电压有效范围下限

//电池电量水平
uint8 batteryLevel = BATTERY_HIGH;

// 函数功能：ADC采样端口初始化
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
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

// 函数功能：ADC模块初始化
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
static void initADC(void)
{
	// 1.配置复位
	ADC_DeInit(ADC1);

	// 2.开启内部高速振荡器，要进行ADC，必须开启这个振荡器（频率为16MHZ）
	RCC_HSICmd(ENABLE);
	// 等待HSI稳定
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
	  	__NOP();

	// 3.设置ADC模块的时钟分频为4，相当与运行在4MHZ
	//   此时ADC为低速工作模式
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInit(&ADC_CommonInitStructure);                  

	// 4.开启ADC模块的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	// 5.配置ADC模块  
	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);
	// ADC分辨率为12位
	ADC_InitStructure.ADC_Resolution           = ADC_Resolution_12b;             
	// 禁止扫描
	ADC_InitStructure.ADC_ScanConvMode         = DISABLE;                        
	// 禁止连续转换 
	ADC_InitStructure.ADC_ContinuousConvMode   = DISABLE;                        
	// 不选择外部触发沿
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  
	// 数据靠右对齐
	ADC_InitStructure.ADC_DataAlign            = ADC_DataAlign_Right;            
	// 转换的通道数为1 
	ADC_InitStructure.ADC_NbrOfConversion      = 1;                              
	ADC_Init(ADC1, &ADC_InitStructure);      

	// 配置ADC1的规则通道：第1个采样通道为1，采样周期为 ADC_SampleTime * 2.5 us
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_96Cycles);
	
	// Enables the ADC1 Power Down during Delay 
	ADC_PowerDownCmd(ADC1, ADC_PowerDown_Idle_Delay, ENABLE);
	  
	// Delay until the converted data has been read
	ADC_DelaySelectionConfig(ADC1, ADC_DelayLength_Freeze);
	
	PWR_FastWakeUpCmd(ENABLE);	
	
	// 允许ADC1工作
	ADC_Cmd(ADC1, ENABLE);     
	  
	// 等待ADC1模块准备好
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	  	__NOP();
	
	while(PWR_GetFlagStatus(PWR_FLAG_VREFINTRDY) == RESET)
	  	__NOP();
}

// 函数功能：开启电池检测模块
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
static void Battery_enable(void)
{
    initPORT();
    initADC();
}

// 函数功能：关闭电池检测模块
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Battery_disable(void)
{
    ADC_DeInit(ADC1);
    ADC_Cmd(ADC1,DISABLE);

	//关闭内部高速振荡器
    RCC_HSICmd(DISABLE);
}

// 函数功能：ADC数据转换
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：采样值
static uint16 convertBatteryVoltage(void)
{
	//启动AD转换
    ADC_SoftwareStartConv(ADC1);
	
	//等待完成
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
        __NOP();
	
	//返回采样值
    return ADC1->DR;
}

/***********************************************
* 功能：计算当前电池电压级别
***********************************************/
static void calculateVoltageLevel(void)
{
	uint32 total   = 0;
    uint8  counter = 0;
	uint16 sampleValue = 0;
//	uint16 voltage;

	//连续采集10次
    for(uint8 i = 0;i < 10;i++)
    {
        sampleValue = convertBatteryVoltage();
		
		//过滤出有效的采样
        if(sampleValue > LOWER_LINIT && sampleValue < UPPER_LINIT)
        {
		  	//舍弃第一个采样值（不准确）
		  	if(i > 0)
			{
            	total += sampleValue;
								
            	counter++;
			}
        }
    }
	
	//如果采样值全部无效，默认电池电量为高
    if(counter == 0)
	{		
	  	batteryLevel = BATTERY_HIGH;
	  
		return;
	}
	
	//电压采样平均值
    sampleValue = total / counter;	
	
	//电池电压计算
//	voltage = sampleValue * 3300 * 11 / 4096;
	
	if(sampleValue > VOLTAGE_THRESHOLD_1)
	{
		batteryLevel = BATTERY_HIGH;	//电池电量充足
	}
	else if(sampleValue > VOLTAGE_THRESHOLD_2 && sampleValue < VOLTAGE_THRESHOLD_1)
	{
		batteryLevel = BATTERY_MIDDLE;	//电池电量中等
	}
	else if(sampleValue > VOLTAGE_THRESHOLD_3 && sampleValue < VOLTAGE_THRESHOLD_2)
	{
		batteryLevel = BATTERY_LOW;		//电池电量较低
	}
	else
	{
		batteryLevel = BATTERY_DANGER;	//电池电池不足
	}
}


// 函数功能：电池电量检测
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Battery_check(void)
{
	Battery_enable();
	calculateVoltageLevel();
	Battery_disable();
}

// 函数功能：定期检测电池电量
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
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
* 功能：获取电池电量等级
***********************************************/
uint8 Battery_getBatteryLevel(void)
{
	return  batteryLevel;     
}

// 函数功能：打开充电检测端口
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
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

// 函数功能：关闭充电检测端口
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
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

// 函数功能：获取电池充电状态
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
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