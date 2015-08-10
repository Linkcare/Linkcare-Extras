#include "stdlib.h"
#include "stm32l1xx.h"

#include "vbus.h"
#include "key.h"
#include "RTC.h"
#include "BLE.h"
#include "System.h"

#include "Event.h"
#include "Task.h"
#include "DataSave.h"
#include "Configuration.h"

//Ĭ�ϳ�ʼ����
static RTC_DateTypeDef default_date = 
{
	DEFAULT_WEEK,
	DEFAULT_MON,
	DEFAULT_DAY,
	DEFAULT_YEAR
};

//Ĭ�ϳ�ʼʱ��
static RTC_TimeTypeDef default_time = 
{
	DEFAULT_HOUR,
	DEFAULT_MIN,
	DEFAULT_SEC,
	DEFAULT_NOON
};

//ϵͳ���붨ʱ������
static uint64 rtcMilliSeconds = 0;

// �������ܣ���ȡRTC���ں�ʱ��
// �����������
// ���������Calendar *calendar
// �� �� ֵ����
// ��    ע����
void RTC_getCalendar(Calendar *calendar)
{
	while(RTC_WaitForSynchro() == ERROR)__NOP();

    RTC_GetTime(RTC_Format_BIN, &calendar->time);

    RTC_GetDate(RTC_Format_BIN, &calendar->date);
}

// �������ܣ�����RTC����
// ���������	RTC_Alarm��RTC_Alarm_A �� RTC_Alarm_B
//			  	RTC_AlarmStructure������ʱ��
// ���������Calendar *calendar
// �� �� ֵ����
// ��    ע����
void RTC_setAlarm(uint32_t RTC_Alarm, RTC_AlarmTypeDef RTC_AlarmStructure)
{
    EXTI_InitTypeDef  EXTI_InitStructure;
    EXTI_ClearITPendingBit(EXTI_Line17);
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef nvicInit;
    nvicInit.NVIC_IRQChannel = RTC_Alarm_IRQn;
    nvicInit.NVIC_IRQChannelPreemptionPriority = PRI_MCU_ALARM;
    nvicInit.NVIC_IRQChannelSubPriority = 0;
    nvicInit.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicInit); 

    RTC_ITConfig(RTC_IT_ALRA, DISABLE);
    RTC_AlarmCmd(RTC_Alarm, DISABLE);

    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm, &RTC_AlarmStructure);

    RTC_ITConfig(RTC_IT_ALRA, ENABLE);
    RTC_AlarmCmd(RTC_Alarm, ENABLE);
}

// �������ܣ��������Ƴ�n����
// ���������	rtcTime����ǰʱ��
//			  	offset������ƫ����
// �����������
// �� �� ֵ����
// ��    ע����
void RTC_putOffAlarm(Calendar* rtcTime,uint8 offset)
{
    Calendar tempcalendar;
    RTC_AlarmTypeDef RTC_AlarmStructure;

    tempcalendar = *rtcTime;

    tempcalendar.time.RTC_Minutes += offset;

    if (tempcalendar.time.RTC_Minutes > 59)
    {
        tempcalendar.time.RTC_Minutes -= 60;
    }

    tempcalendar.time.RTC_Seconds = 0;

    RTC_AlarmStructure.RTC_AlarmTime = tempcalendar.time;
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_Weekday_Sunday;
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay | RTC_AlarmMask_Hours | RTC_AlarmMask_Minutes;
    RTC_setAlarm(RTC_Alarm_A, RTC_AlarmStructure);
}

// �������ܣ��趨RT�������ʱ��
// ���������	date - ����
//				time - ʱ��
// �����������
// �� �� ֵ��	SUCCESS �����óɹ�
//			 	ERROR   : ����ʧ��
// ��    ע����
ErrorStatus RTC_setTime(RTC_DateTypeDef date, RTC_TimeTypeDef time)
{
    RTC_DateTypeDef tempDate;
    RTC_TimeTypeDef tempTime;
    RTC_AlarmTypeDef RTC_AlarmStructure;

  	if(date.RTC_Year < 13 || date.RTC_Month > 12 || date.RTC_Date > 31 || date.RTC_WeekDay > 7)
        return ERROR;

    if (time.RTC_Hours > 24 || time.RTC_Minutes > 59 || time.RTC_Seconds > 59)
        return ERROR;

SET_DATE_AGAIN: 
    while(RTC_SetDate(RTC_Format_BIN, &date) == ERROR)__NOP();//��������

    RTC_GetDate(RTC_Format_BIN, &tempDate);

	if(tempDate.RTC_Date  != date.RTC_Date ||  \
	   tempDate.RTC_Month != date.RTC_Month || \
	   tempDate.RTC_Year  != date.RTC_Year)
    {
        goto SET_DATE_AGAIN;
    }

    if (time.RTC_Hours > 12)
    {
        time.RTC_H12 = RTC_H12_PM;
    }
    else
    {
        time.RTC_H12 = RTC_H12_AM;
    }

SET_TIME_AGAIN:
    while(RTC_SetTime(RTC_Format_BIN, &time) == ERROR)__NOP();
	
    RTC_GetTime(RTC_Format_BIN, &tempTime);

	if(tempTime.RTC_Hours != time.RTC_Hours || tempTime.RTC_Minutes != time.RTC_Minutes)
    {
        goto SET_TIME_AGAIN;
    }

    //��������
    tempTime.RTC_Seconds = 0;

    tempTime.RTC_Minutes += 1;

    if (tempTime.RTC_Minutes > 59)
    {
        tempTime.RTC_Minutes = 0;
    }

	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;
    RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_Weekday_Sunday;
    RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay | RTC_AlarmMask_Hours | RTC_AlarmMask_Minutes;
    RTC_AlarmStructure.RTC_AlarmTime = tempTime;
    RTC_setAlarm(RTC_Alarm_A, RTC_AlarmStructure);

    return SUCCESS;
}

// �������ܣ�RTCģ���ʼ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void RTC_init(void)
{
    RTC_InitTypeDef RTC_InitStructure;

    //1��Enable the Power Controller
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    //2��Reset Backup Domain
    RCC_RTCResetCmd(ENABLE);
    RCC_RTCResetCmd(DISABLE);

    //3��Enable access to RTC domain
    PWR_RTCAccessCmd(ENABLE);

    //4��Enable LSE
    RCC_LSEConfig(RCC_LSE_ON);

    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        __NOP();

    //5��Select the RTC clock source
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    RTC_InitStructure.RTC_AsynchPrediv = 0x7F; //128��Ƶ
    RTC_InitStructure.RTC_SynchPrediv = 0xFF; //256��Ƶ
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; //24Сʱ��
    RTC_Init(&RTC_InitStructure);

    //6��Enable RTC
    RCC_RTCCLKCmd(ENABLE);

    //7������ʱ���ʼ��,��������
    RTC_setTime(default_date, default_time);
}

#if	(BACKUP == ON)
// �������ܣ�RTCģ�鱸�ݳ�ʼ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void RTC_recover(void)
{
  	Calendar backupCalendar;
	
  	UserConfig_getCalendarBackup(&backupCalendar);
	
	RTC_setTime(backupCalendar.date,backupCalendar.time);
}
#endif

// �������ܣ�ʹ��ms��ʱ����30ms��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void RTC_enableRTCWakeup(void)
{
    /*�������ݽṹ*/
    EXTI_InitTypeDef EXTI_InitStructure;

    PWR_RTCAccessCmd(ENABLE);
    RTC_ITConfig(RTC_IT_WUT, DISABLE);
    RTC_WakeUpCmd(DISABLE);

    /*���EXTI20�ж�����*/
    EXTI_ClearITPendingBit(EXTI_Line20);

    /*��ʼ���ⲿ�ж�20*/
    EXTI_InitStructure.EXTI_Line = EXTI_Line20;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*��ʼ��RTC�����жϵ��ж�����*/
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PRI_MCU_TIMER;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /*����RTC������ʱ��32768/2=16384HZ*/
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div2);

    /*����RTC���Ѽ�����������ֵΪ328,�������൱��RTC��ʱƵ��Ϊ(1/16384)*328 = 20ms*/
    RTC_SetWakeUpCounter(RTC_WORK_TIME * 16384 / 1000);

    /*����RTC���Ѳ����ж�*/
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

    /*����RTC���ѹ���*/
    RTC_WakeUpCmd(ENABLE);
}

// �������ܣ�ʹ��ms��ʱ����5s��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void RTC_enableRTCForStop(void)
{
    /*�������ݽṹ*/
    EXTI_InitTypeDef EXTI_InitStructure;

    PWR_RTCAccessCmd(ENABLE);
    RTC_ITConfig(RTC_IT_WUT, DISABLE);
    RTC_WakeUpCmd(DISABLE);

    /*���EXTI20�ж�����*/
    EXTI_ClearITPendingBit(EXTI_Line20);

    /*��ʼ���ⲿ�ж�20*/
    EXTI_InitStructure.EXTI_Line = EXTI_Line20;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*��ʼ��RTC�����жϵ��ж�����*/
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PRI_MCU_TIMER;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /*����RTC������ʱ��1HZ*/
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);

    /*����RTC���Ѽ�����������ֵΪ5,�������൱��RTC��ʱƵ��Ϊ(1 * 5)s*/
    RTC_SetWakeUpCounter(RTC_STOP_TIME / 1000);

    /*����RTC���Ѳ����ж�*/
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

    /*����RTC���ѹ���*/
    RTC_WakeUpCmd(ENABLE);
}

// �������ܣ���ȡms��ʱ������ֵ
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void RTC_obtainMilliSeconds(uint64 *const lastestSecond)
{
    __disable_interrupt();
	
    *lastestSecond = rtcMilliSeconds;
	
    __enable_interrupt();
}

// �������ܣ���鶨ʱ���Ƿ�ʱ
// ���������	checkSecond	--	�����Ķ�ʱ����
//				thresholdSeccond -- ��ʱ����ֵ
// �����������
// �� �� ֵ����
// ��    ע����
TIMEOUT_TypeDef RTC_checkTimeout(const uint64 checkSecond,const uint32 thresholdSeccond)
{
    __disable_interrupt();
	
    if(rtcMilliSeconds - checkSecond > thresholdSeccond)
	{
	  	__enable_interrupt();
			  
		return IS_TIMEOUT;
	}
	else
	{
	  	__enable_interrupt();
		
		return NO_TIMEOUT;
	}
}

// �������ܣ���ʱ������
// ���������interval -- �ۼ���
// �����������
// �� �� ֵ����
// ��    ע����
static void RTC_updateTimer(const uint16 interval)
{
    __disable_interrupt();
	
    rtcMilliSeconds += interval;
	  
	__enable_interrupt();
}

// �������ܣ���ʱ���ж�
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void RTC_WKUP_IRQHandler(void)
{
    if (Event_getCurrentMode() == TASK_TRACKER)
	{
        //���¼�ʱ��
		RTC_updateTimer(RTC_WORK_TIME);
		
		//����ɨ��
		Key_DEV_ScanKey();
		
		//USB�˿�ɨ��
		VBUS_DEV_Scan();
		
		//������ʱ
		BleDelay(1);
	}
    else if (Event_getCurrentMode() == TASK_SLEEP)
	{
		//���¼�ʱ��
		RTC_updateTimer(RTC_WORK_TIME);
		
		//����ɨ��
		Key_DEV_ScanKey();
		
		//USB�˿�ɨ��
		VBUS_DEV_Scan();
	}
	else if(Event_getCurrentMode() == TASK_DEEPSLEEP)
	{
		//���¼�ʱ��
		RTC_updateTimer(RTC_WORK_TIME);
		
		//����ɨ��
		Key_DEV_ScanKey();
	}
	else if(Event_getCurrentMode() == TASK_STOP)
	{
		//���¼�ʱ��
		RTC_updateTimer(RTC_STOP_TIME);
	}
	  
#if	(WATCHDOG == ON)
		
	//ι��
	RTC_updateWatchDog();

#endif
	
	//���RTC�еĻ��ѱ�־
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	/*�ر�ע�⣺�����2���������ö�����ȥ�����������ֲ����������
      ����һ�ֽ���취���������������Ĵ�����Ȼ�������RTC
      ����ⲿ�ж�20�������־
	*/
	EXTI_ClearITPendingBit(EXTI_Line20);
	
	//����ⲿ�ж�20�Ĺ���Ĵ����е��ж������־
	EXTI_ClearFlag(EXTI_Line20);
}

// �������ܣ������ж�
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void RTC_Alarm_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_ALRA) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_ALRA);
        EXTI_ClearITPendingBit(EXTI_Line17);

		//���ݴ洢
		DataSave_alarm();
    }
}
