// ----------------------------------------------
// 文件说明：按键模块的头文件
// ----------------------------------------------
#include "type.h"

// 常量定义
#define NO_KEY	 	 0
#define SHORT_KEY    1
#define LONG_KEY     2

// 宏定义

// 函数原型

// 函数功能：按键扫描
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：该函数被定时中断调用，周期是20mS
//           这个函数支持长按键
void Key_DEV_ScanKey();

// 3.应用层
// 函数功能：初始化按键模块
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void KEY_API_Init();

// 函数功能：检查按键缓冲区中是否有键需要处理
// 输入参数：无
// 输出参数：无
// 返 回 值：1 -- 按键缓冲区中存在按键需要处理
//           0 -- 无
// 备    注：无
uint32 KEY_API_CheckKey();

// 函数功能：返回按键缓冲区中的当前按键
// 输入参数：无
// 输出参数：无
// 返 回 值：按键缓冲区的按键键值
// 备    注：无
uint32 KEY_API_GetKey();


/**************************************************/
/*使能按键中断*/
/**************************************************/
void KEY_enableInt(void);

/**************************************************/
/*禁止按键中断*/
/**************************************************/
void KEY_disableInt(void);
