/*********************************************************/
/*modified by liubingyan*/
/*********************************************************/
#ifndef __CHARACTER_H
#define __CHARACTER_H

#include "type.h"


#define IMAGE_DISTANCE    0
#define IMAGE_PM          1
#define IMAGE_AM          2
#define IMAGE_TEMPERATURE 3
#define IMAGE_GOAL        4
#define IMAGE_ALTITUDE    5
#define IMAGE_STEP        6
#define IMAGE_CALORY      7
#define IMAGE_MPH         8
#define IMAGE_KPH         9
#define IMAGE_FAT         10
#define IMAGE_CLIMB       11
#define IMAGE_MBAR        12
#define IMAGE_CELSIUS     13

extern const uint8 Speed[60];
extern const uint8 ASIIC_NEW[52][52];
extern const uint8 PIC[14][128];
extern const uint8 ichoice[];
extern const uint8 batterylow[];
extern const uint8 batterycharging0[];
extern const uint8 batterycharging1[];
extern const uint8 batterycharging2[];
extern const uint8 batterycharging3[];


















#endif