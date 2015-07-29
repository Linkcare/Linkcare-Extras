#ifndef	__TYPE_H
#define	__TYPE_H

#include "stm32l1xx.h"
#include "ResourceAlloc.h"

typedef		unsigned char  			uint8;		
typedef		unsigned short 			uint16;		
typedef		unsigned int 			uint32;
typedef		unsigned long long		uint64;	

typedef		signed char				int8;		
typedef		signed short			int16;		
typedef		signed int				int32;		
typedef		signed long long		int64;		

/*BIT0~BIT7*/
#define 	BIT0     	(0x0001u)
#define 	BIT1      	(0x0002u)
#define 	BIT2     	(0x0004u)
#define 	BIT3      	(0x0008u)
#define 	BIT4     	(0x0010u)
#define 	BIT5     	(0x0020u)
#define 	BIT6    	(0x0040u)
#define 	BIT7    	(0x0080u)

#endif