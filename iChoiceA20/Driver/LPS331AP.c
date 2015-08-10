#include "LPS331AP.h"
#include <math.h>
#include "IIR.h"
#include "stm32l1xx.h"


#define DUTY	5		//气压传感器的启动周期
						//周期 = DUTY * 200 ms
/*引脚		*/
#define	LPS331AP_MOSI	GPIO_Pin_0
#define	LPS331AP_MISO	GPIO_Pin_1
#define	LPS331AP_CS		GPIO_Pin_2
#define	LPS331AP_SCK	GPIO_Pin_12

#define LPS331AP_PORT	GPIOB

#define	SPI_CHIP_SELECT_L()	(LPS331AP_PORT->BSRRH = LPS331AP_CS)
#define	SPI_CHIP_SELECT_H()	(LPS331AP_PORT->BSRRL = LPS331AP_CS)

#define	SPI_SCK_L()			(LPS331AP_PORT->BSRRH = LPS331AP_SCK)
#define	SPI_SCK_H()			(LPS331AP_PORT->BSRRL = LPS331AP_SCK)

#define	SPI_SEND_0()		(LPS331AP_PORT->BSRRH = LPS331AP_MISO)
#define	SPI_SEND_1()		(LPS331AP_PORT->BSRRL = LPS331AP_MISO)

#define	SPI_READ_BIT()		GPIO_ReadInputDataBit(LPS331AP_PORT,LPS331AP_MOSI)

static uint8 cycle = 0;

static uint32 pressure_ad 	= 0;
static int16 temperature_ad = 0;
static volatile LPS331AP_DATA environment_data;

//温度补偿参数
Calibration calibration;

#define 	T1_0  	-15600
#define 	T2_0  	0
#define 	T3_0 	13200
#define 	P0 		1000

//中间变量
uint8 compens_data[10];
uint8 gain_data[3];
int8  dT1;
int8  dT23;
int8  calSetup;

struct
{
	float P1;
	float P2;
	float P3;
	float a;
	float b;
	float c;
	float e1;
	float d1;
	float e2;
	float d2;
	float T1;
	float T2;
	float T3;
	float d;
	float den;
	float Det;
	float T1d;
	float T2d;
	float T3d;
	
}coefficient;

/********************************************************************
* 功能: 配置SPI管脚
********************************************************************/
void LPS331AP_openSPI(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  
  	GPIO_InitStructure.GPIO_Pin   = LPS331AP_MOSI | LPS331AP_SCK | LPS331AP_CS;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  	GPIO_Init(LPS331AP_PORT, &GPIO_InitStructure);  
  
  	GPIO_InitStructure.GPIO_Pin   =  LPS331AP_MISO;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  	GPIO_Init(LPS331AP_PORT, &GPIO_InitStructure);   
}
/********************************************************************
* 功能: 复位SPI管脚
********************************************************************/
void LPS331AP_closeSPI(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
  	GPIO_InitStructure.GPIO_Pin   = LPS331AP_MOSI | LPS331AP_SCK | LPS331AP_CS | LPS331AP_MISO;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
  	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  	GPIO_Init(LPS331AP_PORT, &GPIO_InitStructure);
	
	GPIO_WriteBit(LPS331AP_PORT,LPS331AP_CS  ,Bit_SET);
	GPIO_WriteBit(LPS331AP_PORT,LPS331AP_MOSI,Bit_RESET);
	GPIO_WriteBit(LPS331AP_PORT,LPS331AP_MISO,Bit_RESET);
	GPIO_WriteBit(LPS331AP_PORT,LPS331AP_SCK ,Bit_RESET);
}

/*************************************************
*延时函数										 *
*************************************************/
static void delay(void)
{
	for(uint8 i = 0;i < 20;i ++)
	{
		__NOP();
		__NOP();
	}
}

/*************************************************
*write coefficient.a byte									 *
*************************************************/
static void spi_write_byte(uint8_t data)
{
	for(uint8 i = 0; i < 8; i ++)
	{
		SPI_SCK_L();
        __NOP();
		if(data & 0x80)
			GPIO_WriteBit(LPS331AP_PORT,LPS331AP_MOSI,Bit_SET);
		else
			GPIO_WriteBit(LPS331AP_PORT,LPS331AP_MOSI,Bit_RESET);
        __NOP();
		data <<= 1;
		SPI_SCK_H();
        __NOP();
	}
}

/*************************************************
*read coefficient.a byte									 *
*************************************************/
static uint8_t spi_read_byte(void)
{
	uint8_t rd = 0;
	for(uint8_t i = 0; i < 8; i++)
	{
		SPI_SCK_L();
		__NOP();
		SPI_SCK_H();
		__NOP();
		rd <<= 1;
		if(GPIO_ReadInputDataBit(LPS331AP_PORT, LPS331AP_MISO))
			rd |= 1;
	}
	return rd;
}

/*************************************************
*Read readRegister								 *
*************************************************/
static uint8 readRegister(uint8 addr)
{
	uint8 rd;
	SPI_CHIP_SELECT_L();
	delay();
	
	spi_write_byte(addr|0x80);
	rd = spi_read_byte();
	
	delay();
	SPI_CHIP_SELECT_H();
	
	return rd;  
}
/*************************************************
*Write Register							 	 *
*************************************************/
static void writeRegister(uint8 addr, uint8 ch)
{
	SPI_CHIP_SELECT_L();
	delay();
	
	spi_write_byte(addr);
	spi_write_byte(ch);
	
	delay();	
	SPI_CHIP_SELECT_H();
}

/**************************************************/
/*初始化传感器									  */
/**************************************************/
ErrorStatus LPS331AP_init(void)
{
	writeRegister(CTRL_REG1,BIT7);	//normal mode
	
	if(readRegister(WHO_AM_I) == LPS331AP_ID)
	{
		return SUCCESS;
	}
	else
	{	  
		return ERROR;
	}
}

/**************************************************/
/*关闭传感器									  */
/**************************************************/
void LPS331AP_close(void)
{
	writeRegister(CTRL_REG1,0x00);	//power down
}

/*功耗计算
	Icc =(1uA + 48nA * Pavg) * ODDRP + 32nA * Tavg * ODDRT

	在本应用中，Pavg = 2^4 = 16,Tavg = 2^4 = 16,ODDRP = ODDRP = 1 hz

	Icc =(1uA + 48nA * 16) * 1 + 32nA * 16 * 1 = 2.28uA

	转换时间：T = 62.5 *(Pavg + Tavg) + 1545 us = 3545us
*/
/**************************************************/
/*获取传感器数据								  */
/**************************************************/
void LPS331AP_getData(LPS331AP_DATA *getLPS331AP_DATA)
{
	getLPS331AP_DATA->airPressure = environment_data.airPressure;
	getLPS331AP_DATA->altitude    = environment_data.altitude;
	getLPS331AP_DATA->temperature = environment_data.temperature;
}

/**************************************************/
/*系数计算（1）									  */
/**************************************************/
static void LPS331AP_step1(void)
{
	coefficient.P3 = 0;

	coefficient.P2 = calibration.TCS2*128;
	coefficient.P2 *= (P0-760);
	coefficient.d = calibration.TCV2;
	coefficient.d *= 65536;
	coefficient.P2 += coefficient.d;

	coefficient.T1d = 300 * calibration.deltaT1;
	coefficient.T2d = 300 * calibration.deltaT2;
	coefficient.T3d = 300 * calibration.deltaT3;

	coefficient.T1 = T1_0 + coefficient.T1d;
	coefficient.T2 = T2_0 + coefficient.T2d;
	coefficient.T3 = T3_0 + coefficient.T3d;

	coefficient.d = coefficient.T2-coefficient.T3;
	coefficient.P2 *= coefficient.d;

	coefficient.den = 1048576 + calibration.digGain*32;
	coefficient.P2 /= coefficient.den;
	coefficient.P2 += coefficient.P3;

	coefficient.P1 = calibration.TCS1*128;
	coefficient.P1 *= (P0-760);
	coefficient.d = calibration.TCV1;
	coefficient.d *= 65536;
	coefficient.P1 += coefficient.d;

	coefficient.d = coefficient.T1-coefficient.T2;
	coefficient.P1 *= coefficient.d;

	coefficient.den = 1048576 + calibration.digGain*32;
	coefficient.P1 /= coefficient.den;
	coefficient.P1 += coefficient.P2;
}

/**************************************************/
/*系数计算（2）									  */
/**************************************************/
static void LPS331AP_step2(void)
{
 	coefficient.Det = pow(coefficient.T1,2)*(coefficient.T2 - coefficient.T3) - pow(coefficient.T2,2)*(coefficient.T1-coefficient.T3) + pow(coefficient.T3,2)*(coefficient.T1-coefficient.T2);

	coefficient.a = (coefficient.P1/coefficient.Det)*(coefficient.T2-coefficient.T3)
		+ (-coefficient.P2/coefficient.Det)*(coefficient.T1-coefficient.T3)
		+ (coefficient.P3/coefficient.Det)*(coefficient.T1-coefficient.T2);

	coefficient.b = (-coefficient.P1/coefficient.Det)*(pow(coefficient.T2,2)-pow(coefficient.T3,2))
		+ (coefficient.P2/coefficient.Det)*(pow(coefficient.T1,2)-pow(coefficient.T3,2))
		+ (coefficient.P3/coefficient.Det)*(pow(coefficient.T1,2)-pow(coefficient.T2,2));

	coefficient.c = (coefficient.P1/coefficient.Det) * (coefficient.T3*pow(coefficient.T2,2)-pow(coefficient.T3,2)*coefficient.T2)
		+ (-coefficient.P2/coefficient.Det)*(coefficient.T3*pow(coefficient.T1,2)-pow(coefficient.T3,2)*(coefficient.T1))
		+ (coefficient.P3/coefficient.Det)*(coefficient.T2*pow(coefficient.T1,2)-pow(coefficient.T2,2)*(coefficient.T1));
 
}

/**************************************************/
/*系数计算（3）									  */
/**************************************************/
static void LPS331AP_step3(void)
{
	float Det2 = coefficient.T2 - coefficient.T3;
	coefficient.e2 = (coefficient.P2/Det2)*(1) + (-coefficient.P3/Det2)*(1);
	coefficient.d2 = (-1)*(coefficient.P2/Det2)*(coefficient.T3) + (coefficient.P3/Det2)*(coefficient.T2);

	coefficient.e1 = (coefficient.P1 - coefficient.d2)/coefficient.T1;
	coefficient.d1 = coefficient.d2;
}


float LPS331AP_getPressureCompensationOffSet(float t)
{
	float delta;

	coefficient.d = t * t;
	delta = coefficient.a * coefficient.d;

	if(t<0)
	{
		coefficient.d = coefficient.b-coefficient.e1;
		coefficient.d = coefficient.d * t;
		delta += coefficient.d;
		coefficient.d = coefficient.c - coefficient.d1;
	}
	else
	{
		coefficient.d = coefficient.b-coefficient.e2;
		coefficient.d = coefficient.d * t;
		delta += coefficient.d;
		coefficient.d = coefficient.c - coefficient.d2;
	}
	delta += coefficient.d;
	
	return delta;
}
/**************************************************/
/*读取传感器数据								  */
/**************************************************/
void LPS331AP_readData(void)
{ 
  	if(cycle % DUTY == 0)
	{
	  	float delta;
		float pComp;
		
		cycle = 0;
		
		LPS331AP_openSPI();

		delay();
                          
		pressure_ad  =(int32)readRegister(PRESS_OUT_XL);
		pressure_ad |=((int32)readRegister(PRESS_OUT_L)) << 8;
		pressure_ad |=((int32)readRegister(PRESS_OUT_H)) << 16;

        delay();
		
		temperature_ad  =(uint16)readRegister(TEMP_OUT_L);
		temperature_ad |=((uint16)readRegister(TEMP_OUT_H)) << 8;
		
		//获取补偿参数
		for(uint8 i = 0;i<10;i++)
		{
			compens_data[i] = readRegister(COMPENS_L + i);
		}
		
		for(uint8 i = 0;i<3;i++)
		{
			gain_data[i] = readRegister(DGAIN_L + i);
		}
		
		dT1  = readRegister(DELTA_T1);
		dT23 = readRegister(DELTA_T2T3);
		calSetup = readRegister(CALIB_SETUP);
		
		//计算存数
		calibration.TSL  = (uint16) ((compens_data[0] & 0xFC) >> 2);
		calibration.TSH  = (uint16) (compens_data[1] & 0x3F);

		calibration.TCV1 = (uint32) ((((compens_data[3] & 0x03) << 16) | ((compens_data[2] & 0xFF) << 8) | (compens_data[1] & 0xC0)) >> 6);
		calibration.TCV2 = (uint32) ((((compens_data[4] & 0x3F) << 8) | (compens_data[3] & 0xFC)) >> 2);
		calibration.TCV3 = (uint32) ((((compens_data[6] & 0x03) << 16) | ((compens_data[5] & 0xFF) << 8) | (compens_data[4] & 0xC0)) >> 6);

		calibration.TCS1 = (uint32) ((((compens_data[7] & 0x0F) << 8) | (compens_data[6] & 0xFC)) >> 2);
		calibration.TCS2 = (uint32) ((((compens_data[8] & 0x3F) << 8) | (compens_data[7] & 0xF0)) >> 4);
		calibration.TCS3 = (uint32) ((((compens_data[9] & 0xFF) << 8) | (compens_data[8] & 0xC0)) >> 6);

		calibration.digGain = (uint32) ((((gain_data[2] & 0x0F) << 16) | ((gain_data[1] & 0xFF) << 8) | (gain_data[0] & 0xFC)) >> 2);

		calibration.deltaT1 = (((int8)(dT1 & 0xF0)) >> 4);
		calibration.deltaT2 = (((int8)(dT23 & 0xF0)) >> 4);
		calibration.deltaT3 = (((int8)((dT23 & 0x0F) << 4)) >>4);
		
		LPS331AP_step1();
		LPS331AP_step2();
		LPS331AP_step3();
		
		delta = LPS331AP_getPressureCompensationOffSet(temperature_ad);
		
		pComp = delta + pressure_ad;
		
		pComp = IIR(pComp,1);
	
		//输出测量值
		environment_data.airPressure = ((uint32)pComp) >> 12;	//气压，mbar
		environment_data.temperature = (int16)((temperature_ad/480) + 42.5);	//温度，℃
		environment_data.altitude 	 = (int16)(44307 *(1 - pow(environment_data.airPressure * 0.000987,0.19029496)));	
		
		//环境温度单位: ℃
		//海拔高度单位：米
		
		LPS331AP_close();
		LPS331AP_closeSPI();
	}
	
	cycle ++;
}

/**************************************************/
/*打开传感器									  */
/**************************************************/
void LPS331AP_open(void)
{
  	if(cycle % DUTY == 0)
	{
		LPS331AP_openSPI();
	  
		writeRegister(CTRL_REG1,0x00);	//power down
		writeRegister(RES_CONF ,0x44);	//Pavg = 2^4，Tavg = 2^4
	  
		writeRegister(CTRL_REG1,0x84);	//normal mode，4 spi wire
		writeRegister(CTRL_REG2,BIT0);	//start to sample
		
		LPS331AP_closeSPI();
	}
}
