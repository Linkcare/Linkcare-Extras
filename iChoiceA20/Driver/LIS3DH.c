#include "Event.h"
#include "Task.h"
#include "RTC.h"
#include "System.h"
#include "stm32l1xx.h"
#include "LIS3DH.h"
#include "DeviceInfo.h"

//传感器纠错定时器
static uint64 fault_time = 0;

//加速度临时存储区
static LIS3DH_DATA buffer_acc[FIFO_LENGTH]; 

//传感器FIFO状态
static uint8 LIS3DH_dataStatus = DATA_NOT_READY;

#if	(TAP == ON)
//CLICK信号状态位
static uint8 LIS3DH_clickStatus = CLICK_INVALID;
#endif
/********************************************************************
* 功能:配置中断引脚
********************************************************************/
static void LIS3DH_configIntPin(void)
{
	/************************configure pin***************************/
    GPIO_InitTypeDef gpioInit;
	
  	RCC_AHBPeriphClockCmd(LIS3DH_PORT_CLK,ENABLE);
    
	//开启INIT1中断
    gpioInit.GPIO_Pin 	= LIS3DH_INT1;
    gpioInit.GPIO_Speed = GPIO_Speed_400KHz;
    gpioInit.GPIO_Mode 	= GPIO_Mode_IN;
    gpioInit.GPIO_OType = GPIO_OType_PP;
    gpioInit.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
    GPIO_Init(LIS3DH_PORT, &gpioInit);
    
	//禁止INIT2中断
	gpioInit.GPIO_Pin 	= LIS3DH_INT2;
    gpioInit.GPIO_Speed = GPIO_Speed_400KHz;
    gpioInit.GPIO_Mode 	= GPIO_Mode_IN;
	GPIO_Init(LIS3DH_PORT, &gpioInit);	
	
	/**************configure interrupt function**********************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	EXTI_InitTypeDef extiInit;
	NVIC_InitTypeDef nvicInit;
	
	EXTI_ClearITPendingBit(EXTI_Line3);
	
    SYSCFG_EXTILineConfig(LIS3DH_PORT_EXIT,EXTI_PinSource3);
    
    extiInit.EXTI_Line 		= EXTI_Line3;
    extiInit.EXTI_Mode 		= EXTI_Mode_Interrupt;
    extiInit.EXTI_Trigger 	= EXTI_Trigger_Rising;
    extiInit.EXTI_LineCmd 	= ENABLE;
    EXTI_Init(&extiInit);
	
    nvicInit.NVIC_IRQChannel			 		= EXTI3_IRQn;
    nvicInit.NVIC_IRQChannelPreemptionPriority 	= PRI_MCU_FIFO;
    nvicInit.NVIC_IRQChannelSubPriority 		= 0;
    nvicInit.NVIC_IRQChannelCmd 				= ENABLE;
    NVIC_Init(&nvicInit);

#if	(TAP == ON)
	EXTI_ClearITPendingBit(EXTI_Line4);
	
	SYSCFG_EXTILineConfig(LIS3DH_PORT_EXIT,EXTI_PinSource4);
    extiInit.EXTI_Line 		= EXTI_Line4;
    extiInit.EXTI_Mode 		= EXTI_Mode_Interrupt;
    extiInit.EXTI_Trigger 	= EXTI_Trigger_Rising;
    extiInit.EXTI_LineCmd 	= ENABLE;
    EXTI_Init(&extiInit);

    nvicInit.NVIC_IRQChannel			 		= EXTI4_IRQn;
    nvicInit.NVIC_IRQChannelPreemptionPriority 	= PRI_3DH_TAP;
    nvicInit.NVIC_IRQChannelSubPriority 		= 0;
    nvicInit.NVIC_IRQChannelCmd 				= ENABLE;
    NVIC_Init(&nvicInit);
#endif
}

// 函数功能：打开SPI端口
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void LIS3DH_openSPI(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    SPI_InitTypeDef   SPI_InitStructure;

	//Enable peripheral clock
    RCC_AHBPeriphClockCmd(LIS3DH_PORT_CLK,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	//Peripherals alternate function
	GPIO_PinAFConfig(LIS3DH_PORT, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_PinAFConfig(LIS3DH_PORT, GPIO_PinSource6,GPIO_AF_SPI1);
    GPIO_PinAFConfig(LIS3DH_PORT, GPIO_PinSource7,GPIO_AF_SPI1);

	//Configure the desired pin
    GPIO_InitStructure.GPIO_Pin 	= LIS3DH_CS;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
    GPIO_Init(LIS3DH_PORT, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin 	= LIS3DH_SCK;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  	= GPIO_PuPd_UP;
    GPIO_Init(LIS3DH_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin 	= LIS3DH_MOSI;
    GPIO_Init(LIS3DH_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin 	= LIS3DH_MISO;
    GPIO_Init(LIS3DH_PORT, &GPIO_InitStructure);

	//Configure SPI1
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode 		= SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize 	= SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL 		= SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA 		= SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS 		= SPI_NSS_Soft;
    SPI_InitStructure.SPI_FirstBit 	= SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
	
    LIS3DH_CS_H();
}

// 函数功能：关闭SPI端口
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void LIS3DH_closeSPI(void)
{
	SPI_Cmd(SPI1,DISABLE);
}

// 函数功能：向SPI接口发送一个字节
// 输入参数：byte - 要发送的字节
// 输出参数：无
// 返 回 值：无
// 备    注：无
static void sendByteToSPI(uint8 byte)
{
  	uint16 i = 0;
  
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET && i < 10000) 
	{
		i++;
	}
	
    SPI_I2S_SendData(SPI1,byte);
	
	i = 0;
	
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET && i < 10000) 
	{
		i++;
	}
	
    SPI_I2S_ReceiveData(SPI1);
	
    return;
}

// 函数功能：从SPI接口接收一个字节
// 输入参数：byte - 要发送的字节
// 输出参数：无
// 返 回 值：收到的字节
// 备    注：无
static uint8 readByteFromSPI(void)
{
  	uint16 i = 0;
  
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET && i < 10000) 
	{
		i++;
	}
	
    SPI_I2S_SendData(SPI1,0x00);
	
	i = 0;
	
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET && i < 10000) 
	{
		i++;
	}
	
    return(uint8)SPI_I2S_ReceiveData(SPI1);
}

// 函数功能：读取3DH的寄存器
// 输入参数：Reg -- 待读取的寄存器地址
// 输出参数：无
// 返 回 值：寄存器内容
// 备    注：无
uint8 LIS3DH_ReadReg(uint8 Reg) 
{
  	uint8 data;

    LIS3DH_CS_L();

    sendByteToSPI(0x80|Reg);

    data = readByteFromSPI();

    LIS3DH_CS_H();
	
	return data;
}

// 函数功能：向寄存器中写入数据
// 输入参数：WriteAddr -- 待写入的寄存器地址
//			  Data		-- 待写入数据
// 输出参数：无
// 返 回 值：无
// 备    注：无
void LIS3DH_WriteReg(uint8 WriteAddr, uint8 Data) 
{
    LIS3DH_CS_L();
	
    sendByteToSPI(WriteAddr & 0x3F);
	
    sendByteToSPI(Data);
	
    LIS3DH_CS_H();
}

// 函数功能：从FIFO中批量读取数据
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void burstRead_ISR(void)
{	
    LIS3DH_CS_L();

    sendByteToSPI(0xC0 | OUT_X_L);
	
    for(uint8 i = 0;i < FIFO_LENGTH;i++)
    {
        readByteFromSPI();
        buffer_acc[i].x = readByteFromSPI();

        readByteFromSPI();
        buffer_acc[i].y = readByteFromSPI();

        readByteFromSPI();
        buffer_acc[i].z = readByteFromSPI();
    }

    LIS3DH_CS_H();

    return;
}

// 函数功能：获取合加速度
// 输入参数：index -- 数据序列
// 输出参数：无
// 返 回 值：合加速度
// 备    注：无
uint16 LIS3DSH_getAccData(uint8 index)
{
	uint16 accData = 0;
	uint16 x,y,z;
	
	x =(uint32)(buffer_acc[index].x * buffer_acc[index].x);
	y =(uint32)(buffer_acc[index].y * buffer_acc[index].y);
	z =(uint32)(buffer_acc[index].z * buffer_acc[index].z);
		   
	accData =(uint16)((System_sqrt(x + y + z) << 4) / 25);
	
	return accData;
}

/*************************************************
*Get xyz axis data
*************************************************/
void LIS3DSH_getAxisData(LIS3DH_DATA *data)
{
	data->x =((buffer_acc[0].x + buffer_acc[2].x + buffer_acc[4].x + buffer_acc[6].x + buffer_acc[8].x)<<2)/125;
	data->y =((buffer_acc[0].y + buffer_acc[2].y + buffer_acc[4].y + buffer_acc[6].y + buffer_acc[8].y)<<2)/125;
	data->z =((buffer_acc[0].z + buffer_acc[2].z + buffer_acc[4].z + buffer_acc[6].z + buffer_acc[8].z)<<2)/125;
}

/*************************************************
*Init the sensor								 *
*************************************************/
ErrorStatus LIS3DH_init(void)
{	
	LIS3DH_configIntPin();

    if(LIS3DH_ReadReg(WHO_AM_I) == LIS3DH_ID)
	{
		return SUCCESS;
	}
	else
	{
		return ERROR;
	}
}

// 函数功能：传感器休眠
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void LIS3DH_powerdown_mode(void)
{
	LIS3DH_WriteReg(CTRL_REG1,0x00);
}

// 函数功能：使能传感器FIFO模式
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void LIS3DH_FIFO_mode(void)
{    
  	//Clear the registers used lastly content
  	LIS3DH_WriteReg(CTRL_REG1,0x00);
	LIS3DH_WriteReg(CTRL_REG2,0x00);
	LIS3DH_WriteReg(CTRL_REG3,0x00);
	LIS3DH_WriteReg(CTRL_REG4,0x00);
	LIS3DH_WriteReg(INT1_THS,0x00);
	LIS3DH_WriteReg(INT1_DURATION,0X00);
	LIS3DH_WriteReg(INT1_CFG,0x00);

  	//Configure the common registers used
	LIS3DH_WriteReg(CTRL_REG1,0x4F); 			/*****
													50HZ,low power mode,z、y、z enable
												*****/
	
	LIS3DH_WriteReg(CTRL_REG3,0x04);			/*****
													FIFO watermak interrupt mapped to INT1
												*****/
	
	LIS3DH_WriteReg(CTRL_REG4,0xC8);			/*****
													BDU enable,LSB mode,+/-2G,high resolution enable,4-wire
												*****/
	
	LIS3DH_WriteReg(CTRL_REG5,0x48);			/*****
													FIFO enable
												*****/
	
	LIS3DH_WriteReg(CTRL_REG6,0x80);			/*****
													CLICK interrupt mapped to INT2 and INT active high
												*****/
#if	(TAP == ON)
	//Configure the registers for CLICK
	LIS3DH_WriteReg(CTRL_REG2,0x24); 			/*****
													Normal mode,BIT6~BIT7
													High pass filter cut off frequency(BIT5~BIT4,ODR = 50HZ):
													  	00: 1 HZ
													  	01: 0.5 HZ
													  	10: 0.2 HZ
													  	11: 0.1 HZ
													Enable High Pass Filter for click,BIT2	
												*****/
	LIS3DH_ReadReg(REFERENCE);
	

	LIS3DH_WriteReg(CLICK_CFG,0x10);			/*****
													Enable single CLICK-CLICK on Z-axis
												*****/
	
	LIS3DH_WriteReg(CLICK_THS,0x08);			/*****
													Configure the threshold value,BIT0 ~ BIT6,LSB = full scale / 128
												*****/
	
	LIS3DH_WriteReg(TIME_LIMIT,0x02);			/*****
													Define the maximum time interval that can elapse between the start of the
													click detection procedure(the acceleration on the selected channel exceeds
													the programmed threshold) and when the acceleration goes back below the 
													threshold,LSB = 1 / ODR,BIT0 ~ BIT6
												*****/
	
	LIS3DH_WriteReg(TIME_LATENCY,0x14);			/*****
													Define the time interval that starts after the first click detection where the
													click detection procedure is disabled, in cases where the device is configured 
													for doubleclick detection
													LSB = 1 / ODR,BIT0 ~ BIT7
												*****/
	
	LIS3DH_WriteReg(TIME_WINDOE,0x00);			/*****
													Define the maximum interval of time that can elapse after the end of the latency
													interval in which the click detection procedure can start, in cases where the 
													device is configured for double click detection
													LSB = 1 / ODR,BIT0 ~ BIT7
												*****/
#endif
	//Configure the registers for FIFO
	LIS3DH_WriteReg(FIFO_CTRL_REG,0X00);		/*****
													Bypass mode
												*****/
	
	LIS3DH_WriteReg(FIFO_CTRL_REG,0x40 + FIFO_LENGTH - 1);		/*****
																	FIFO mode,FIFO length = 10
																*****/
}

// 函数功能：使能传感器晃动检测模式
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void LIS3DH_Motion_mode(void)
{	
  	//Clear the registers used lastly content
  	LIS3DH_WriteReg(CTRL_REG1,0X00);
	LIS3DH_WriteReg(CTRL_REG2,0x00);
	LIS3DH_WriteReg(CTRL_REG3,0X00);
	LIS3DH_WriteReg(CTRL_REG4,0X00);
	LIS3DH_WriteReg(CTRL_REG5,0X00);
	LIS3DH_WriteReg(CTRL_REG6,0X00);
	LIS3DH_WriteReg(FIFO_CTRL_REG,0X00);
	LIS3DH_WriteReg(FIFO_CTRL_REG,0X00);
	
#if	(TAP == ON)			
	LIS3DH_WriteReg(CLICK_CFG,0X00);			
	LIS3DH_WriteReg(CLICK_THS,0X00);			
	LIS3DH_WriteReg(TIME_LIMIT,0X00);			
	LIS3DH_WriteReg(TIME_LATENCY,0X00);			
	LIS3DH_WriteReg(TIME_WINDOE,0X00);			
#endif
	
  	//Configure the registers used for SHAKE mode
	LIS3DH_WriteReg(CTRL_REG1,0x3F); 	//10HZ,low power mode,z、y、z enable
	
	LIS3DH_WriteReg(CTRL_REG2,0x00); 	//High_pass filter disable
	
	LIS3DH_WriteReg(CTRL_REG3,0X40);	//Motion interrupt mapped to INT1
	
	LIS3DH_WriteReg(CTRL_REG4,0x00);	//+/-2G

	LIS3DH_WriteReg(INT1_THS,SENSE_DEFINE);	//Threshold value
	
	LIS3DH_WriteReg(INT1_DURATION,0X00);
	
	LIS3DH_WriteReg(INT1_CFG,0XAA);
}

// 函数功能：获取数据缓冲区状态
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
uint8 LIS3DH_getDataStatus(void)
{
    uint8 temp = LIS3DH_dataStatus;

	LIS3DH_dataStatus = DATA_NOT_READY;

	return temp;
}

// 函数功能：读取FIFO中的数据
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void LIS3DH_operate(void)
{
	LIS3DH_openSPI();

	burstRead_ISR();
	   
	LIS3DH_closeSPI();
}

// 函数功能：激活传感器
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void LIS3DH_active(void)
{  
	LIS3DH_openSPI();
	LIS3DH_WriteReg(FIFO_CTRL_REG,0X00);
	LIS3DH_WriteReg(FIFO_CTRL_REG,0x40 + FIFO_LENGTH - 1);
	LIS3DH_closeSPI();
}

// 函数功能：对纠错定时器重新赋值
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void LIS3DH_feed(void)
{
	RTC_obtainMilliSeconds(&fault_time);
}

// 函数功能：获取纠错定时器运行时间
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
uint64 LIS3DH_getRunTimer(void)
{
	return fault_time;
}

// 函数功能：中断服务程序
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void EXTI3_IRQHandler()
{
	if(Event_getCurrentMode() == TASK_STOP)
	{
		Event_register(USER_SHAKE);
	} 
	else if(Event_getCurrentMode() == TASK_TRACKER || Event_getCurrentMode() == TASK_SLEEP)
	{			
		LIS3DH_dataStatus = DATA_IS_READY;
		
        //读取加速度数据
        LIS3DH_operate(); 
	}
	
    EXTI_ClearFlag(EXTI_Line3);
	EXTI_ClearITPendingBit(EXTI_Line3);
}
#if	(TAP == ON)
/***********************************************
*功能：获取加速度传感器CLICK状态位
***********************************************/
uint8 LIS3DH_getClickState(void)
{
  	if(LIS3DH_clickStatus == CLICK_VALID)
	{
		LIS3DH_clickStatus = CLICK_INVALID;
		
		return CLICK_VALID;
	}
	else
	{
		return CLICK_INVALID;
	}
}

/***********************************************
* 功能：中断服务程序————CLICK中断信号
***********************************************/
void EXTI4_IRQHandler()
{	
  	static uint64 timer_click = 0;
	
	if(RTC_checkTimeout(timer_click,CLICK_INTERVAL) == IS_TIMEOUT)
	{
		LIS3DH_clickStatus = CLICK_VALID;
		
		RTC_obtainMilliSeconds(timer_click);
	}
	else
	{
		LIS3DH_clickStatus = CLICK_INVALID;
	}
  	
    EXTI_ClearFlag(EXTI_Line4);
	EXTI_ClearITPendingBit(EXTI_Line4);
}
#endif