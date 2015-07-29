#ifndef __DATACALCULATE_H
#define __DATACALCULATE_H

#include "type.h"


typedef enum
{
	NO_MOVING	=	0x00,
	IS_MOVING	=	0x01

}Moving_TypeDef;

#define		TYPE_STEP	0x00
#define		TYPE_SLEEP	0x01
/********************************************************/
typedef struct  
{  
	uint32 climbAltitude;	/*�����ۻ������߶�	��ÿ��0������*/
	uint32 distance; 		/*����·������		��ÿ��0������*/
	uint32 steps; 			/*����Ʋ�����		��ÿ��0������*/
   	uint32 calories; 		/*���쿨·��ȼ��������ÿ��0������*/
	uint16 speed;      		/*���5s�ڵ�ƽ���ٶ�			 */
   	uint16 fat; 			/*����֬��ȼ������	��ÿ��0������*/
   	int16  altitude; 		/*��ǰ���θ߶ȣ�ʵʱ��ÿ��0������*/
   	int16  temperature; 	/*��ǰ�����¶ȣ�ʵʱ��ÿ��0������*/
	uint16 percent; 		/*��ǰĿ����ɰٷֱ�			 */
	
}SportDataForDisplay ; 
/********************************************************/
typedef struct  
{  			
  	uint16 steps:9,
		   climbAltitude:6;
	uint8  sleepTime;
	uint8  sportState;
	
}SportDataForStorage;
/********************************************************/

void   Calculate_init(void);
void   DataCalculate(void);
uint8  Calculate_getPercent(void);
uint16 Calculate_getDistance(void);
uint16 Calculate_getSpeed(void);
int16  Calculate_getAltitude(void);
uint32 Calculate_getClimbAltitude(void);
uint32 Calculate_getSteps(void);
uint32 Calculate_getCalories(void);
uint16 Calculate_getFat(void);
int16  Calculate_getTemperature(void);
void   Calculate_getSportDataForMin(SportDataForStorage* data);
void   Calculate_recover(void);

Moving_TypeDef       Calculate_getUserState(void);
SportDataForStorage *Calculate_getSleepDataForMin(void);
SportDataForStorage *Calculate_getSleepDataForMin(void);

#endif