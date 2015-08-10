#include "stm32l1xx.h"
/////////////////////////////////////////
//数据定义
#define bool_ACC uint8_t
#define true_ACC 1
#define false_ACC 0

#define bit8_ACC int8_t
#define bit16_ACC int32_t
#define bit32_ACC int32_t
#define bit64_ACC uint64_t
///////////////////////////////////////////



///////////////////////////////////////////
//初始设定
#define MinTHRESHOLDPEAK 30L
#define corr1     1L
#define corr2   6L

#define SAMPLE 50UL//采样率100
#define MS1000 SAMPLE 
#define MS50  SAMPLE/20
#define MS150  3*SAMPLE/10
#define MS100  SAMPLE/10
#define MS500  SAMPLE/2
#define MS400  2*SAMPLE/5
#define MS700  7*SAMPLE/10
#define MS250  SAMPLE/4
#define MS850  17*SAMPLE/20
#define MS600  6*SAMPLE/10
#define Len_Mem SAMPLE/3


///////////////////////////////////////////


////////////////////////////////////////////
//输入输出
extern bit32_ACC Count_Step ;//记步数
extern bit16_ACC  Data_ACCEL ;//和加速度数据
extern bool_ACC Valid_Step ;//迈步标记
extern bit64_ACC Time_Count_Step  ;//连续走步时间
extern bit64_ACC CTime_Count_Step ;//临时走步时间

extern void Init_ACCEL();//初始化
extern void main_ACCEL();//主函数
extern void Init_ACCEL_recover(void);
/////////////////////////////////////////////





/////////////////////////////////////////
//算法过程参量
extern bit16_ACC filterData ;
extern bit16_ACC lastfilterData;
extern bit16_ACC THRESHOLDPEAK;

extern bit16_ACC AVGTHRESHOLD ;
/////////////////////////////////////////
