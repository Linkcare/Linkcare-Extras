/*********************************************************/
/*modified by liubingyan*/
/*********************************************************/
#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include "type.h"

//OLED IIC从机地址
#define OLED_ADDR	0x3C

//控制引脚分配
#define OLED_ON		GPIO_Pin_9
#define OLED_RES	GPIO_Pin_8
#define	OLED_PORT	GPIOB

#define Graphics_ENABLE() 	(GPIO_WriteBit(OLED_PORT,OLED_ON,Bit_RESET))
#define Graphics_DISABLE() 	(GPIO_WriteBit(OLED_PORT,OLED_ON,Bit_SET))
#define Graphics_RESET() 	(GPIO_WriteBit(OLED_PORT,OLED_RES,Bit_RESET))
#define Graphics_NORMAL() 	(GPIO_WriteBit(OLED_PORT,OLED_RES,Bit_SET))

//屏幕长度（column）
#define SCREEN_LENGTH 	128

//屏幕高度（page）
#define SCREEN_PAGE 	4

//定义函数指针
typedef void (*pFunc)(void);

//屏幕开关状态
typedef enum
{
	DISPLAY_ON  = 0x01,	//屏幕开启
	DISPLAY_OFF = 0x00	//屏幕关闭

}Graphics_stateDef;

//滚动模式
typedef enum
{
	ROLL_LEFT 		= 0x26,	//从左向右
	ROLL_RIGHT 		= 0x27,	//从右向左
	ROLL_VERTICAL_LEFT 	= 0x29,	//垂直从左向右
	ROLL_VERTICAL_RIGHT = 0x2A	//垂直从右向左

}Graphics_rollMode;


/********************************************************************
* 功能：设置屏幕的亮度
* 参数：value - 亮度值(1-5)
********************************************************************/
void Graphics_setBrightness(uint8 value);

/********************************************************************
* 功能：判断屏幕是否为开启
********************************************************************/
Graphics_stateDef Graphics_isDisplayOn(void);

/********************************************************************
* 功能：关闭屏幕
* 参数：isUseHardware - 是否使用硬件断电
********************************************************************/
void Graphics_displayOff(void);

/********************************************************************
* 功能：清屏
********************************************************************/
void Graphics_clearScreen(void);

/********************************************************************
* 功能：重新渲染屏幕
********************************************************************/
void Graphics_repaint(void);

/********************************************************************
* 功能：关闭屏幕
* 参数：isUseHardware - 是否使用硬件断电
********************************************************************/
void Graphics_clearBuffer(void);

/********************************************************************
* 功能：初始化屏幕
********************************************************************/
void Graphics_init(void);

/**************************************************************/
/*显示字符*/
/**************************************************************/
void Graphics_DispChar(uint8 x,uint8 data);

/**************************************************************/
/*显示字符串*/
/**************************************************************/
void Graphics_DispString (uint8 x, int8 *buf);

/**************************************************************/
/*显示数字*/
/**************************************************************/
void Graphics_DispStringDec (uint8 x,int32 data);

/**************************************************************/
/*显示图片*/
/**************************************************************/
void Graphics_DispPic (uint8 x, uint8 k);

/**************************************************************/
/*显示整屏图片*/
/**************************************************************/
void Graphics_DispScreen(const uint8* image);

#endif