#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "stm32l1xx.h"
#include "Graphics.h"

#define  GREETING_MAX			8

#define  FUNC_DISPLAY_MAX		9  	//���ܽ����������
#define  INFO_DISPLAY_MAX		11  //��Ϣ�����������
#define  TEST_DISPLAY_MAX		4   //��������������

//��Ϣ�������
#define	 	CHARGING		0
#define	 	BATTEYFULL		1
#define	 	BATTEYLOW		2
#define	 	MEMORYLOW		3
#define	 	MEMORYFULL		4
#define		CHOICEMARK		5
#define		SLEEP			6
#define		LETGO			7
#define		COMEON			8
#define		WELLDONE		9
#define		SHAKE			10

//���Խ������
#define	 	LIS3DH			0
#define	 	TEMPERATURE		1
#define	 	AIRPRESSURE		2
#define	 	EEPROM			3

//��ȡ��������
uint8 Display_getDisplayCount(void);

//���ܽ����趨
ErrorStatus Display_setDisplay(uint8 *pvalue,const uint8 length);
		
//���ܽ���
extern pFunc func_disp[FUNC_DISPLAY_MAX];

//���Խ���
extern const pFunc test_disp[TEST_DISPLAY_MAX];

//��Ϣ����
extern const pFunc infor_disp[INFO_DISPLAY_MAX];


//��������
void displayMEMS_test(void);
void displayAirpressure_test(void);
void displayEEPROM_test(void);
void displayTemperature_test(void);
	  
void displayStep(void);
void displayClock(void);
void displayPercent(void);
void displayDistance(void);
void displaySpeed(void);
void displayCalory(void);
void displayFat(void);
void displayAltitude(void);
void displayTemperature(void);
void displayClimbAltitude(void);
void displayCharging(void);
void displayBatteryLow(void);
void displayMemoryLow(void);
void displayMemoryFull(void);
void displayChoiceMark(void);
void displaySleep(void);
void displayLETGO(void);
void displayCOMEON(void);
void displayWELLDONE(void);
void displayWakeByShake(void);
void displayBatteryFull(void);
void displayPlaneOn(void);
void displayPlaneOff(void);

#endif