/*********************************************************/
/*modified by liubingyan*/
/*********************************************************/
#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H

#include "RTC.h"
#include "type.h"
#include "DataSave.h"
#include "Display.h"
/***************************************************************************************/
/*内部EEPROM地址分配*/

//用户信息和用户配置存储在内部EEPROM中，

#define USER_BASE_ADDR			(uint32)INNER_EEPROM_START			// 用户信息数据保存基地址

#define GENDER_ADDR             (uint32)(0X0000 + USER_BASE_ADDR) 	// 用户性别，1字节
#define AGE_ADDR                (uint32)(0X0001 + USER_BASE_ADDR)	// 用户年龄，1字节
#define HEIGHT_ADDR             (uint32)(0X0002 + USER_BASE_ADDR)	// 用户身高，1字节
#define WEIGHT_ADDR       		(uint32)(0X0003 + USER_BASE_ADDR)	// 用户体重，2字节

#define BIRTHDAYYEAR_ADDR 		(uint32)(0X0005 + USER_BASE_ADDR)	// 用户生日（年），2字节

#define BIRTHDAYMON_ADDR        (uint32)(0X0007 + USER_BASE_ADDR)	// 用户生日（月），1字节
#define BIRTHDAYDAY_ADDR        (uint32)(0X0008 + USER_BASE_ADDR)	// 用户生日（日），1字节

#define TEMPERUNIT_ADDR    		(uint32)(0X0009 + USER_BASE_ADDR)	// 温度单位，1字节
#define DISTANCEUNIT_ADDR       (uint32)(0X000A + USER_BASE_ADDR)	// 距离单位，1字节

#define GOALSTEP_ADDR     		(uint32)(0x000B + USER_BASE_ADDR) 	// 目标步数，4字节
#define TIMEFORMAT_ADDR         (uint32)(0X000F + USER_BASE_ADDR) 	// 小时制，1字节
#define TIMEZONE_MIN_ADDR       (uint32)(0X0010 + USER_BASE_ADDR) 	// 分钟时区，1字节
#define TIMEZONE_HOUR_ADDR      (uint32)(0X0011 + USER_BASE_ADDR) 	// 小时时区，1字节

#define DISPORDER_ADDR    		(uint32)(0X0012 + USER_BASE_ADDR) 	// 界面顺序，11字节
#define GREETING_ADDR     		(uint32)(0x001D + USER_BASE_ADDR) 	// 问候语，9字节

#define STEPLENGTH_ADDR			(uint32)(0x0025 + USER_BASE_ADDR)	// 步长，4字节

#define POWERON_FLAG_ADDR		(uint32)(0x0029 + USER_BASE_ADDR)	// 设备首次上电标志，1字节

#define PAIRCODE_ADDR			(uint32)(0x002A + USER_BASE_ADDR)   // 蓝牙配对密码存储地址

/***************************************************************************************/
/*内部RAM地址分配*/

//数据备份在两个区域分别为：
#define SYSTEM_BACKUP_ADDR_0		(uint32)(0x00000000 + USER_RAM_START)	//备份区域0
#define SYSTEM_BACKUP_ADDR_1		(uint32)(0x00000100 + USER_RAM_START)	//备份区域1

#define CHECKSUM_ADDR_0				(uint32)(0x000000FF + USER_RAM_START)	//备份区域校验和地址
#define CHECKSUM_ADDR_1				(uint32)(0x000001FF + USER_RAM_START)	//备份区域校验和地址
/***************************************************************************************/
/*内部FLASH地址分配*/

// DFU升级标志位地址
#define	UPDATEFLAG_ADDR				(uint32)(0x08080F98)		

//ID号存储在内部FLASH中(DFU区最后四个字节)
#define TRACKERID_ADDR    			(uint32)(0x08002FFC)

/***************************************************************************************/
//DFU升级标志
#define DFU_ENTER       			(uint32)(0x5E9E6211)
#define DFU_EXIT        			(uint32)(0x73E04F60)

typedef enum
{
	FIRST_POWERON		= 0x00,	// 首次上电
	NON_FIRST_POWERON	= 0xFF	// 非首次上电	
	  
}PowerOn_Status;


/***************************************************************************************/

//用户信息
typedef struct
{ 
  	uint16 	weight;          	//体重  
    uint16 	birthdayYear;    	//生日 年 
    uint8 	birthdayMon;      	//生日 月  
    uint8 	birthdayDay;      	//生日 日  
    GENDAR_TypeDef 	gender;     //性别 
    uint8 	height;           	//身高 
    uint8 	age;              	//年龄 
    float 	stepLenght;       	//步长
	
}UserInfo;

//用户设置
typedef struct
{ 
  	uint32 		goalStep;					//目标步数
  	TIME_FormatDef 	timeFormat;				//时间制式
    TEMP_TypeDef 	temperUnit; 			//温度单位
    LUINT_TypeDef 	distanceUnit;    		//距离单位
	uint8 		greeting[GREETING_MAX + 1];	//问候语
	uint8 		displayOrder[FUNC_DISPLAY_MAX];	//界面信息
	TimeZone 	timeZone;					//时区信息
	
}UserConfig;

#if	(BACKUP == ON)
//系统备份
typedef struct
{
	Calendar 		systemClock;		//设备时钟，用于数据恢复
	uint32			displayStep;		//显示步数
	uint32  		userSteps;			//用户当前步数，用于数据恢复
	uint32      	lastSteps;			//用户上一次步数，用于数据恢复
	uint32			userClimb;			//用户当前爬升高度
	MemoryContent 	memoryRun;			//系统备份数据

}SystemRun;
#endif

/*******************************************************************************
*功能：设置问候语
*******************************************************************************/
ErrorStatus UserConfig_setGreeting(const uint8 *buf,const uint8 num);

/*******************************************************************************
*功能：获取问候语
*******************************************************************************/
void UserConfig_getGreeting(uint8 getGreeting[GREETING_MAX + 1]);

/*******************************************************************************
*功能： 设置时区
*******************************************************************************/
ErrorStatus UserConfig_setTimeZone(const TimeZone timeZone);

/*******************************************************************************
*功能： 获取时区
*******************************************************************************/
void UserConfig_getTimeZone(TimeZone *getTimeZone);

/*******************************************************************************
*功能：设置时间制式
*******************************************************************************/
ErrorStatus UserConfig_setTimeFormat(const TIME_FormatDef Format);

/*******************************************************************************
*功能：获取当前时间制式
*******************************************************************************/
void UserConfig_getTimeFormat(uint8 *getTimeFormat);

/*******************************************************************************
*功能：设置显示顺序
*******************************************************************************/
ErrorStatus UserConfig_setDispOrder(const uint8 *order,const uint8 num);

/*******************************************************************************
*功能：获取显示顺序
*******************************************************************************/
void UserConfig_getDispOrder(uint8 *getOrder);

/*******************************************************************************
*功能：设置距离单位制
*******************************************************************************/
ErrorStatus UserConfig_setDistanceUnit(const LUINT_TypeDef temp);

/*******************************************************************************
*功能：获取距离单位制
*******************************************************************************/
void UserConfig_getDistanceUnit(LUINT_TypeDef *getDistanceUnit);

/*******************************************************************************
*功能：设置温度单位制
*参数：温度单位制
返回值： 0 保存失败     1 保存成功
*******************************************************************************/
ErrorStatus UserConfig_setTemperatureUnit(const TEMP_TypeDef temp);

/*******************************************************************************
*功能：获取温度单位制
*******************************************************************************/
void UserConfig_getTemperatureUnit(TEMP_TypeDef *getTempUnit);

/*******************************************************************************
*功能：设置用户目标步数
*******************************************************************************/
ErrorStatus UserConfig_setGoalStep(const uint32 temp);

/*******************************************************************************
*功能：获取用户目标步数
*******************************************************************************/
void UserConfig_geGoalSteps(uint32 *goalSteps);

/*******************************************************************************
*功能：设置用户信息
*******************************************************************************/
ErrorStatus UserConfig_setUserInfo(UserInfo* setUserInfo);

/*******************************************************************************
*功能：获取用户信息
*返回值：用户信息结构体 userInfo类型
*******************************************************************************/
void UserConfig_getUserInfo(UserInfo *getUserInfo);

/*******************************************************************************
*功能：获取配置信息
*******************************************************************************/
void UserConfig_getUserConfig(UserConfig *setUserConfig);

/*******************************************************************************
*功能：初始化设置管理模块
*******************************************************************************/
void UserConfig_init(void);

/*******************************************************************************
*功能：恢复出厂设置
*******************************************************************************/
void UserConfig_clearPowerOnState(void);

/*******************************************************************************
*功能：读取 Tracker ID号
*******************************************************************************/
uint32 UserConfig_readID(void);

/*******************************************************************************
*功能：从EEPROM读取备份时钟
*******************************************************************************/
void UserConfig_getCalendarBackup(Calendar *tempCalendar);

/*******************************************************************************
*功能：从EEPROM读取备份存储状态机
*******************************************************************************/
//void UserConfig_getMemoryBackup(MemoryContent *tempMemory);

/*******************************************************************************
*功能：从EEPROM读取备份显示步数
*******************************************************************************/
void UserConfig_getStepDisplay(uint32 *displayStep);

/*******************************************************************************
*功能：从EEPROM读取备份步数
*******************************************************************************/
void UserConfig_getStepBackup(uint32 *step);

/*******************************************************************************
*功能：从EEPROM读取备份历史步数
*******************************************************************************/
void UserConfig_getLastStepBackup(uint32 *lastStep);

/*******************************************************************************
*功能：从EEPROM读取备份高度
*******************************************************************************/
void UserConfig_getClimbBackup(uint32 *climb);

/*******************************************************************************
*功能：从EEPROM读取系统上电信息
*******************************************************************************/
PowerOn_Status UserConfig_getPowerOnState(void);

/*******************************************************************************
*功能：将系统上电标志位置位
*******************************************************************************/
void UserConfig_setPowerOnState(void);

void UserConfig_initDispOrder(void);


#endif