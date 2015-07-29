#include "stm32l1xx.h"
/////////////////////////////////////////
//���ݶ���
#define bool_ACC uint8_t
#define true_ACC 1
#define false_ACC 0

#define bit8_ACC int8_t
#define bit16_ACC int32_t
#define bit32_ACC int32_t
#define bit64_ACC uint64_t
///////////////////////////////////////////



///////////////////////////////////////////
//��ʼ�趨
#define MinTHRESHOLDPEAK 30L
#define corr1     1L
#define corr2   6L

#define SAMPLE 50UL//������100
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
//�������
extern bit32_ACC Count_Step ;//�ǲ���
extern bit16_ACC  Data_ACCEL ;//�ͼ��ٶ�����
extern bool_ACC Valid_Step ;//�������
extern bit64_ACC Time_Count_Step  ;//�����߲�ʱ��
extern bit64_ACC CTime_Count_Step ;//��ʱ�߲�ʱ��

extern void Init_ACCEL();//��ʼ��
extern void main_ACCEL();//������
extern void Init_ACCEL_recover(void);
/////////////////////////////////////////////





/////////////////////////////////////////
//�㷨���̲���
extern bit16_ACC filterData ;
extern bit16_ACC lastfilterData;
extern bit16_ACC THRESHOLDPEAK;

extern bit16_ACC AVGTHRESHOLD ;
/////////////////////////////////////////
