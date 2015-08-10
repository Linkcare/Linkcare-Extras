#ifndef	__RTC_H
#define	__RTC_H

#include "stm32l1xx.h"
#include "type.h"
#include "DeviceInfo.h"

//默认时间：2013年1月1日1，星期二，上午，1:0:0
#define	  DEFAULT_YEAR	14
#define	  DEFAULT_MON	1
#define	  DEFAULT_DAY	1
#define	  DEFAULT_WEEK	RTC_Weekday_Wednesday
#define	  DEFAULT_HOUR	1
#define	  DEFAULT_MIN	0
#define	  DEFAULT_SEC	0
#define	  DEFAULT_NOON	RTC_H12_AM

typedef struct
{
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
	TIME_FormatDef timeFormat;
	TIME_PeriodDef timePeriod;
}
Calendar;
  
typedef struct
{
	int8 hour;
  	uint8 min; 
}
TimeZone;

/***********************************************
* 功能：获取RTC日期和时间
***********************************************/
void RTC_getCalendar(Calendar *calendar);

/***********************************************
* 功能：RTC模块初始化
***********************************************/
void RTC_init();

/***********************************************
* 功能：RTC模块初始化
***********************************************/
void RTC_recover(void);

/***********************************************
* 功能：设置闹钟
* 参数：RTC_Alarm - RTC_Alarm_A 或 RTC_Alarm_B
        RTC_AlarmStructure - 闹钟配置参数
***********************************************/
void RTC_setAlarm(uint32_t RTC_Alarm,RTC_AlarmTypeDef  RTC_AlarmStructure);

/***********************************************
* 功能：设置时间
* 参数：date - 日期
        time - 时间
返回值：SUCCESS ：设置成功
		ERROR   : 设置失败
***********************************************/
ErrorStatus RTC_setTime(RTC_DateTypeDef date,RTC_TimeTypeDef time);

/***********************************************
* 功能：使能RTC定时器
***********************************************/
void RTC_enableRTCWakeup();		//唤醒周期：20ms
	
void RTC_enableRTCForStop();	//唤醒周期：5s
/***********************************************
* 功能：禁止RTC定时器
***********************************************/
void RTC_disableRTCWakeup(void);

/***********************************************
* 功能：将闹钟推迟1分钟
***********************************************/
void RTC_putOffAlarm(Calendar* rtcTime,uint8 offset);

/***********************************************
* 功能：获取系统当前的毫秒数
* 参数：毫秒数
***********************************************/
void RTC_obtainMilliSeconds(uint64 *const lastestSecond);

/***********************************************
* 功能：检测定时器是否超时
* 参数：
***********************************************/
TIMEOUT_TypeDef RTC_checkTimeout(const uint64 checkSecond,const uint32 thresholdSeccond);

#endif