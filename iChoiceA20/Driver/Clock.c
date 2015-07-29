#include "stm32l1xx.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_syscfg.h"
#include "stm32l1xx_rcc.h"

// 函数功能：配置系统时钟源
// 输入参数：RCC_SYSCLKSource
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Clock_configSYSCLK(uint32_t RCC_SYSCLKSource)
{
    uint32_t clockSourceFlag = RCC_CFGR_SWS_MSI;
    if(RCC_SYSCLKSource == RCC_SYSCLKSource_HSE)
        clockSourceFlag = RCC_CFGR_SWS_HSE;
    if(RCC_SYSCLKSource == RCC_SYSCLKSource_PLLCLK)
        clockSourceFlag = RCC_CFGR_SWS_PLL;
    if(RCC_SYSCLKSource == RCC_SYSCLKSource_HSI)
        clockSourceFlag = RCC_CFGR_SWS_HSI;
	
	RCC_SYSCLKConfig(RCC_SYSCLKSource);
	while(RCC_GetSYSCLKSource() != (uint8_t)clockSourceFlag) 
		__NOP();
}

// 函数功能：USB模式时钟初始化
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Clock_TaskUSB(void)
{
  	/*系统时钟初始化*/
	RCC_DeInit();
	
	//注：USB模式下，主频为32MHZ，内核电压必须为PWR_VoltageScaling_Range1
	PWR_VoltageScalingConfig(PWR_VoltageScaling_Range1);
    while(PWR_GetFlagStatus(PWR_FLAG_VOS) != RESET)
	  	__NOP();
	
	/*使能低速外设时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/*启动外部高速晶振*/
    RCC_HSEConfig(RCC_HSE_ON);
    while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
	  	__NOP();
	
	/*锁相环配置*/
    RCC_PLLCmd(DISABLE);
    RCC_PLLConfig(RCC_PLLSource_HSE,RCC_PLLMul_8,RCC_PLLDiv_3);
    RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	  	__NOP();
	
	Clock_configSYSCLK(RCC_SYSCLKSource_PLLCLK);
	
    /* HCLK = SYSCLK /1*/
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
	
    /* PCLK1 = HCLK /1*/
    RCC_PCLK1Config(RCC_HCLK_Div1);
	
    /* PCLK2 = HCLK /1*/
    RCC_PCLK2Config(RCC_HCLK_Div1);
	
}

// 函数功能：记步&睡眠&关机模式时钟初始化
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Clock_init(void)
{
    RCC_DeInit();

    PWR_VoltageScalingConfig(PWR_VoltageScaling_Range2);
    while(PWR_GetFlagStatus(PWR_FLAG_VOS) != RESET)
	  	__NOP();
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/*MSI clock is around 4.194 MHz*/
    RCC_MSIRangeConfig(RCC_MSIRange_6); 
    while (RCC_GetFlagStatus(RCC_FLAG_MSIRDY) == RESET)
	  	__NOP();

	/*MSI作为系统时钟*/
    Clock_configSYSCLK(RCC_SYSCLKSource_MSI);
	
    /*Configures the AHB clock,HCLK = SYSCLK /1*/
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    /*Configures the Low Speed APB clock,PCLK2 = HCLK /1*/
    RCC_PCLK1Config(RCC_HCLK_Div1);
    /*Configures the Low Speed APB clock,PCLK1 = HCLK /1*/
    RCC_PCLK2Config(RCC_HCLK_Div1);
}
