/*********************************************************/
/*modified by liubingyan*/
/*********************************************************/
#include "type.h"
#include "stdlib.h"
#include "LPS331AP.h"

#include "ACCEL.h"
#include "Calculate.h"
#include "TimeManage.h"
#include "Configuration.h"
#include "DeviceInfo.h"
#include "Event.h"
#include "Task.h"

#define  FILTER_NUMBER 	    32  // 海拔和温度滤波均值样本数

#define  AVERAGE_NUMBER 	5   // 平均速度均值样本数

/********************************************************/
//用户运动数据（显示）
static SportDataForDisplay  sportDataForDisplay;	  

//速度均值数组
static uint16 distanceInfiveSec[6];

//海拔高度均值滤波数组
static int16  HighFilter[FILTER_NUMBER];

//环境温度均值滤波数组
static int16  TempFilter[FILTER_NUMBER];

//上一次的更新海拔高度，用于爬高计算
static int16  lastAltitude 			= 0;

//上一次的更新步数，用于运动情况判断
static uint32 lastStepQuantity 		= 0;	

//上一分钟的脚步数，用于每分钟的步数增量计算
static uint16  StepsInLastMin 		= 0;	

//上一分钟的爬升高度，用于每分钟的爬高增量计算
static uint16  AltitudeInLastMin 	= 0;	

//上一次速度计算的时间（秒）坐标
static int8  lastSecond  = -2;

//用户是否在运动
static uint8 isMoving = NO_MOVING;		

//睡眠数据
extern uint8 SleepData;	

//上一次的更新步数，用于步数计算
uint32 lastStepsForCalculate = 0;
/*********************************************
*计步模式下的数据计算
*计算输出量				单位:
*	 climbAltitude			m,米
*	 distance				m,米
*	 steps					m,步
*    calorie				m,米
*    altitude				m,米
*	 speed;					0.1m/s,米每秒
*    fat					g,克
*    temperature			℃,摄氏度
*	 percent				1%
*	(输出均为公制单位)
*********************************************/
void DataCalculate(void)
{	
 
	/****************************************************/ 
	/****************************************************/ 
		//步数计算
		sportDataForDisplay.steps += Count_Step - lastStepsForCalculate;

		//最大步数99999
		if(sportDataForDisplay.steps > 99999)
		{
			sportDataForDisplay.steps = 99999;
		}

		lastStepsForCalculate = Count_Step;
	/****************************************************/
	/****************************************************/
		//爬升高度计算，单位：m，运动情况判断	
		if(sportDataForDisplay.steps  > lastStepQuantity)
		{  	

			lastStepQuantity = sportDataForDisplay.steps;
				
			isMoving = IS_MOVING;
		}
		else
		{
			isMoving = NO_MOVING;
		}
		return;
}	

// 函数功能：获取每分钟运动数据
// 输入参数：无
// 输出参数：SportDataForStorage* data
// 返 回 值：无
// 备    注：存储时调用
void Calculate_getSportDataForMin(SportDataForStorage* data)
{		
  	data->steps = 0;
	data->sleepTime = 0;
	data->climbAltitude = 0;
	data->sportState = 0;
	
	//记步数据
	if(Event_getCurrentMode() == TASK_TRACKER || (Event_getCurrentMode() == TASK_STOP && Event_getLastMode() == TASK_TRACKER))
	{
		//计算本分钟总步数
		data->steps = sportDataForDisplay.steps - StepsInLastMin;

		if(data->steps > 350)
		{
			data->steps = 350;
		}
		
		//计算本分钟总爬升高度
		data->climbAltitude = sportDataForDisplay.climbAltitude - AltitudeInLastMin;
		
		if(data->climbAltitude > 63)
		{
			data->climbAltitude = 63;
		}
		
		//记录数据类型
		data->sportState = TYPE_STEP;
		
		//更新中间变量
		if(data->steps > 0)
		{
			StepsInLastMin = sportDataForDisplay.steps;
		}
			
		//更新中间变量
		if(data->climbAltitude > 0)
		{
			AltitudeInLastMin = sportDataForDisplay.climbAltitude;
		}
	}
	//睡眠数据
	else if(Event_getCurrentMode() == TASK_SLEEP || (Event_getCurrentMode() == TASK_STOP && Event_getLastMode() == TASK_SLEEP))
	{
	  	//计算本分钟总步数
		data->steps = sportDataForDisplay.steps - StepsInLastMin;

		if(data->steps > 350)
		{
			data->steps = 350;
		}
		
		//计算本分钟总爬升高度
		data->climbAltitude = sportDataForDisplay.climbAltitude - AltitudeInLastMin;
		
		if(data->climbAltitude > 63)
		{
			data->climbAltitude = 63;
		}
		
		//更新中间变量
		if(data->steps > 0)
		{
			StepsInLastMin = sportDataForDisplay.steps;
		}
			
		if(data->climbAltitude > 0)
		{
			AltitudeInLastMin = sportDataForDisplay.climbAltitude;
		}

		//获取睡眠数据
		if(SleepData > 60)
		{
			data->sleepTime = 60;
		}
		else
		{
			data->sleepTime = SleepData;
		}

		//更新数据类型		
		data->sportState = TYPE_SLEEP;
	}
	else
	{
	  	//清零
		data->steps = 0;
		data->sleepTime = 0;
		data->climbAltitude = 0;
		data->sportState = 0;
	}
}

// 函数功能：获取目标完成率
// 输入参数：无
// 输出参数：无
// 返 回 值：目标完成率
// 备    注：无
uint8 Calculate_getPercent(void)
{
	return sportDataForDisplay.percent;
}

// 函数功能：获取步行距离
// 输入参数：无
// 输出参数：无
// 返 回 值：步行距离
// 备    注：无
uint16 Calculate_getDistance(void)
{
	return sportDataForDisplay.distance;
}

// 函数功能：获取速度
// 输入参数：无
// 输出参数：无
// 返 回 值：速度
// 备    注：无
uint16 Calculate_getSpeed(void)
{
	return sportDataForDisplay.speed;
}

// 函数功能：获取海拔高度
// 输入参数：无
// 输出参数：无
// 返 回 值：海拔高度
// 备    注：无
int16 Calculate_getAltitude(void)
{
	return sportDataForDisplay.altitude;
}

// 函数功能：获取步数
// 输入参数：无
// 输出参数：无
// 返 回 值：步数
// 备    注：无
uint32 Calculate_getSteps(void)
{	
	return sportDataForDisplay.steps;
}

// 函数功能：获取卡路里消耗量
// 输入参数：无
// 输出参数：无
// 返 回 值：卡路里消耗量
// 备    注：无
uint32 Calculate_getCalories(void)
{
	return sportDataForDisplay.calories;
}

// 函数功能：获取脂肪消耗量
// 输入参数：无
// 输出参数：无
// 返 回 值：脂肪消耗量
// 备    注：无
uint16 Calculate_getFat(void)
{
	return sportDataForDisplay.fat;
}

// 函数功能：获取环境温度
// 输入参数：无
// 输出参数：无
// 返 回 值：环境温度
// 备    注：无
int16 Calculate_getTemperature(void)
{
	return sportDataForDisplay.temperature;
}

// 函数功能：获取爬升高度
// 输入参数：无
// 输出参数：无
// 返 回 值：爬升高度
// 备    注：无
uint32 Calculate_getClimbAltitude(void)
{
	return sportDataForDisplay.climbAltitude;
}

// 函数功能：获取用户运动状态
// 输入参数：无
// 输出参数：无
// 返 回 值：Moving_TypeDef
// 备    注：无
Moving_TypeDef Calculate_getUserState(void)
{
  	if(isMoving == IS_MOVING)
	{
		return IS_MOVING;
	}
	else
	{
		return NO_MOVING;
	}
}

#if	(BACKUP == ON)

// 函数功能：数据计算单元备份初始化
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Calculate_recover(void)
{
	UserConfig_getClimbBackup(&sportDataForDisplay.climbAltitude);
	UserConfig_getStepDisplay(&sportDataForDisplay.steps);
}
#endif

// 函数功能：数据计算单元初始化
// 输入参数：无
// 输出参数：无
// 返 回 值：无
// 备    注：无
void Calculate_init(void)
{
  	//初始化计步算法	
	Init_ACCEL();		
	
	//中间变量初始化
	lastStepQuantity 	= 0;
	lastAltitude            = 0;		
	  
	StepsInLastMin 		= 0;
	AltitudeInLastMin 	= 0; 	 	
	
	//显示初始化
	sportDataForDisplay.fat 		  = 0;	
	sportDataForDisplay.speed 		  = 0;	
	sportDataForDisplay.steps 		  = 0;	
	sportDataForDisplay.percent		  = 0;	
	sportDataForDisplay.distance 	  = 0;	
	sportDataForDisplay.calories 	  = 0;	
	sportDataForDisplay.altitude 	  = 0;	
	sportDataForDisplay.temperature   = 0;	
	sportDataForDisplay.climbAltitude = 0;
}

