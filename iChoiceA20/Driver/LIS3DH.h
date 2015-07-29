#ifndef	__LIS3DH_H
#define	__LIS3DH_H

#include "type.h"

#define		LIS3DH_ID			0X33

#define		FIFO_LENGTH			10

#define     CLICK_INTERVAL		400

/*LIS3DH 平均无故障时间*/
#define LIS3DH_MTBF		250

/*REGISTER ADDRESS*/
#define		STATUS_REG_AUX		0X07
#define		OUT_ADC1_L			0X08
#define		OUT_ADC1_H			0X09
#define		OUT_ADC2_L			0X0A
#define		OUT_ADC2_H			0X0B
#define		OUT_ADC3_L			0X0C
#define		OUT_ADC3_H			0X0D
#define		INT_COUNTER_REG		0X0E
#define		WHO_AM_I			0X0F
#define		TEMP_CFG_REG		0X1F
#define		CTRL_REG1			0X20
#define		CTRL_REG2			0X21
#define		CTRL_REG3			0X22
#define		CTRL_REG4			0X23
#define		CTRL_REG5			0X24
#define		CTRL_REG6			0X25

#define		REFERENCE			0X26
#define		STATUS_REG2			0X27

#define		OUT_X_L				0X28
#define		OUT_X_H				0X29
#define		OUT_Y_L				0X2A
#define		OUT_Y_H				0X2B
#define		OUT_Z_L				0X2C
#define		OUT_Z_H				0X2D

#define		FIFO_CTRL_REG		0X2E
#define		FIFO_SRC_REG		0X2F

#define		INT1_CFG			0X30
#define		INT1_SOURCE			0X31
#define		INT1_THS			0X32
#define		INT1_DURATION		0X33

#define		CLICK_CFG			0X38
#define		CLICK_SRC			0X39
#define		CLICK_THS			0X3A

#define		TIME_LIMIT			0X3B
#define		TIME_LATENCY		0X3C
#define		TIME_WINDOE			0X3D

#define		DATA_NOT_READY		0
#define 	DATA_IS_READY		1

#define		CLICK_INVALID		0
#define 	CLICK_VALID			1


/*引脚		*/
#define	LIS3DH_CS	GPIO_Pin_2
#define	LIS3DH_INT1	GPIO_Pin_3
#define	LIS3DH_INT2	GPIO_Pin_4

#define	LIS3DH_SCK	GPIO_Pin_5
#define	LIS3DH_MISO	GPIO_Pin_6
#define	LIS3DH_MOSI	GPIO_Pin_7

#define LIS3DH_PORT			GPIOA

#define LIS3DH_PORT_CLK		RCC_AHBPeriph_GPIOA

#define LIS3DH_PORT_EXIT	EXTI_PortSourceGPIOA

#define	LIS3DH_CS_H()		GPIO_SetBits(LIS3DH_PORT,LIS3DH_CS)
#define	LIS3DH_CS_L()		GPIO_ResetBits(LIS3DH_PORT,LIS3DH_CS)

/*data struct*/
typedef struct
{	
	int8 x;
	
	int8 y;
	
	int8 z;
	
}LIS3DH_DATA;

uint16 	LIS3DSH_getAccData(uint8 index);
ErrorStatus 	LIS3DH_init(void);
void 	LIS3DH_powerdown_mode(void);
void 	LIS3DH_FIFO_mode(void);
void 	LIS3DH_Motion_mode(void);
uint8 	LIS3DH_getDataStatus(void);
void 	LIS3DH_feed(void);
uint64 	LIS3DH_getRunTimer(void);
uint8 	LIS3DH_getClickState(void);
void LIS3DSH_getAxisData(LIS3DH_DATA *data);
void 	LIS3DH_openSPI(void);
void 	LIS3DH_closeSPI(void);

#endif

