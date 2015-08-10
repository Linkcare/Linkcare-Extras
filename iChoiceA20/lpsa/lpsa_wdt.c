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
*                             ����һ�����Ź�
* ���룺VDT_DATA *WDT;--���Ź����ݽṹ
*       lpsa_u8  ModuleSum;--������Ź��ĸ���������ܳ���16
*
* ����� 0;--�ɹ�
        -1;��������Ϊ��
        -2;��������������ֵ
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
*                             ���ι������
*
* ���룺WDT_DATA  *Wdt;--���Ź����ݽṹ
*       lpsa_u16  Module;--��Ҫι�ĸ����Ź�ģ��
*       lpsa_u8   reg;--ι��ģʽ��������NORMAL_EAT(�ظ�ι��)��ONCE_EAT(����ι��)
*
* ����� 0;--�ɹ�
        -1;���Ź����ݽṹΪ��
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
*                             Ӳ��ι������
*
* ���룺WDT_DATA  *Wdt;--���Ź����ݽṹ
*
* �����NULL;
*****************************************************************************/
void WdtEate (WDT_DATA *Wdt)
{
	if (Wdt != NULL)
		__eatfun (Wdt);
}

