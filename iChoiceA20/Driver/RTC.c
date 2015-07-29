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

//默认初始日期
static RTC_DateTypeDef default_date = 
{
	DEFAULT_WEEK,
	DEFAULT_MON,
	DEFAULT_DAY,
	DEFAULT_YEAR
};

//默认初始时间
static RTC_TimeTypeDef default_time = 
{
	DEFAULT_HOUR,
	DEFAULT_MIN,
	DEFAULT_SEC,
	DEFAULT_NOON
};

//系统毫秒定时计数器
static uint64 rtcMilliSeconds = 0;

// 函数功能：获取RTC日期和时间
// 输入参数：无
// 输出参数：Calendar *calendar
// 返 回 值：无
// 备    注：无
void RTC_getCalendar(Calendar *calendar)
{
	while(RTC_WaitForSynchro() == ERROR)__NOP();

    RTC_GetTime(RTC_Format_BIN, &calendar->time);

    RTC_GetDate(RTC_Format_BIN, &calendar->date);
}

// 函数功能：设置RTC闹钟
// 输入参数：	RTC_Alarm：RTC_Alarm_A 或 RTC_Alarm_B
//			  	RTC_AlarmStructure：闹钟时间
// 输出参数：Calendar *calendar
// 返 回 值：无
// 备    注：无
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

// 函数功能：将闹钟推迟n分钟
// 输入参数：	rtcTime：当前时钟
//			  	offset：分钟偏移量
// 输出参数：无
// 返 回 值：无
// 备    注：无
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

// 函数功能：设定RT次日起和时间
// 输入参数：	date - 日期
//				time - 时间
// 输出参数：无
// 返 回 值：	SUCCESS ：设置成功
//			 	ERROR   : 设置失败
// 备    注：无
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
    while(RTC_SetDate(RTC_Format_BIN, &date) == ERROR)__NOP();//设置日期

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

    //设置闹钟
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

// 函数功能：RTC模块初始化
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void RTC_init(void)
{
    RTC_InitTypeDef RTC_InitStructure;

    //1、Enable the Power Controller
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    //2、Reset Backup Domain
    RCC_RTCResetCmd(ENABLE);
    RCC_RTCResetCmd(DISABLE);

    //3、Enable access to RTC domain
    PWR_RTCAccessCmd(ENABLE);

    //4、Enable LSE
    RCC_LSEConfig(RCC_LSE_ON);

    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        __NOP();

    //5、Select the RTC clock source
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    RTC_InitStructure.RTC_AsynchPrediv = 0x7F; //128分频
    RTC_InitStructure.RTC_SynchPrediv = 0xFF; //256分频
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; //24小时制
    RTC_Init(&RTC_InitStructure);

    //6、Enable RTC
    RCC_RTCCLKCmd(ENABLE);

    //7、日期时间初始化,设置闹钟
    RTC_setTime(default_date, default_time);
}

#if	(BACKUP == ON)
// 函数功能：RTC模块备份初始化
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void RTC_recover(void)
{
  	Calendar backupCalendar;
	
  	UserConfig_getCalendarBackup(&backupCalendar);
	
	RTC_setTime(backupCalendar.date,backupCalendar.time);
}
#endif

// 函数功能：使能ms定时器（30ms）
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void RTC_enableRTCWakeup(void)
{
    /*定义数据结构*/
    EXTI_InitTypeDef EXTI_InitStructure;

    PWR_RTCAccessCmd(ENABLE);
    RTC_ITConfig(RTC_IT_WUT, DISABLE);
    RTC_WakeUpCmd(DISABLE);

    /*清除EXTI20中断请求*/
    EXTI_ClearITPendingBit(EXTI_Line20);

    /*初始化外部中断20*/
    EXTI_InitStructure.EXTI_Line = EXTI_Line20;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*初始化RTC唤醒中断的中断向量*/
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PRI_MCU_TIMER;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /*设置RTC的输入时钟32768/2=16384HZ*/
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div2);

    /*设置RTC唤醒计数器的重置值为328,这样就相当于RTC定时频率为(1/16384)*328 = 20ms*/
    RTC_SetWakeUpCounter(RTC_WORK_TIME * 16384 / 1000);

    /*允许RTC唤醒产生中断*/
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

    /*启动RTC唤醒功能*/
    RTC_WakeUpCmd(ENABLE);
}

// 函数功能：使能ms定时器（5s）
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void RTC_enableRTCForStop(void)
{
    /*定义数据结构*/
    EXTI_InitTypeDef EXTI_InitStructure;

    PWR_RTCAccessCmd(ENABLE);
    RTC_ITConfig(RTC_IT_WUT, DISABLE);
    RTC_WakeUpCmd(DISABLE);

    /*清除EXTI20中断请求*/
    EXTI_ClearITPendingBit(EXTI_Line20);

    /*初始化外部中断20*/
    EXTI_InitStructure.EXTI_Line = EXTI_Line20;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*初始化RTC唤醒中断的中断向量*/
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PRI_MCU_TIMER;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /*设置RTC的输入时钟1HZ*/
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);

    /*设置RTC唤醒计数器的重置值为5,这样就相当于RTC定时频率为(1 * 5)s*/
    RTC_SetWakeUpCounter(RTC_STOP_TIME / 1000);

    /*允许RTC唤醒产生中断*/
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

    /*启动RTC唤醒功能*/
    RTC_WakeUpCmd(ENABLE);
}

// 函数功能：获取ms定时器计数值
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void RTC_obtainMilliSeconds(uint64 *const lastestSecond)
{
    __disable_interrupt();
	
    *lastestSecond = rtcMilliSeconds;
	
    __enable_interrupt();
}

// 函数功能：检查定时器是否超时
// 输入参数：	checkSecond	--	待检测的定时常数
//				thresholdSeccond -- 超时门限值
// 输出参数：无
// 返 回 值：无
// 备    注：无
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

// 函数功能：定时器更新
// 输入参数：interval -- 累加量
// 输出参数：无
// 返 回 值：无
// 备    注：无
static void RTC_updateTimer(const uint16 interval)
{
    __disable_interrupt();
	
    rtcMilliSeconds += interval;
	  
	__enable_interrupt();
}

// 函数功能：定时器中断
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void RTC_WKUP_IRQHandler(void)
{
    if (Event_getCurrentMode() == TASK_TRACKER)
	{
        //更新计时器
		RTC_updateTimer(RTC_WORK_TIME);
		
		//按键扫描
		Key_DEV_ScanKey();
		
		//USB端口扫描
		VBUS_DEV_Scan();
		
		//蓝牙延时
		BleDelay(1);
	}
    else if (Event_getCurrentMode() == TASK_SLEEP)
	{
		//更新计时器
		RTC_updateTimer(RTC_WORK_TIME);
		
		//按键扫描
		Key_DEV_ScanKey();
		
		//USB端口扫描
		VBUS_DEV_Scan();
	}
	else if(Event_getCurrentMode() == TASK_DEEPSLEEP)
	{
		//更新计时器
		RTC_updateTimer(RTC_WORK_TIME);
		
		//按键扫描
		Key_DEV_ScanKey();
	}
	else if(Event_getCurrentMode() == TASK_STOP)
	{
		//更新计时器
		RTC_updateTimer(RTC_STOP_TIME);
	}
	  
#if	(WATCHDOG == ON)
		
	//喂狗
	RTC_updateWatchDog();

#endif
	
	//清除RTC中的唤醒标志
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	/*特别注意：下面的2个函数调用都不能去掉，否则会出现不正常的情况
      还有一种解决办法，就是先清除挂起寄存器，然户再清除RTC
      清除外部中断20的请求标志
	*/
	EXTI_ClearITPendingBit(EXTI_Line20);
	
	//清除外部中断20的挂起寄存器中的中断请求标志
	EXTI_ClearFlag(EXTI_Line20);
}

// 函数功能：闹钟中断
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void RTC_Alarm_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_ALRA) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_ALRA);
        EXTI_ClearITPendingBit(EXTI_Line17);

		//数据存储
		DataSave_alarm();
    }
}
