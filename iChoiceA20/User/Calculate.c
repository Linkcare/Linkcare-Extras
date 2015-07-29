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

#define  FILTER_NUMBER 	    32  // ���κ��¶��˲���ֵ������

#define  AVERAGE_NUMBER 	5   // ƽ���ٶȾ�ֵ������

/********************************************************/
//�û��˶����ݣ���ʾ��
static SportDataForDisplay  sportDataForDisplay;	  

//�ٶȾ�ֵ����
static uint16 distanceInfiveSec[6];

//���θ߶Ⱦ�ֵ�˲�����
static int16  HighFilter[FILTER_NUMBER];

//�����¶Ⱦ�ֵ�˲�����
static int16  TempFilter[FILTER_NUMBER];

//��һ�εĸ��º��θ߶ȣ��������߼���
static int16  lastAltitude 			= 0;

//��һ�εĸ��²����������˶�����ж�
static uint32 lastStepQuantity 		= 0;	

//��һ���ӵĽŲ���������ÿ���ӵĲ�����������
static uint16  StepsInLastMin 		= 0;	

//��һ���ӵ������߶ȣ�����ÿ���ӵ�������������
static uint16  AltitudeInLastMin 	= 0;	

//��һ���ٶȼ����ʱ�䣨�룩����
static int8  lastSecond  = -2;

//�û��Ƿ����˶�
static uint8 isMoving = NO_MOVING;		

//˯������
extern uint8 SleepData;	

//��һ�εĸ��²��������ڲ�������
uint32 lastStepsForCalculate = 0;
/*********************************************
*�Ʋ�ģʽ�µ����ݼ���
*���������				��λ:
*	 climbAltitude			m,��
*	 distance				m,��
*	 steps					m,��
*    calorie				m,��
*    altitude				m,��
*	 speed;					0.1m/s,��ÿ��
*    fat					g,��
*    temperature			��,���϶�
*	 percent				1%
*	(�����Ϊ���Ƶ�λ)
*********************************************/
void DataCalculate(void)
{	
 
	/****************************************************/ 
	/****************************************************/ 
		//��������
		sportDataForDisplay.steps += Count_Step - lastStepsForCalculate;

		//�����99999
		if(sportDataForDisplay.steps > 99999)
		{
			sportDataForDisplay.steps = 99999;
		}

		lastStepsForCalculate = Count_Step;
	/****************************************************/
	/****************************************************/
		//�����߶ȼ��㣬��λ��m���˶�����ж�	
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

// �������ܣ���ȡÿ�����˶�����
// �����������
// ���������SportDataForStorage* data
// �� �� ֵ����
// ��    ע���洢ʱ����
void Calculate_getSportDataForMin(SportDataForStorage* data)
{		
  	data->steps = 0;
	data->sleepTime = 0;
	data->climbAltitude = 0;
	data->sportState = 0;
	
	//�ǲ�����
	if(Event_getCurrentMode() == TASK_TRACKER || (Event_getCurrentMode() == TASK_STOP && Event_getLastMode() == TASK_TRACKER))
	{
		//���㱾�����ܲ���
		data->steps = sportDataForDisplay.steps - StepsInLastMin;

		if(data->steps > 350)
		{
			data->steps = 350;
		}
		
		//���㱾�����������߶�
		data->climbAltitude = sportDataForDisplay.climbAltitude - AltitudeInLastMin;
		
		if(data->climbAltitude > 63)
		{
			data->climbAltitude = 63;
		}
		
		//��¼��������
		data->sportState = TYPE_STEP;
		
		//�����м����
		if(data->steps > 0)
		{
			StepsInLastMin = sportDataForDisplay.steps;
		}
			
		//�����м����
		if(data->climbAltitude > 0)
		{
			AltitudeInLastMin = sportDataForDisplay.climbAltitude;
		}
	}
	//˯������
	else if(Event_getCurrentMode() == TASK_SLEEP || (Event_getCurrentMode() == TASK_STOP && Event_getLastMode() == TASK_SLEEP))
	{
	  	//���㱾�����ܲ���
		data->steps = sportDataForDisplay.steps - StepsInLastMin;

		if(data->steps > 350)
		{
			data->steps = 350;
		}
		
		//���㱾�����������߶�
		data->climbAltitude = sportDataForDisplay.climbAltitude - AltitudeInLastMin;
		
		if(data->climbAltitude > 63)
		{
			data->climbAltitude = 63;
		}
		
		//�����м����
		if(data->steps > 0)
		{
			StepsInLastMin = sportDataForDisplay.steps;
		}
			
		if(data->climbAltitude > 0)
		{
			AltitudeInLastMin = sportDataForDisplay.climbAltitude;
		}

		//��ȡ˯������
		if(SleepData > 60)
		{
			data->sleepTime = 60;
		}
		else
		{
			data->sleepTime = SleepData;
		}

		//������������		
		data->sportState = TYPE_SLEEP;
	}
	else
	{
	  	//����
		data->steps = 0;
		data->sleepTime = 0;
		data->climbAltitude = 0;
		data->sportState = 0;
	}
}

// �������ܣ���ȡĿ�������
// �����������
// �����������
// �� �� ֵ��Ŀ�������
// ��    ע����
uint8 Calculate_getPercent(void)
{
	return sportDataForDisplay.percent;
}

// �������ܣ���ȡ���о���
// �����������
// �����������
// �� �� ֵ�����о���
// ��    ע����
uint16 Calculate_getDistance(void)
{
	return sportDataForDisplay.distance;
}

// �������ܣ���ȡ�ٶ�
// �����������
// �����������
// �� �� ֵ���ٶ�
// ��    ע����
uint16 Calculate_getSpeed(void)
{
	return sportDataForDisplay.speed;
}

// �������ܣ���ȡ���θ߶�
// �����������
// �����������
// �� �� ֵ�����θ߶�
// ��    ע����
int16 Calculate_getAltitude(void)
{
	return sportDataForDisplay.altitude;
}

// �������ܣ���ȡ����
// �����������
// �����������
// �� �� ֵ������
// ��    ע����
uint32 Calculate_getSteps(void)
{	
	return sportDataForDisplay.steps;
}

// �������ܣ���ȡ��·��������
// �����������
// �����������
// �� �� ֵ����·��������
// ��    ע����
uint32 Calculate_getCalories(void)
{
	return sportDataForDisplay.calories;
}

// �������ܣ���ȡ֬��������
// �����������
// �����������
// �� �� ֵ��֬��������
// ��    ע����
uint16 Calculate_getFat(void)
{
	return sportDataForDisplay.fat;
}

// �������ܣ���ȡ�����¶�
// �����������
// �����������
// �� �� ֵ�������¶�
// ��    ע����
int16 Calculate_getTemperature(void)
{
	return sportDataForDisplay.temperature;
}

// �������ܣ���ȡ�����߶�
// �����������
// �����������
// �� �� ֵ�������߶�
// ��    ע����
uint32 Calculate_getClimbAltitude(void)
{
	return sportDataForDisplay.climbAltitude;
}

// �������ܣ���ȡ�û��˶�״̬
// �����������
// �����������
// �� �� ֵ��Moving_TypeDef
// ��    ע����
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

// �������ܣ����ݼ��㵥Ԫ���ݳ�ʼ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void Calculate_recover(void)
{
	UserConfig_getClimbBackup(&sportDataForDisplay.climbAltitude);
	UserConfig_getStepDisplay(&sportDataForDisplay.steps);
}
#endif

// �������ܣ����ݼ��㵥Ԫ��ʼ��
// �����������
// �����������
// �� �� ֵ����
// ��    ע����
void Calculate_init(void)
{
  	//��ʼ���Ʋ��㷨	
	Init_ACCEL();		
	
	//�м������ʼ��
	lastStepQuantity 	= 0;
	lastAltitude            = 0;		
	  
	StepsInLastMin 		= 0;
	AltitudeInLastMin 	= 0; 	 	
	
	//��ʾ��ʼ��
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

