#ifndef __RSA_H
#define __RSA_H

#include "Type.h"

#define         RSA_ON                      1
#define			Prime_stat_1				43//��Կ��������1
#define			Prime_stat_2				47//��Կ��������2
#define         Prime_stat_3				53//��Կ��������3
#define			Rsa_Key						31//��Կ

typedef struct RSA_DATA
{
  uint8  Rsa_Rand_Data [8];
  uint8  Rsa_Rand_Save_Data [8];
  uint32  Rsa_Data [8];
  uint32  d;
  uint32  d_pc;
  uint32  temp;
  uint8 sum;
  uint8 lock;
}RSA_DATA;

extern RSA_DATA  RsaData;
uint32 Get_d (void);//��ȡ��λ����Կ
uint32 candp (uint32 mc, uint32 d, uint32 Euler);//����/���غ������Ĳ���
uint32 EncrData (uint32 Data);//��װ���API����������һ��8/16λ����
uint32 DecrData (uint32 Data);//��װ���API����������һ��32/24/λ��������
void RsaTest (void);//��ʾ����


uint32 Get_d_pc (void);//��ȡ��λ����Կ
uint32 DecrData_pc (uint32 Data);//��װ���API������������λ��һ��32/24λ��������

#endif