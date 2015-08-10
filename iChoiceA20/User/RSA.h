#ifndef __RSA_H
#define __RSA_H

#include "Type.h"

#define         RSA_ON                      1
#define			Prime_stat_1				43//密钥生成素数1
#define			Prime_stat_2				47//密钥生成素数2
#define         Prime_stat_3				53//密钥生成素数3
#define			Rsa_Key						31//公钥

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
uint32 Get_d (void);//获取下位机密钥
uint32 candp (uint32 mc, uint32 d, uint32 Euler);//加密/揭秘函数核心部件
uint32 EncrData (uint32 Data);//封装后的API函数，加密一个8/16位数据
uint32 DecrData (uint32 Data);//封装后的API函数，解密一个32/24/位加密数据
void RsaTest (void);//演示函数


uint32 Get_d_pc (void);//获取上位机密钥
uint32 DecrData_pc (uint32 Data);//封装后的API函数，解密上位机一个32/24位加密数据

#endif