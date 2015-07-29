// wdt.cpp : Defines the entry point for the console application.
//

#include "lpsa.h"
const static lpsa_u16 ModuleStk[17] = 
{
	NULL,
	MODULE_0, MODULE_1, MODULE_2, MODULE_3, MODULE_4,
	MODULE_5, MODULE_6, MODULE_7, MODULE_8, MODULE_9,
	MODULE_10, MODULE_11, MODULE_12, MODULE_13, MODULE_14, MODULE_15,
};


/*****************************************************************************
*                             创建一个看门狗
* 输入：VDT_DATA *WDT;--看门狗数据结构
*       lpsa_u8  ModuleSum;--软件看门狗的个数，最大不能超过16
*
* 输出： 0;--成功
        -1;看门数据为空
        -2;超出软件狗的最大值
*****************************************************************************/
lpsa_i8 CreateWdt (WDT_DATA *Wdt, lpsa_u8 ModuleSum)
{
	if (Wdt == NULL)
		return -1;
	if (ModuleSum > 16)
		return -2;

	Wdt->ModuleReg = 0x0000;
	Wdt->ModuleSum = ModuleSum;
	Wdt->OnceReg = 0x00000;
        Wdt->eatwdt = 0;
        Wdt->reset = 0;
	return 0;
}

/*****************************************************************************
*                             软件喂狗函数
*
* 输入：WDT_DATA  *Wdt;--看门狗数据结构
*       lpsa_u16  Module;--需要喂哪个看门狗模块
*       lpsa_u8   reg;--喂狗模式，可以是NORMAL_EAT(重复喂狗)或ONCE_EAT(单次喂狗)
*
* 输出： 0;--成功
        -1;看门狗数据结构为空
*****************************************************************************/
lpsa_i8 WdtTaskEat (WDT_DATA *Wdt, lpsa_u16 Module, lpsa_u8  reg)
{
	if (Wdt == NULL)
		return -1;

	if (reg)
		Wdt->ModuleReg |= Module;
	else
		if (!(Wdt->OnceReg & Module))
		{
			Wdt->OnceReg |= Module;
			Wdt->ModuleReg |= Module;
		}
		else
			Wdt->ModuleReg &= ~Module;

	return 0;
}

void __eatfun (WDT_DATA *Wdt)
{
	if (Wdt->ModuleReg == ModuleStk[Wdt->ModuleSum])
	{
		Wdt->ModuleReg = 0x0000;
		Wdt->OnceReg = 0x00000;
                if (Wdt->eatwdt != 0)
		  Wdt->eatwdt ();
		return;
	}
#ifdef	SOFT_RESET
        if (Wdt->reset != 0)
	  Wdt->reset ();
#endif
}
/*****************************************************************************
*                             硬件喂狗函数
*
* 输入：WDT_DATA  *Wdt;--看门狗数据结构
*
* 输出：NULL;
*****************************************************************************/
void WdtEate (WDT_DATA *Wdt)
{
	if (Wdt != NULL)
		__eatfun (Wdt);
}

