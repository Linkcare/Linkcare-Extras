#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "type.h"

/**********************************************/
//A20�豸��Ϣ
extern const uint8 deviceType;
extern const uint8 version_issued[13];
extern const uint8 version_developer[13];
/***********************************************
* ���ܣ���ƽ������
* ������a - ��������
* ���أ����
***********************************************/
uint16 System_sqrt(uint32 a);

/***********************************************
* ���ܣ����뼶��ʱ
* ������ms - Ҫ��ʱ��ʱ��(����)
***********************************************/
void System_delay(uint16 ms);

/***********************************************
* ���ܣ����¿��Ź�
***********************************************/
void System_updateWatchDog(void);

/***********************************************
* ���ܣ��ر�����I/O
***********************************************/
void System_disablePin(void);

/***********************************************
*����:ϵͳ��ʼ��
***********************************************/
void System_init(void);


#endif