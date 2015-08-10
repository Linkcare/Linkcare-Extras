#ifndef	__VBUS_H
#define	__VBUS_H

#include "type.h"
/*
  模块说明：与VBUS相关的头文件
*/

// 常量定义
#define VBUS_ON       0                 // VBUS有电标志
#define VBUS_OFF      1                 // VBUS没电标志


#define USB_ON            1             // USB打开的标志
#define USB_OFF           0             // USB关闭的标志
// 宏定义

// 函数原型

// 函数功能：VBUS信号扫描
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：该函数被定时中断调用，周期是20mS
void VBUS_DEV_Scan();

// 3.应用层

// 函数功能：初始化VBUS检测模块
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void VBUS_API_Init();

// 函数功能：检查当前状态机
// 输入参数：无
// 输出参数：无
// 返 回 值：VBUS_ON    -- VBUS有电
//           VBUS_OFF   -- VBUS没电
// 备    注：无
uint32 VBUS_API_GetVBus();

/******************************************************************/
/*使能VBUS ON 检测中断											  */
/******************************************************************/
void VBUS_enableVBUSON(void);

/******************************************************************/
/*禁止VBUS ON 检测中断											  */
/******************************************************************/
void VBUS_disableVBUSON(void);

void VBUS_enableCable(void);

#endif