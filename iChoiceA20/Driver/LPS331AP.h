#ifndef	__LPS331AP_H
#define	__LPS331AP_H

#include "type.h"

typedef struct
{
	int16 	altitude;
	int16 	temperature;
	uint32  airPressure;

}LPS331AP_DATA;


typedef struct
{
	uint16 TSL;
	uint16 TSH;
	uint32 TCV1;
	uint32 TCV2;
	uint32 TCV3;
	uint32 TCS1;
	uint32 TCS2;
	uint32 TCS3;
	uint32 digGain;
	int32  deltaT1;
	int32  deltaT2;
	int32  deltaT3;
	
}Calibration;

/*GENERAL INFORMATION		*/
#define		LPS331AP_ID					0xBB

#define		PRESSURE_DATA_READY			BIT1
#define		TEMPERATURE_DATA_READY		BIT0

/*register address*/
#define		REF_P_XL			0X08
#define		REF_P_L				0X09
#define		REF_P_H				0X0A
#define		WHO_AM_I			0X0F
#define		RES_CONF			0X10

#define		CTRL_REG1			0X20
#define		CTRL_REG2			0X21
#define		CTRL_REG3			0X22

#define		INT_CFG_REG			0X23
#define		INT_SOURCE_REG		0X24

#define		THS_P_LOW_REG		0X25
#define		THS_P_HIGH_REG		0X26

#define		STATUS_REG			0X27

#define		PRESS_OUT_XL		0X28
#define		PRESS_OUT_L			0X29
#define		PRESS_OUT_H			0X2A

#define		TEMP_OUT_L			0X2B
#define		TEMP_OUT_H			0X2C

#define		AMP_CTRL			0X30

/**************************************************/
#define		COMPENS_L			0X30
#define		DGAIN_L				0x18
#define		DELTA_T1			0x3B
#define		DELTA_T2T3			0x3F
#define		CALIB_SETUP			0x1E
/**************************************************/
/*初始化传感器									  */
/**************************************************/
ErrorStatus LPS331AP_init(void);

/**************************************************/
/*打开传感器									  */
/**************************************************/
void LPS331AP_open(void);

/**************************************************/
/*获取传感器数据								  */
/**************************************************/
void LPS331AP_getData(LPS331AP_DATA *getLPS331AP_DATA);

/**************************************************/
/*读取传感器数据								  */
/**************************************************/
void LPS331AP_readData(void);

/********************************************************************
* 功能: 配置SPI管脚
********************************************************************/
void LPS331AP_openSPI(void);

/********************************************************************
* 功能: 复位SPI管脚
********************************************************************/
void LPS331AP_closeSPI(void);

/**************************************************/
/*关闭传感器									  */
/**************************************************/
void LPS331AP_close(void);


#endif