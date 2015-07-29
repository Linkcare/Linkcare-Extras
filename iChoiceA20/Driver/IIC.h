#ifndef	__IIC_H
#define	__IIC_H

#include "type.h"
/***********************************************
* 功能：初始化IIC模块
***********************************************/
void IIC_enable();

/***********************************************
* 功能：关闭IIC接口
***********************************************/
void IIC_disable();

/***********************************************
* 功能：写一个字节到指定的寄存器中
* 参数：subAddr - 寄存器地址
*       byteData - 字节数据
***********************************************/
void IIC_writeByte(uint8 subAddr, uint8 con, uint8 byteData);

/***********************************************
* 功能：写一块数据到指定的寄存器中
* 参数：subAddr - 寄存器起始地址
*       buffer - 字节数据
*       size - 字节数
***********************************************/
void IIC_writeBlock(uint8 subAddr,uint8 *buffer,uint8 size);

/***********************************************
* 功能：从指定的寄存器中读取一个字节
* 参数：subAddr - 寄存器起始地址
* 返回：读取的字节
***********************************************/
uint8 IIC_readByte(uint8 subAddr);
 
/***********************************************
* 功能：从指定的寄存器中读取一块数据
* 参数：subAddr - 寄存器起始地址
*       buffer - 缓冲区地址
*       size - 要读的字节数
***********************************************/
void IIC_readBlock(uint8 subAddr,uint8 *buffer,uint8 size);


void IIC_start (void);
void IIC_stop (void);
void OutByte(uint8 byteData);
void _IIC_writeBlock (uint8 *byteData, uint16 len);
void _IIC_writeBlockCommand(uint8 *Command, uint16 len);


#endif