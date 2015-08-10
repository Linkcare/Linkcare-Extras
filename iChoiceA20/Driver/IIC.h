#ifndef	__IIC_H
#define	__IIC_H

#include "type.h"
/***********************************************
* ���ܣ���ʼ��IICģ��
***********************************************/
void IIC_enable();

/***********************************************
* ���ܣ��ر�IIC�ӿ�
***********************************************/
void IIC_disable();

/***********************************************
* ���ܣ�дһ���ֽڵ�ָ���ļĴ�����
* ������subAddr - �Ĵ�����ַ
*       byteData - �ֽ�����
***********************************************/
void IIC_writeByte(uint8 subAddr, uint8 con, uint8 byteData);

/***********************************************
* ���ܣ�дһ�����ݵ�ָ���ļĴ�����
* ������subAddr - �Ĵ�����ʼ��ַ
*       buffer - �ֽ�����
*       size - �ֽ���
***********************************************/
void IIC_writeBlock(uint8 subAddr,uint8 *buffer,uint8 size);

/***********************************************
* ���ܣ���ָ���ļĴ����ж�ȡһ���ֽ�
* ������subAddr - �Ĵ�����ʼ��ַ
* ���أ���ȡ���ֽ�
***********************************************/
uint8 IIC_readByte(uint8 subAddr);
 
/***********************************************
* ���ܣ���ָ���ļĴ����ж�ȡһ������
* ������subAddr - �Ĵ�����ʼ��ַ
*       buffer - ��������ַ
*       size - Ҫ�����ֽ���
***********************************************/
void IIC_readBlock(uint8 subAddr,uint8 *buffer,uint8 size);


void IIC_start (void);
void IIC_stop (void);
void OutByte(uint8 byteData);
void _IIC_writeBlock (uint8 *byteData, uint16 len);
void _IIC_writeBlockCommand(uint8 *Command, uint16 len);


#endif