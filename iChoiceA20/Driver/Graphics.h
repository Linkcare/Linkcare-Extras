/*********************************************************/
/*modified by liubingyan*/
/*********************************************************/
#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include "type.h"

//OLED IIC�ӻ���ַ
#define OLED_ADDR	0x3C

//�������ŷ���
#define OLED_ON		GPIO_Pin_9
#define OLED_RES	GPIO_Pin_8
#define	OLED_PORT	GPIOB

#define Graphics_ENABLE() 	(GPIO_WriteBit(OLED_PORT,OLED_ON,Bit_RESET))
#define Graphics_DISABLE() 	(GPIO_WriteBit(OLED_PORT,OLED_ON,Bit_SET))
#define Graphics_RESET() 	(GPIO_WriteBit(OLED_PORT,OLED_RES,Bit_RESET))
#define Graphics_NORMAL() 	(GPIO_WriteBit(OLED_PORT,OLED_RES,Bit_SET))

//��Ļ���ȣ�column��
#define SCREEN_LENGTH 	128

//��Ļ�߶ȣ�page��
#define SCREEN_PAGE 	4

//���庯��ָ��
typedef void (*pFunc)(void);

//��Ļ����״̬
typedef enum
{
	DISPLAY_ON  = 0x01,	//��Ļ����
	DISPLAY_OFF = 0x00	//��Ļ�ر�

}Graphics_stateDef;

//����ģʽ
typedef enum
{
	ROLL_LEFT 		= 0x26,	//��������
	ROLL_RIGHT 		= 0x27,	//��������
	ROLL_VERTICAL_LEFT 	= 0x29,	//��ֱ��������
	ROLL_VERTICAL_RIGHT = 0x2A	//��ֱ��������

}Graphics_rollMode;


/********************************************************************
* ���ܣ�������Ļ������
* ������value - ����ֵ(1-5)
********************************************************************/
void Graphics_setBrightness(uint8 value);

/********************************************************************
* ���ܣ��ж���Ļ�Ƿ�Ϊ����
********************************************************************/
Graphics_stateDef Graphics_isDisplayOn(void);

/********************************************************************
* ���ܣ��ر���Ļ
* ������isUseHardware - �Ƿ�ʹ��Ӳ���ϵ�
********************************************************************/
void Graphics_displayOff(void);

/********************************************************************
* ���ܣ�����
********************************************************************/
void Graphics_clearScreen(void);

/********************************************************************
* ���ܣ�������Ⱦ��Ļ
********************************************************************/
void Graphics_repaint(void);

/********************************************************************
* ���ܣ��ر���Ļ
* ������isUseHardware - �Ƿ�ʹ��Ӳ���ϵ�
********************************************************************/
void Graphics_clearBuffer(void);

/********************************************************************
* ���ܣ���ʼ����Ļ
********************************************************************/
void Graphics_init(void);

/**************************************************************/
/*��ʾ�ַ�*/
/**************************************************************/
void Graphics_DispChar(uint8 x,uint8 data);

/**************************************************************/
/*��ʾ�ַ���*/
/**************************************************************/
void Graphics_DispString (uint8 x, int8 *buf);

/**************************************************************/
/*��ʾ����*/
/**************************************************************/
void Graphics_DispStringDec (uint8 x,int32 data);

/**************************************************************/
/*��ʾͼƬ*/
/**************************************************************/
void Graphics_DispPic (uint8 x, uint8 k);

/**************************************************************/
/*��ʾ����ͼƬ*/
/**************************************************************/
void Graphics_DispScreen(const uint8* image);

#endif