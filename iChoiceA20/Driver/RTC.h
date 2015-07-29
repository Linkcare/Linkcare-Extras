#ifndef	__RTC_H
#define	__RTC_H

#include "stm32l1xx.h"
#include "type.h"
#include "DeviceInfo.h"

//Ĭ��ʱ�䣺2013��1��1��1�����ڶ������磬1:0:0
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
* ���ܣ���ȡRTC���ں�ʱ��
***********************************************/
void RTC_getCalendar(Calendar *calendar);

/***********************************************
* ���ܣ�RTCģ���ʼ��
***********************************************/
void RTC_init();

/***********************************************
* ���ܣ�RTCģ���ʼ��
***********************************************/
void RTC_recover(void);

/***********************************************
* ���ܣ���������
* ������RTC_Alarm - RTC_Alarm_A �� RTC_Alarm_B
        RTC_AlarmStructure - �������ò���
***********************************************/
void RTC_setAlarm(uint32_t RTC_Alarm,RTC_AlarmTypeDef  RTC_AlarmStructure);

/***********************************************
* ���ܣ�����ʱ��
* ������date - ����
        time - ʱ��
����ֵ��SUCCESS �����óɹ�
		ERROR   : ����ʧ��
***********************************************/
ErrorStatus RTC_setTime(RTC_DateTypeDef date,RTC_TimeTypeDef time);

/***********************************************
* ���ܣ�ʹ��RTC��ʱ��
***********************************************/
void RTC_enableRTCWakeup();		//�������ڣ�20ms
	
void RTC_enableRTCForStop();	//�������ڣ�5s
/***********************************************
* ���ܣ���ֹRTC��ʱ��
***********************************************/
void RTC_disableRTCWakeup(void);

/***********************************************
* ���ܣ��������Ƴ�1����
***********************************************/
void RTC_putOffAlarm(Calendar* rtcTime,uint8 offset);

/***********************************************
* ���ܣ���ȡϵͳ��ǰ�ĺ�����
* ������������
***********************************************/
void RTC_obtainMilliSeconds(uint64 *const lastestSecond);

/***********************************************
* ���ܣ���ⶨʱ���Ƿ�ʱ
* ������
***********************************************/
TIMEOUT_TypeDef RTC_checkTimeout(const uint64 checkSecond,const uint32 thresholdSeccond);

#endif