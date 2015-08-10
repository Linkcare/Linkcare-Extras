#ifndef __DATASAVE_H
#define __DATASAVE_H

#include "Type.h"

//7�����ݴ洢��ֹ��ַ(28KByte �Ŀռ�)
#define SPACE_7		0x7000

#define ADDRESS_START_7 		(OUTER_EEPROM_START + 0x0000)
#define ADDRESS_END_7 			(OUTER_EEPROM_START + 0x6FFF)

//23�����ݴ洢��ֹ��ַ(138Byte �Ŀռ�)
#define SPACE_23		0x8A

#define ADDRESS_START_23	 	(OUTER_EEPROM_START + 0x7400)
#define ADDRESS_END_23	 		(OUTER_EEPROM_START + 0x7489)

//7�����Ԥ�����пռ�
#define MAX_RESERVED_SPACE	0x3E8

//˯�����ݱ�ǩ
#define SLEEPMARK   0x1FF

//���洢����
#define MAXDAYS		7

//�洢�ռ�ʹ�����
typedef enum
{
	ENOUGH  = 0x00,	//�ڴ����
	LOW	 	= 0x01,	//�ڴ治��
	FULL	= 0X02	//�ڴ���
	
}MemDef;

//������������
typedef enum
{
	SEQUENTIAL  = 0x00,	//������������
	BREAK	 	= 0x01	//���ݶ�������
	
}Data_RankDef;

//���ݴ洢���Ͷ���
typedef enum
{
	BASIC_TYPE = 0x00,	//F1 OR F2
	TIME_TYPE  = 0x01,	//F3 + F1 OR F3 + F2
	DATE_TYPE  = 0x02	//F4 + F3 + F1 OR F4 + F3 + F2
	
}DataSave_TypeDef;

//��Ȼ�����Ͷ���
typedef enum
{
	ISEMPTY   = 0x00,	//������δ���ݴ洢
	NOTEMPTY  = 0x01	//�����������ݴ洢
	
}DAY_TypeDef;

//������Ч�����Ͷ���
typedef enum
{
	VALID   = 0x01,	//��Ч
	INVALID = 0x00	//��Ч
	
}VALID_TypeDef;

//���ڱ�ǩ��ʽ
typedef struct 
{
	uint16 	day		:5,	
			month	:4,	
			year	:5,	
			type	:2;
			
}DateMarkType;

//ʱ���ǩ��ʽ
typedef struct 
{
  	uint16 minutes	:14,
		   type		:2;
		   
}TimeMarkType;

//�����˶���������
typedef struct 
{
  	uint16 data_9	:9,
		   data_6	:6,
		   type		:1;
		   
}MinuteDataType;

//����������
typedef struct 
{
  	DateMarkType  timeMark;	//��������
	uint32	allStep;		//�����ܲ���
	uint32	allClimb;		//�����ܸ߶�
	
}DayDataDef;

//�����ݴ洢��
typedef struct 
{
  	uint8 data[6];		//���ݻ���
	uint8 length;		//���ݰ�����
	uint8 valid;		//������Ч��
	uint16 address;		//���ݴ洢��ַ
}
DayDataPocket;

//�������ݴ洢��
typedef struct
{
	uint8  data[6];		//���ݻ���
	uint16 address;		//���ݴ洢��ַ
	uint16 address1;	//���ݴ洢��ַ1
	uint16 dataType:4,	//��������
		   length:4,	//���ݰ�����
		   length1:4,	//���ݰ�����1
	       valid:4;		//������Ч��
	
}MinDataPocket;


//��Ȼ�����������Ϣ
typedef struct 
{
	uint32  startAddress;		 //����������ʼ��ַ
	
	DayDataDef dayDataPocket; 	 //�����ݴ洢��
	
}DayInfoType;


//��������
uint16 DataSave_getRestSpace(void);
uint8 DataSave_getMemPercent(void);
MemDef DataSave_getMemState(void);
void DataSave_setHeadAddress(const uint16 newHeadAddress);
void DataSave_getReadyForUpload(void);
void DataSave_initAfterUpload(void);
void DataSave_init(void);
uint16 DataSave_getStartAddress7(void);
uint16 DataSave_getEndAddress7(void);
uint16 DataSave_getStartAddress23(void);
uint16 DataSave_getEndAddress23(void);
void DataSave_writeToEEPROM(void);
void DataSave_alarm(void);


#endif