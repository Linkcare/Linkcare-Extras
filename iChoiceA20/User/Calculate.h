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
	uint32 climbAltitude;	/*当天累积爬升高度	，每天0点清零*/
	uint32 distance; 		/*当天路程总量		，每天0点清零*/
	uint32 steps; 			/*当天计步总数		，每天0点清零*/
   	uint32 calories; 		/*当天卡路里燃烧总量，每天0点清零*/
	uint16 speed;      		/*最近5s内的平均速度			 */
   	uint16 fat; 			/*当天脂肪燃烧总量	，每天0点清零*/
   	int16  altitude; 		/*当前海拔高度，实时，每天0点清零*/
   	int16  temperature; 	/*当前环境温度，实时，每天0点清零*/
	uint16 percent; 		/*当前目标完成百分比			 */
	
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