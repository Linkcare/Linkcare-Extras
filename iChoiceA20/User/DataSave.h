#ifndef __DATASAVE_H
#define __DATASAVE_H

#include "Type.h"

//7天数据存储起止地址(28KByte 的空间)
#define SPACE_7		0x7000

#define ADDRESS_START_7 		(OUTER_EEPROM_START + 0x0000)
#define ADDRESS_END_7 			(OUTER_EEPROM_START + 0x6FFF)

//23天数据存储起止地址(138Byte 的空间)
#define SPACE_23		0x8A

#define ADDRESS_START_23	 	(OUTER_EEPROM_START + 0x7400)
#define ADDRESS_END_23	 		(OUTER_EEPROM_START + 0x7489)

//7天最大预留空闲空间
#define MAX_RESERVED_SPACE	0x3E8

//睡眠数据标签
#define SLEEPMARK   0x1FF

//最大存储天数
#define MAXDAYS		7

//存储空间使用情况
typedef enum
{
	ENOUGH  = 0x00,	//内存充足
	LOW	 	= 0x01,	//内存不足
	FULL	= 0X02	//内存满
	
}MemDef;

//数据排列类型
typedef enum
{
	SEQUENTIAL  = 0x00,	//数据连续排列
	BREAK	 	= 0x01	//数据断续排列
	
}Data_RankDef;

//数据存储类型定义
typedef enum
{
	BASIC_TYPE = 0x00,	//F1 OR F2
	TIME_TYPE  = 0x01,	//F3 + F1 OR F3 + F2
	DATE_TYPE  = 0x02	//F4 + F3 + F1 OR F4 + F3 + F2
	
}DataSave_TypeDef;

//自然天类型定义
typedef enum
{
	ISEMPTY   = 0x00,	//当天尚未数据存储
	NOTEMPTY  = 0x01	//当天已有数据存储
	
}DAY_TypeDef;

//数据有效性类型定义
typedef enum
{
	VALID   = 0x01,	//有效
	INVALID = 0x00	//无效
	
}VALID_TypeDef;

//日期标签格式
typedef struct 
{
	uint16 	day		:5,	
			month	:4,	
			year	:5,	
			type	:2;
			
}DateMarkType;

//时间标签格式
typedef struct 
{
  	uint16 minutes	:14,
		   type		:2;
		   
}TimeMarkType;

//分钟运动数据类型
typedef struct 
{
  	uint16 data_9	:9,
		   data_6	:6,
		   type		:1;
		   
}MinuteDataType;

//天数据类型
typedef struct 
{
  	DateMarkType  timeMark;	//本天日期
	uint32	allStep;		//本天总步数
	uint32	allClimb;		//本天总高度
	
}DayDataDef;

//天数据存储包
typedef struct 
{
  	uint8 data[6];		//数据缓存
	uint8 length;		//数据包长度
	uint8 valid;		//数据有效性
	uint16 address;		//数据存储地址
}
DayDataPocket;

//分钟数据存储包
typedef struct
{
	uint8  data[6];		//数据缓存
	uint16 address;		//数据存储地址
	uint16 address1;	//数据存储地址1
	uint16 dataType:4,	//数据类型
		   length:4,	//数据包长度
		   length1:4,	//数据包长度1
	       valid:4;		//数据有效性
	
}MinDataPocket;


//自然天分钟数据信息
typedef struct 
{
	uint32  startAddress;		 //本天数据起始地址
	
	DayDataDef dayDataPocket; 	 //天数据存储包
	
}DayInfoType;


//函数声明
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